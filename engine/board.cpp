
#include	"board.h"
#include	"config.h"
#include	"mt19937ar.h"



/*
 *	駒の動き
 */
int CBoard::PieceMoveNear[30][16] = {
	{                            }, {},	//	空
	{     -7,                    }, {},	//	歩
	{ -8, -7, -6,        6,    8 }, {},	//	銀
	{                            }, {},	//	角
	{                            }, {},	//	飛
	{ -8, -7, -6, -1, 1,    7,   }, {},	//	金
	{ -8, -7, -6, -1, 1, 6, 7, 8 }, {},	//	玉
	{ -8, -7, -6, -1, 1,    7,   }, {},	//	と
	{ -8, -7, -6, -1, 1,    7,   }, {},	//	全
	{     -7,     -1, 1,    7,   }, {},	//	馬
	{ -8,     -6,        6,    8 }, {},	//	龍
};

int CBoard::PieceMoveFar[30][16] = {
	{                            }, {},	//	空
	{                            }, {},	//	歩
	{                            }, {},	//	銀
	{ -8,     -6,        6,    8 }, {},	//	角
	{     -7,     -1, 1,    7,   }, {},	//	飛
	{                            }, {},	//	金
	{                            }, {},	//	玉
	{                            }, {},	//	と
	{                            }, {},	//	全
	{ -8,     -6,        6,    8 }, {},	//	馬
	{     -7,     -1, 1,    7,   }, {},	//	龍
};

int *CBoard::PieceMoveToEffectNear;
int CBoard::PieceMoveToEffectNearTemp[21];
int *CBoard::PieceMoveToEffectFar;
int CBoard::PieceMoveToEffectFarTemp[21];



/*
 *	ハッシュ用乱数
 */
HASH CBoard::HashBoard[49][32];
HASH CBoard::HashHand[14][32];
HASH CBoard::HashTurn;



/*
 *	Initialize
 *		初期化
 */
void CBoard::Initialize()
{
	GlobalInitialize();

	const char board[121] = {
		 1,  1,  1,  1,  1,  1,  1,
		 1, 13, 11,  5,  7,  9,  1,
		 1,  3,  0,  0,  0,  0,  1,
		 1,  0,  0,  0,  0,  0,  1,
		 1,  0,  0,  0,  0,  2,  1,
		 1,  8,  6,  4, 10, 12,  1,
		 1,  1,  1,  1,  1,  1,  1,
	};

	memcpy( Board, board, sizeof Board );
	memset( Hand, 0, sizeof Hand );
	Turn = 0;
	Step = 0;

	Update();
}



/*
 *	Globalnitialize
 *		初期化（全体）
 */
void CBoard::GlobalInitialize()
{
	#define ELEMNUM(a) (sizeof (a)/sizeof (a)[0])

	static bool initialized = false;
	if ( initialized )
		return;

	//	PieceMove
	for ( int i=0; i<sizeof PieceMoveFar/sizeof PieceMoveFar[0]; i+=2 )
		for ( int j=0; j<sizeof PieceMoveFar[0]/sizeof PieceMoveFar[0][0]; j++ )
			PieceMoveFar[i+1][j] = -PieceMoveFar[i][j];

	for ( int i=0; i<sizeof PieceMoveNear/sizeof PieceMoveNear[0]; i+=2 )
		for ( int j=0; j<sizeof PieceMoveNear[0]/sizeof PieceMoveNear[0][0]; j++ )
			PieceMoveNear[i+1][j] = -PieceMoveNear[i][j];

	//	PieceMoveToEffect
	PieceMoveToEffectNear = PieceMoveToEffectNearTemp + 10;
	PieceMoveToEffectFar = PieceMoveToEffectFarTemp + 10;

	int near[] = { -8, -7, -6, -1, +1, +6, +7, +8 };
	for ( int i=0; i<8; i++ )
		PieceMoveToEffectNear[near[i]] = 1 << (i+8);

	int far[] = { -8, -7, -6, -1, +1, +6, +7, +8 };
	for ( int i=0; i<8; i++ )
		PieceMoveToEffectFar[far[i]] = 1 << i;

	//	HashBoard
	for ( int i=0; i<ELEMNUM(HashBoard); i++ )
		for ( int j=2; j<ELEMNUM(HashBoard[i]); j++ )		//	空・壁は0
			HashBoard[i][j] = (HASH)genrand_int32()<<32 | genrand_int32();

	//	HashHand
	for ( int i=2; i<ELEMNUM(HashHand); i++ )				//	空・壁は0
		for ( int j=1; j<ELEMNUM(HashHand[i]); j++ )		//	持駒無しは0
			HashHand[i][j] = (HASH)genrand_int32()<<32 | genrand_int32();

	//	HashTurn
	HashTurn = (HASH)genrand_int32()<<32 | genrand_int32();

	initialized = true;

	#undef ELEMNUM
}



/*
 *	To/FromString
 *		盤面を文字列に変換・文字列から読み込み
 */
string CBoard::ToString() const
{
	string s;

	for ( int y=0; y<5; y++ )
	for ( int x=0; x<5; x++ )
		s += 'a' + Board[(y+1)*7+(x+1)];

	for ( int i=2; i<14; i++ )
		s += '0' + Hand[i];

	s += Turn == 0 ? "+" : "-";

	return s;
}

bool CBoard::FromString( string s )
{
	if ( s.length() < 38 )
		return false;

	Initialize();

	for ( int y=0; y<5; y++ )
	for ( int x=0; x<5; x++ )
		Board[(y+1)*7+(x+1)] = s[y*5+x] - 'a';

	for ( int i=2; i<14; i++ )
		Hand[i] = s[i-2+25] - '0';

	Turn = s[37] == '+' ? 0 : 1;
	Step = 0;

	Update();

	return true;
}



/*
 *	GetMove
 *		着手可能手を取得
 */
int CBoard::GetMove( MOVE move[MAXMOVE], int flag/*=0*/ ) const
{
	#define ADDMOVE(m)					\
		if ( CheckMoveCore( m, flag ) )	\
			move[num++] = m;

	int num = 0;

	int fp;

	for ( int from=8; from<=40; from++ )
	if ( IsFriend( ( fp = Board[from] ) ) )
	{
		MOVE m( from, 0, false, 0 );

		//	１マス
		int *near = PieceMoveNear[fp];
		for ( int i=0; near[i]!=0; i++ )
		{
			m.to = m.from + near[i];

			if ( fp < KIN )
			{
				m.promote = true;
				ADDMOVE( m )
				m.promote = false;
			}

			ADDMOVE( m )
		}

		//	飛駒
		int *far = PieceMoveFar[fp];
		for ( int i=0; far[i]!=0; i++ )
		{
			for( int to = from+far[i];
				 to == from + far[i]  ||  Board[to-far[i]] == SPACE;
				 to += far[i] )
			{
				m.to = to;

				if ( fp < KIN )
				{
					m.promote = true;
					ADDMOVE( m )
					m.promote= false;
				}

				ADDMOVE( m )
			}
		}
	}

	//	駒打ち
	if ( ! ( flag & MV_CAPTURE ) )
	{
		for ( int hand=FU+Turn; hand<TO+Turn; hand+=2 )
		if ( Hand[hand] != 0 )
		{
			MOVE m( 0, 0, false, hand );

			for ( int to=8; to<=40; to++ )
			if ( Board[to] == SPACE )
			{
				m.to = to;
				ADDMOVE( m )
			}
		}
	}

	return num;

	#undef ADDMOVE
}



/*
 *	CheckMove
 *		moveが着手可能かどうか判定
 */
bool CBoard::CheckMove( MOVE move, int flag/*=0*/ ) const
{
	//	nullは常に無効
	if ( move == MOVE::null )
		return false;

	//	移動元移動先の範囲
	if ( move.from < 0  ||  49 <= move.from  ||
		 move.to < 0  ||  49 <= move.to )
		return false;

	//	駒打で移動元は0か
	if ( move.hand  &&  move.from != 0 )
		return false;

	int fp = Board[move.from];
	int tp = Board[move.to];

	//	移動元に駒があるか
	if ( move.hand == 0  &&  ! IsFriend(fp) )
		return false;
	if ( move.hand != 0  &&  Hand[move.hand] == 0 )
		return false;

	//	移動先に移動できるか
	if ( move.hand == 0  &&  tp != SPACE  &&  ! IsEnemy(tp)  ||
		 move.hand != 0  &&  tp != SPACE )
		return false;

	//	動き方は正しいか
	if ( move.hand == 0 )
	{
		bool valid = false;

		int *near = PieceMoveNear[fp];
		for ( int i=0; near[i]!=0; i++ )
			if ( move.from + near[i] == move.to )
				valid = true;

		int *far = PieceMoveFar[fp];
		for ( int i=0; far[i]!=0; i++ )
		{
			int to;
			for ( to = move.from + far[i];
				  Board[to] == SPACE;
				  to += far[i] )
				if ( to == move.to )
					valid = true;
			if ( to == move.to )
				valid = true;
		}

		if ( ! valid )
			return false;
	}

	//	成り
	if ( move.promote )
	{
		if ( move.hand != 0 )
			return false;

		if ( fp >= KIN )
			return false;

		//	敵陣かどうかはcoreで判定
	}

	return CheckMoveCore( move, flag );
}


/*
 *	Move
 *		手を指す
 */
void CBoard::Move( MOVE move )
{
	int value = Evaluator.GetValue( this );

	//	移動先に駒があれば駒台へ
	int rp = Board[move.to];
	if ( rp != SPACE )
		Remove( move.to ),
		PutHand( ( rp<TO ? rp : rp-PROMOTE ) ^ 1 );

	//	移動元から駒を取る
	int p;
	if ( move.hand == 0 )
		p = Board[move.from],
		Remove( move.from );
	else
		p = move.hand,
		RemoveHand( move.hand );

	//	成りならば駒を裏返す
	if ( move.promote )
		p += PROMOTE;

	//	移動先に駒を置く
	Put( move.to, p );

	//	手番交代
	Turn ^= 1;

	Hash ^= HashTurn;

	Step++;

	//	Log
	LOG log;
	log.move = move;
	log.remove = rp;
	log.value = value;
	Log.push_back( log );
}


/*
 *	MoveNull
 *		何もせずに手番を反転
 */
void CBoard::MoveNull()
{
	int value = Evaluator.GetValue( this );

	//	手番交代
	Turn ^= 1;

	Hash ^= HashTurn;

	Step++;

	//	Log
	LOG log;
	log.move = MOVE::null;
	log.remove = 0;
	log.value = value;
	Log.push_back( log );

}



/*
 *	Undo
 *		一手戻す
 */
void CBoard::Undo()
{
	//	Log
	LOG log = Log.back();
	Log.pop_back();
	MOVE move = log.move;

	//	手番交代
	Turn ^= 1;

	Hash ^= HashTurn;

	Step--;

	//	NullMoveだったならば終了
	if ( move == MOVE::null )
		return;

	//	移動先から駒をとる
	int p = Board[move.to];
	RemoveNoValue( move.to );

	//	成りならば駒を裏返す
	if ( move.promote )
		p -= PROMOTE;

	//	移動元に駒を置く
	if ( move.hand == 0 )
		PutNoValue( move.from, p );
	else
		PutHandNoValue( p );

	//	移動先にあった駒を戻す
	int r = log.remove;
	if ( r != SPACE )
	{
		RemoveHandNoValue( ( r < TO ? r : r - PROMOTE ) ^ 1 );
		Put( move.to, r );
	}

	//	評価値を元に戻す
	Evaluator.SetValue( log.value );
}



/*
 *	LogClear
 *		Logを削除
 */
void CBoard::LogClear()
{
	Log.clear();
}



/*
 *	GetPiece
 *		posの位置の駒を取得
 */
int CBoard::GetPiece( int pos ) const
{
	return Board[pos];
}

int CBoard::GetPiece( int x, int y ) const
{
	return GetPiece( y * 7 + x );
}



/*
 *	GetHand
 *		持ち駒の枚数を返す
 */
int CBoard::GetHand( int hand ) const
{
	return Hand[hand];
}



/*
 *	GetTurn(Sign)
 *		手番を取得
 *
 *	(先,後) = ( 0, 1 )		GetTurn
 *			= ( 1, -1 )		GetTurnSign
 */
int CBoard::GetTurn() const
{
	return Turn;
}

int CBoard::GetTurnSign() const
{
	return 1 - 2 * Turn;
}



/*
 *	GetStep
 *		手数を取得
 */
int CBoard::GetStep() const
{
	return Step;
}



/*
 *	IsFinished
 *		試合が終了しているか
 */
bool CBoard::IsFinished() const
{
	return
		Hand[GYOKU] > 0  ||						//	先手勝ち
		Hand[GYOKU+1] > 0  ||					//	後手勝ち
		Turn == 0  &&  IsCheckedGyoku(1)  ||	//	先手勝ち
		Turn == 1  &&  IsCheckedGyoku(0);		//	後手勝ち
}



/*
 *	IsEffected
 *		posの位置にturnの駒が効いているか
 *		far==trueなら飛駒のみを考慮
 */
bool CBoard::IsEffected( int pos, int turn, bool far/*=false*/ ) const
{
	if ( far )
		return ( Effect[pos][turn] & 0xff ) != 0;
	else
		return Effect[pos][turn] != 0;
}



/*
 *	IsCheckmated
 *		turn（0:先手、1:後手）の王が詰んでいるか
 */
bool CBoard::IsCheckmated( int turn ) const
{
	int	g = GyokuPosition[turn];

	//	玉の位置に敵の駒が効いている
	if ( Effect[g][turn^1] == 0 )
		return false;

	//	玉の移動可能な場所が無い
	int *near = PieceMoveNear[GYOKU^turn];
	for ( int i=0; near[i]!=0; i++ )
	{
		int n = g + near[i];
		if ( ( Board[n] == SPACE  ||  ( Board[n]>=FU && (Board[n]&1^turn)==1 ) )  &&
			 Effect[n][turn^1] == 0 )
			return false;
	}

	//	玉の位置に効いてる駒について
	const static int efnear[12] = { -8, -7, -6, -1, +1, +6, +7, +8 };

	for ( int i=0, is=1<<8; i<8; i++, is<<=1 )
	if ( ( Effect[g][turn^1] & is ) != 0 )
	{
		int e = g + efnear[i];

		//	その効いている敵駒に玉以外の効きが無い
		if ( ( Effect[e][turn] & ~PieceMoveToEffectNear[-efnear[i]] ) != 0 )
			return false;
	}

	const static int effar[8] = { -8, -7, -6, -1, +1, +6, +7, +8 };

	for ( int i=0, is=1; i<8; i++, is<<=1 )
	if ( ( Effect[g][turn^1] & is ) != 0 )
	{
		int e = g + efnear[i];

		//	１マス隣に敵駒があり、その効いている敵駒に玉以外の効きが無い
		//	２マス以上の効きはとりあえず無視
		if ( Board[e] == SPACE  ||
			 ( Effect[e][turn] & ~PieceMoveToEffectNear[-efnear[i]] ) != 0 )
			return false;
	}

	return true;
}



/*
 *	IsCheckedGyoku
 *		turn（0:先手、1:後手）の王がチェックされているか
 */
bool CBoard::IsCheckedGyoku( int turn ) const
{
	return IsEffected( GyokuPosition[turn], turn^1 );
}



/*
 *	GetValue
 *		評価を取得
 */
int CBoard::GetValue() const
{
	if ( Hand[GYOKU] > 0 )
		return MATE - Step;
	if ( Hand[GYOKU+1] > 0 )
		return -MATE + Step;
	if ( Turn == 0  &&  IsCheckedGyoku(1) )
		return MATE - Step;
	if ( Turn == 1  &&  IsCheckedGyoku(0) )
		return -MATE + Step;

	return Evaluator.GetValue( this );
}



/*
 *	GetHash
 *		盤面ハッシュを取得
 */
HASH CBoard::GetHash() const
{
	return Hash;
}



/*
 *	SetWeight
 *		評価関数の重みを設定
 */
void CBoard::SetWeight( const int weight[ELEMNUM] )
{
	Evaluator.SetWeight( this, weight );
}



/*
 *	GetElement
 *		評価関数の算出に使う要素を取得
 */
void CBoard::GetElement( int element[ELEMNUM] )
{
	return Evaluator.GetElement( this, element );
}



/*
 *	Display
 *		表示
 */
void CBoard::Display() const
{
	const char *piece[] = {
		" ・ ", " 壁 ", " 歩A", "V歩 ", " 銀A", "V銀 ", " 角A", "V角 ", " 飛A", "V飛 ",
		" 金A", "V金 ", " 王A", "V玉 ", " とA", "Vと ", " 全A", "V全 ", " 馬A", "V馬 ",
		" 龍A", "V龍 ", };

	printf( "┌―――┬――――%s――――┬―――┐\n", Turn==1?"━━":"――" );

	for ( int y=1; y<=5; y++ )
	{
		//	敵駒台
		if ( Hand[y*2+1] == 0 )
			printf( "│　　　│" );
		else
			printf( "│%s%2d│", piece[y*2+1], Hand[y*2+1] );

		//	盤
		for ( int x=1; x<=5; x++ )
			printf( "%s", piece[ Board[y*7+6-x] ] );

		//	自駒台
		if ( Hand[12-y*2] == 0 )
			printf( "│　　　│\n" );
		else
			printf( "│%s%2d│\n", piece[12-y*2], Hand[12-y*2] );
	}

	printf( "└―――┴――――%s――――┴―――┘\n", Turn==0?"━━":"――" );

	//printf( "Value: %8d\n", Evaluator.GetValue(this) );
	//printf( "String: %s\n", ToString().c_str() );
}



/*
 *	DisplayEffect
 *		効きを表示
 */
void CBoard::DisplayEffect() const
{
	for ( int y=0; y<7; y++ )
	{
		for ( int x=0; x<7; x++ )
		{
			const char *c = " ・ ";

			int i = y*7 + 6-x;

			if ( IsEffected( i, 0 )  &&  IsEffected( i, 1 ) )
				c = " Ｘ ";
			else if ( IsEffected( i, 0 ) )
				c = " ∧ ";
			else if ( IsEffected( i, 1 ) )
				c = " ∨ ";

			printf( "%s", c );
		}
		printf( "\n" );
	}

	printf( "\n" );
}



/*
 *	Update
 *		Board, Hand, Turnに合わせて
 *		Fu, Hashなどを更新
 */
void CBoard::Update()
{
	Evaluator.Update( this );

	//	Log
	Log.clear();

	//	Hash
	Hash = 0;

	for ( int i=0; i<49; i++ )
		Hash ^= HashBoard[i][ Board[i] ];

	for ( int i=0; i<14; i++ )
		for ( int j=0; j<=Hand[i]; j++ )
			Hash ^= HashHand[i][j];

	if ( Turn == 1 )
		Hash ^= HashTurn;

	//	Fu
	for ( int i=0; i<7; i++ )
		Fu[i][0] = Fu[i][1] = false;

	for ( int y=0; y<7; y++ )
	for ( int x=0; x<7; x++ )
	{
		if ( Board[y*7+x] == FU )
			Fu[0][x] = true;
		if ( Board[y*7+x] == FU + 1 )
			Fu[1][x] = true;
	}

	//	GyokuPosition
	for ( int i=0; i<49; i++ )
		if ( ( Board[i] & ~1 ) == GYOKU )
			GyokuPosition[Board[i]&1] = i;

	//	Effect
	for ( int i=0; i<49; i++ )
		Effect[i][0] = Effect[i][1] = 0;

	for ( int i=0; i<49; i++ )
	{
		int p = Board[i];
		int *near = PieceMoveNear[p];
		int *far = PieceMoveFar[p];

		for ( int j=0; near[j]!=0; j++ )
			Effect[i+near[j]][p&1] |= PieceMoveToEffectNear[-near[j]];

		for ( int j=0; far[j]!=0; j++ )
		{
			for ( int k=i+far[j]; ; k+=far[j] )
			{
				Effect[k][p&1] |= PieceMoveToEffectFar[-far[j]];
				if ( Board[k] != SPACE  &&
					 Board[k] != (GYOKU^p&1^1) )	//	敵玉は突き抜ける
					break;
			}
		}
	}
}



/*
 *	CheckMoveCore
 *		着手可能か判定
 *		移動元・移動先の範囲、移動元に駒があるか、動き方、駒打ちでの成り、成れない駒の成りを除く
 */
bool CBoard::CheckMoveCore( MOVE move, int flag ) const
{
	int		fp;		//	移動する駒
	int		tp;		//	移動先の駒

	//	成りの場合
	if ( move.promote )
	{
		//	移動元も移動先も敵陣ではない
		if ( ! ( Turn==0 ? move.from < 14 : move.from >= 35 )  &&
			 ! ( Turn==0 ? move.to < 14 : move.to >= 35 ) )
			return false;
	}

	//	移動元、移動先の駒を取得
	if ( move.hand == 0 )
		fp = Board[move.from];
	else
		fp = move.hand;

	tp = Board[move.to];

	//	移動先に味方の駒・壁
	if ( IsFriend( tp )  ||  tp == WALL )
		return false;

	//	MV_CAPTUREで移動先に駒が無い
	if ( flag & MV_CAPTURE )
	{
		if ( tp == SPACE )
			return false;
	}

	//	駒打ちで既に駒がある
	if ( move.hand != 0  &&  tp != SPACE )
		return false;

	//	二歩
	if ( ( move.hand & ~1 ) == FU  &&
		 Fu[Turn][move.to%7] )
		return false;

	//	行き所の無い駒
	if ( ! move.promote )
	{
		if ( fp == FU  &&  move.to < 14 )		//	１段目
			return false;
		if ( fp == FU+1  &&  move.to >= 35 )	//	１段目
			return false;
	}

	//	TODO 打ち歩詰め

	return true;
}



/*
 *	Put / PutHand / Remove / RemoveHand
 *		指定された駒を置く/取り除く
 *		NoValueは評価値の更新を行わない
 */
void CBoard::Put( int pos, int piece )
{
	PutNoValue( pos, piece );

	//	評価
	Evaluator.Put( this, pos, piece );
}

void CBoard::PutNoValue( int pos, int piece )
{
	Board[pos] = piece;

	//	ハッシュ
	Hash ^= HashBoard[pos][piece];

	//	歩
	if ( ( piece & ~1 ) == FU )
		Fu[piece&1][pos%11] = true;

	//	玉
	if ( ( piece & ~1 ) == GYOKU )
		GyokuPosition[piece&1] = pos;

	//	効き
	int *near = PieceMoveNear[piece];
	int *far = PieceMoveFar[piece];

	for ( int i=0; near[i]!=0; i++ )
		Effect[pos+near[i]][piece&1] |= PieceMoveToEffectNear[-near[i]];

	//	飛駒の効きを遮る
	const static int effect[] = { -8, -7, -6, -1, +1, +6, +7, +8 };

	if ( ( Effect[pos][0] & 0xff ) != 0  &&
		 piece != GYOKU+1 )					//	玉は敵の利きを遮らない
	{
		for ( int i=0, is=1; i<8; i++, is<<=1 )
		if ( ( Effect[pos][0] & is ) != 0 )
			for ( int j=pos-effect[i]; ; j-=effect[i] )
			{
				Effect[j][0] &= ~is;
				if ( Board[j] != SPACE  &&
					 Board[j] != GYOKU+1 )  //	玉は敵の利きを遮らない
					break;
			}
	}

	if ( ( Effect[pos][1] & 0xff ) != 0  &&
		 piece != GYOKU )					//	玉は敵の利きを遮らない
	{
		for ( int i=0, is=1; i<8; i++, is<<=1 )
		if ( ( Effect[pos][1] & is ) != 0 )
			for ( int j=pos-effect[i]; ; j-=effect[i] )
			{
				Effect[j][1] &= ~is;
				if ( Board[j] != SPACE  &&
					 Board[j] != GYOKU )  //	玉は敵の利きを遮らない
					break;
			}
	}

	//	置いた駒の効き
	for ( int i=0; far[i]!=0; i++ )
	{
		for ( int j=pos+far[i]; ; j+=far[i] )
		{
			Effect[j][piece&1] |= PieceMoveToEffectFar[-far[i]];
			if ( Board[j] != SPACE  &&
				 Board[j] != (GYOKU^piece&1^1) )  //	玉は敵の利きを遮らない
				break;
		}
	}
}

void CBoard::PutHand( int piece )
{
	PutHandNoValue( piece );

	//	評価
	Evaluator.PutHand( this, piece );
}

void CBoard::PutHandNoValue( int piece )
{
	Hand[piece]++;

	//	ハッシュ
	Hash ^= HashHand[piece][ Hand[piece] ];
}

void CBoard::Remove( int pos )
{
	//	評価
	Evaluator.Remove( this, pos );

	RemoveNoValue( pos );
}

void CBoard::RemoveNoValue( int pos )
{
	int piece = Board[pos];

	//	ハッシュ
	Hash ^= HashBoard[pos][piece];

	//	歩
	if ( ( piece & ~1 ) == FU )
		Fu[piece&1][pos%11] = false;

	//	効き
	int *near = PieceMoveNear[piece];
	int *far = PieceMoveFar[piece];

	for ( int i=0; near[i]!=0; i++ )
		Effect[pos+near[i]][piece&1] &= ~PieceMoveToEffectNear[-near[i]];

	//	取り除く駒の効き
	for ( int i=0; far[i]!=0; i++ )
	{
		for ( int j=pos+far[i]; ; j+=far[i] )
		{
			Effect[j][piece&1] &= ~PieceMoveToEffectFar[-far[i]];
			if ( Board[j] != SPACE  &&
				 Board[j] != (GYOKU^piece&1^1) )  //	玉は敵の利きを遮らない
				break;
		}
	}

	//	遮っていた効きを復活
	const static int effect[] = { -8, -7, -6, -1, +1, +6, +7, +8 };

	if ( ( Effect[pos][0] & 0xff ) != 0  &&
		 piece != GYOKU+1 )					//	玉は敵の利きを遮らない
	{
		for ( int i=0, is=1; i<8; i++, is<<=1 )
		if ( ( Effect[pos][0] & is ) != 0 )
			for ( int j=pos-effect[i]; ; j-=effect[i] )
			{
				Effect[j][0] |= is;
				if ( Board[j] != SPACE  &&
					 Board[j] != GYOKU+1 )  //	玉は敵の利きを遮らない
					break;
			}
	}

	if ( ( Effect[pos][1] & 0xff ) != 0  &&
		 piece != GYOKU )					//	玉は敵の利きを遮らない
	{
		for ( int i=0, is=1; i<8; i++, is<<=1 )
		if ( ( Effect[pos][1] & is ) != 0 )
			for ( int j=pos-effect[i]; ; j-=effect[i] )
			{
				Effect[j][1] |= is;
				if ( Board[j] != SPACE  &&
					 Board[j] != GYOKU )  //	玉は敵の利きを遮らない
					break;
			}
	}

	Board[pos] = SPACE;
}

void CBoard::RemoveHand( int piece )
{
	//	評価
	Evaluator.RemoveHand( this, piece );

	RemoveHandNoValue( piece );
}

void CBoard::RemoveHandNoValue( int piece )
{
	//	ハッシュ
	Hash ^= HashHand[piece][ Hand[piece] ];

	Hand[piece]--;
}



/*
 *	IsFriend/Enemy
 *		pが味方/敵かどうか判定
 */
bool CBoard::IsFriend( int p ) const
{
	return ( p & ~1 ) != 0  &&  ( p&1 ^ Turn ^ 1 ) != 0;
}

bool CBoard::IsEnemy( int p ) const
{
	return ( p & ~1 ) != 0  &&  ( p&1 ^ Turn ) != 0;
}
