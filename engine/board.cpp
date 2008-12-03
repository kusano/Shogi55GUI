
#include	"board.h"
#include	"config.h"
#include	"mt19937ar.h"



/*
 *	��̓���
 */
int CBoard::PieceMoveNear[30][16] = {
	{                            }, {},	//	��
	{     -7,                    }, {},	//	��
	{ -8, -7, -6,        6,    8 }, {},	//	��
	{                            }, {},	//	�p
	{                            }, {},	//	��
	{ -8, -7, -6, -1, 1,    7,   }, {},	//	��
	{ -8, -7, -6, -1, 1, 6, 7, 8 }, {},	//	��
	{ -8, -7, -6, -1, 1,    7,   }, {},	//	��
	{ -8, -7, -6, -1, 1,    7,   }, {},	//	�S
	{     -7,     -1, 1,    7,   }, {},	//	�n
	{ -8,     -6,        6,    8 }, {},	//	��
};

int CBoard::PieceMoveFar[30][16] = {
	{                            }, {},	//	��
	{                            }, {},	//	��
	{                            }, {},	//	��
	{ -8,     -6,        6,    8 }, {},	//	�p
	{     -7,     -1, 1,    7,   }, {},	//	��
	{                            }, {},	//	��
	{                            }, {},	//	��
	{                            }, {},	//	��
	{                            }, {},	//	�S
	{ -8,     -6,        6,    8 }, {},	//	�n
	{     -7,     -1, 1,    7,   }, {},	//	��
};

int *CBoard::PieceMoveToEffectNear;
int CBoard::PieceMoveToEffectNearTemp[21];
int *CBoard::PieceMoveToEffectFar;
int CBoard::PieceMoveToEffectFarTemp[21];



/*
 *	�n�b�V���p����
 */
HASH CBoard::HashBoard[49][32];
HASH CBoard::HashHand[14][32];
HASH CBoard::HashTurn;



/*
 *	Initialize
 *		������
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
 *		�������i�S�́j
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
		for ( int j=2; j<ELEMNUM(HashBoard[i]); j++ )		//	��E�ǂ�0
			HashBoard[i][j] = (HASH)genrand_int32()<<32 | genrand_int32();

	//	HashHand
	for ( int i=2; i<ELEMNUM(HashHand); i++ )				//	��E�ǂ�0
		for ( int j=1; j<ELEMNUM(HashHand[i]); j++ )		//	�������0
			HashHand[i][j] = (HASH)genrand_int32()<<32 | genrand_int32();

	//	HashTurn
	HashTurn = (HASH)genrand_int32()<<32 | genrand_int32();

	initialized = true;

	#undef ELEMNUM
}



/*
 *	To/FromString
 *		�Ֆʂ𕶎���ɕϊ��E�����񂩂�ǂݍ���
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
 *		����\����擾
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

		//	�P�}�X
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

		//	���
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

	//	��ł�
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
 *		move������\���ǂ�������
 */
bool CBoard::CheckMove( MOVE move, int flag/*=0*/ ) const
{
	//	null�͏�ɖ���
	if ( move == MOVE::null )
		return false;

	//	�ړ����ړ���͈̔�
	if ( move.from < 0  ||  49 <= move.from  ||
		 move.to < 0  ||  49 <= move.to )
		return false;

	//	��łňړ�����0��
	if ( move.hand  &&  move.from != 0 )
		return false;

	int fp = Board[move.from];
	int tp = Board[move.to];

	//	�ړ����ɋ���邩
	if ( move.hand == 0  &&  ! IsFriend(fp) )
		return false;
	if ( move.hand != 0  &&  Hand[move.hand] == 0 )
		return false;

	//	�ړ���Ɉړ��ł��邩
	if ( move.hand == 0  &&  tp != SPACE  &&  ! IsEnemy(tp)  ||
		 move.hand != 0  &&  tp != SPACE )
		return false;

	//	�������͐�������
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

	//	����
	if ( move.promote )
	{
		if ( move.hand != 0 )
			return false;

		if ( fp >= KIN )
			return false;

		//	�G�w���ǂ�����core�Ŕ���
	}

	return CheckMoveCore( move, flag );
}


/*
 *	Move
 *		����w��
 */
void CBoard::Move( MOVE move )
{
	int value = Evaluator.GetValue( this );

	//	�ړ���ɋ����΋���
	int rp = Board[move.to];
	if ( rp != SPACE )
		Remove( move.to ),
		PutHand( ( rp<TO ? rp : rp-PROMOTE ) ^ 1 );

	//	�ړ������������
	int p;
	if ( move.hand == 0 )
		p = Board[move.from],
		Remove( move.from );
	else
		p = move.hand,
		RemoveHand( move.hand );

	//	����Ȃ�΋�𗠕Ԃ�
	if ( move.promote )
		p += PROMOTE;

	//	�ړ���ɋ��u��
	Put( move.to, p );

	//	��Ԍ��
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
 *		���������Ɏ�Ԃ𔽓]
 */
void CBoard::MoveNull()
{
	int value = Evaluator.GetValue( this );

	//	��Ԍ��
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
 *		���߂�
 */
void CBoard::Undo()
{
	//	Log
	LOG log = Log.back();
	Log.pop_back();
	MOVE move = log.move;

	//	��Ԍ��
	Turn ^= 1;

	Hash ^= HashTurn;

	Step--;

	//	NullMove�������Ȃ�ΏI��
	if ( move == MOVE::null )
		return;

	//	�ړ��悩�����Ƃ�
	int p = Board[move.to];
	RemoveNoValue( move.to );

	//	����Ȃ�΋�𗠕Ԃ�
	if ( move.promote )
		p -= PROMOTE;

	//	�ړ����ɋ��u��
	if ( move.hand == 0 )
		PutNoValue( move.from, p );
	else
		PutHandNoValue( p );

	//	�ړ���ɂ��������߂�
	int r = log.remove;
	if ( r != SPACE )
	{
		RemoveHandNoValue( ( r < TO ? r : r - PROMOTE ) ^ 1 );
		Put( move.to, r );
	}

	//	�]���l�����ɖ߂�
	Evaluator.SetValue( log.value );
}



/*
 *	LogClear
 *		Log���폜
 */
void CBoard::LogClear()
{
	Log.clear();
}



/*
 *	GetPiece
 *		pos�̈ʒu�̋���擾
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
 *		������̖�����Ԃ�
 */
int CBoard::GetHand( int hand ) const
{
	return Hand[hand];
}



/*
 *	GetTurn(Sign)
 *		��Ԃ��擾
 *
 *	(��,��) = ( 0, 1 )		GetTurn
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
 *		�萔���擾
 */
int CBoard::GetStep() const
{
	return Step;
}



/*
 *	IsFinished
 *		�������I�����Ă��邩
 */
bool CBoard::IsFinished() const
{
	return
		Hand[GYOKU] > 0  ||						//	��菟��
		Hand[GYOKU+1] > 0  ||					//	��菟��
		Turn == 0  &&  IsCheckedGyoku(1)  ||	//	��菟��
		Turn == 1  &&  IsCheckedGyoku(0);		//	��菟��
}



/*
 *	IsEffected
 *		pos�̈ʒu��turn�̋�����Ă��邩
 *		far==true�Ȃ���݂̂��l��
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
 *		turn�i0:���A1:���j�̉����l��ł��邩
 */
bool CBoard::IsCheckmated( int turn ) const
{
	int	g = GyokuPosition[turn];

	//	�ʂ̈ʒu�ɓG�̋�����Ă���
	if ( Effect[g][turn^1] == 0 )
		return false;

	//	�ʂ̈ړ��\�ȏꏊ������
	int *near = PieceMoveNear[GYOKU^turn];
	for ( int i=0; near[i]!=0; i++ )
	{
		int n = g + near[i];
		if ( ( Board[n] == SPACE  ||  ( Board[n]>=FU && (Board[n]&1^turn)==1 ) )  &&
			 Effect[n][turn^1] == 0 )
			return false;
	}

	//	�ʂ̈ʒu�Ɍ����Ă��ɂ���
	const static int efnear[12] = { -8, -7, -6, -1, +1, +6, +7, +8 };

	for ( int i=0, is=1<<8; i<8; i++, is<<=1 )
	if ( ( Effect[g][turn^1] & is ) != 0 )
	{
		int e = g + efnear[i];

		//	���̌����Ă���G��ɋʈȊO�̌���������
		if ( ( Effect[e][turn] & ~PieceMoveToEffectNear[-efnear[i]] ) != 0 )
			return false;
	}

	const static int effar[8] = { -8, -7, -6, -1, +1, +6, +7, +8 };

	for ( int i=0, is=1; i<8; i++, is<<=1 )
	if ( ( Effect[g][turn^1] & is ) != 0 )
	{
		int e = g + efnear[i];

		//	�P�}�X�ׂɓG�����A���̌����Ă���G��ɋʈȊO�̌���������
		//	�Q�}�X�ȏ�̌����͂Ƃ肠��������
		if ( Board[e] == SPACE  ||
			 ( Effect[e][turn] & ~PieceMoveToEffectNear[-efnear[i]] ) != 0 )
			return false;
	}

	return true;
}



/*
 *	IsCheckedGyoku
 *		turn�i0:���A1:���j�̉����`�F�b�N����Ă��邩
 */
bool CBoard::IsCheckedGyoku( int turn ) const
{
	return IsEffected( GyokuPosition[turn], turn^1 );
}



/*
 *	GetValue
 *		�]�����擾
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
 *		�Ֆʃn�b�V�����擾
 */
HASH CBoard::GetHash() const
{
	return Hash;
}



/*
 *	SetWeight
 *		�]���֐��̏d�݂�ݒ�
 */
void CBoard::SetWeight( const int weight[ELEMNUM] )
{
	Evaluator.SetWeight( this, weight );
}



/*
 *	GetElement
 *		�]���֐��̎Z�o�Ɏg���v�f���擾
 */
void CBoard::GetElement( int element[ELEMNUM] )
{
	return Evaluator.GetElement( this, element );
}



/*
 *	Display
 *		�\��
 */
void CBoard::Display() const
{
	const char *piece[] = {
		" �E ", " �� ", " ��A", "V�� ", " ��A", "V�� ", " �pA", "V�p ", " ��A", "V�� ",
		" ��A", "V�� ", " ��A", "V�� ", " ��A", "V�� ", " �SA", "V�S ", " �nA", "V�n ",
		" ��A", "V�� ", };

	printf( "���\�\�\���\�\�\�\%s�\�\�\�\���\�\�\��\n", Turn==1?"����":"�\�\" );

	for ( int y=1; y<=5; y++ )
	{
		//	�G���
		if ( Hand[y*2+1] == 0 )
			printf( "���@�@�@��" );
		else
			printf( "��%s%2d��", piece[y*2+1], Hand[y*2+1] );

		//	��
		for ( int x=1; x<=5; x++ )
			printf( "%s", piece[ Board[y*7+6-x] ] );

		//	�����
		if ( Hand[12-y*2] == 0 )
			printf( "���@�@�@��\n" );
		else
			printf( "��%s%2d��\n", piece[12-y*2], Hand[12-y*2] );
	}

	printf( "���\�\�\���\�\�\�\%s�\�\�\�\���\�\�\��\n", Turn==0?"����":"�\�\" );

	//printf( "Value: %8d\n", Evaluator.GetValue(this) );
	//printf( "String: %s\n", ToString().c_str() );
}



/*
 *	DisplayEffect
 *		������\��
 */
void CBoard::DisplayEffect() const
{
	for ( int y=0; y<7; y++ )
	{
		for ( int x=0; x<7; x++ )
		{
			const char *c = " �E ";

			int i = y*7 + 6-x;

			if ( IsEffected( i, 0 )  &&  IsEffected( i, 1 ) )
				c = " �w ";
			else if ( IsEffected( i, 0 ) )
				c = " �� ";
			else if ( IsEffected( i, 1 ) )
				c = " �� ";

			printf( "%s", c );
		}
		printf( "\n" );
	}

	printf( "\n" );
}



/*
 *	Update
 *		Board, Hand, Turn�ɍ��킹��
 *		Fu, Hash�Ȃǂ��X�V
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
					 Board[k] != (GYOKU^p&1^1) )	//	�G�ʂ͓˂�������
					break;
			}
		}
	}
}



/*
 *	CheckMoveCore
 *		����\������
 *		�ړ����E�ړ���͈̔́A�ړ����ɋ���邩�A�������A��ł��ł̐���A����Ȃ���̐��������
 */
bool CBoard::CheckMoveCore( MOVE move, int flag ) const
{
	int		fp;		//	�ړ������
	int		tp;		//	�ړ���̋�

	//	����̏ꍇ
	if ( move.promote )
	{
		//	�ړ������ړ�����G�w�ł͂Ȃ�
		if ( ! ( Turn==0 ? move.from < 14 : move.from >= 35 )  &&
			 ! ( Turn==0 ? move.to < 14 : move.to >= 35 ) )
			return false;
	}

	//	�ړ����A�ړ���̋���擾
	if ( move.hand == 0 )
		fp = Board[move.from];
	else
		fp = move.hand;

	tp = Board[move.to];

	//	�ړ���ɖ����̋�E��
	if ( IsFriend( tp )  ||  tp == WALL )
		return false;

	//	MV_CAPTURE�ňړ���ɋ����
	if ( flag & MV_CAPTURE )
	{
		if ( tp == SPACE )
			return false;
	}

	//	��ł��Ŋ��ɋ����
	if ( move.hand != 0  &&  tp != SPACE )
		return false;

	//	���
	if ( ( move.hand & ~1 ) == FU  &&
		 Fu[Turn][move.to%7] )
		return false;

	//	�s�����̖�����
	if ( ! move.promote )
	{
		if ( fp == FU  &&  move.to < 14 )		//	�P�i��
			return false;
		if ( fp == FU+1  &&  move.to >= 35 )	//	�P�i��
			return false;
	}

	//	TODO �ł����l��

	return true;
}



/*
 *	Put / PutHand / Remove / RemoveHand
 *		�w�肳�ꂽ���u��/��菜��
 *		NoValue�͕]���l�̍X�V���s��Ȃ�
 */
void CBoard::Put( int pos, int piece )
{
	PutNoValue( pos, piece );

	//	�]��
	Evaluator.Put( this, pos, piece );
}

void CBoard::PutNoValue( int pos, int piece )
{
	Board[pos] = piece;

	//	�n�b�V��
	Hash ^= HashBoard[pos][piece];

	//	��
	if ( ( piece & ~1 ) == FU )
		Fu[piece&1][pos%11] = true;

	//	��
	if ( ( piece & ~1 ) == GYOKU )
		GyokuPosition[piece&1] = pos;

	//	����
	int *near = PieceMoveNear[piece];
	int *far = PieceMoveFar[piece];

	for ( int i=0; near[i]!=0; i++ )
		Effect[pos+near[i]][piece&1] |= PieceMoveToEffectNear[-near[i]];

	//	���̌������Ղ�
	const static int effect[] = { -8, -7, -6, -1, +1, +6, +7, +8 };

	if ( ( Effect[pos][0] & 0xff ) != 0  &&
		 piece != GYOKU+1 )					//	�ʂ͓G�̗������Ղ�Ȃ�
	{
		for ( int i=0, is=1; i<8; i++, is<<=1 )
		if ( ( Effect[pos][0] & is ) != 0 )
			for ( int j=pos-effect[i]; ; j-=effect[i] )
			{
				Effect[j][0] &= ~is;
				if ( Board[j] != SPACE  &&
					 Board[j] != GYOKU+1 )  //	�ʂ͓G�̗������Ղ�Ȃ�
					break;
			}
	}

	if ( ( Effect[pos][1] & 0xff ) != 0  &&
		 piece != GYOKU )					//	�ʂ͓G�̗������Ղ�Ȃ�
	{
		for ( int i=0, is=1; i<8; i++, is<<=1 )
		if ( ( Effect[pos][1] & is ) != 0 )
			for ( int j=pos-effect[i]; ; j-=effect[i] )
			{
				Effect[j][1] &= ~is;
				if ( Board[j] != SPACE  &&
					 Board[j] != GYOKU )  //	�ʂ͓G�̗������Ղ�Ȃ�
					break;
			}
	}

	//	�u������̌���
	for ( int i=0; far[i]!=0; i++ )
	{
		for ( int j=pos+far[i]; ; j+=far[i] )
		{
			Effect[j][piece&1] |= PieceMoveToEffectFar[-far[i]];
			if ( Board[j] != SPACE  &&
				 Board[j] != (GYOKU^piece&1^1) )  //	�ʂ͓G�̗������Ղ�Ȃ�
				break;
		}
	}
}

void CBoard::PutHand( int piece )
{
	PutHandNoValue( piece );

	//	�]��
	Evaluator.PutHand( this, piece );
}

void CBoard::PutHandNoValue( int piece )
{
	Hand[piece]++;

	//	�n�b�V��
	Hash ^= HashHand[piece][ Hand[piece] ];
}

void CBoard::Remove( int pos )
{
	//	�]��
	Evaluator.Remove( this, pos );

	RemoveNoValue( pos );
}

void CBoard::RemoveNoValue( int pos )
{
	int piece = Board[pos];

	//	�n�b�V��
	Hash ^= HashBoard[pos][piece];

	//	��
	if ( ( piece & ~1 ) == FU )
		Fu[piece&1][pos%11] = false;

	//	����
	int *near = PieceMoveNear[piece];
	int *far = PieceMoveFar[piece];

	for ( int i=0; near[i]!=0; i++ )
		Effect[pos+near[i]][piece&1] &= ~PieceMoveToEffectNear[-near[i]];

	//	��菜����̌���
	for ( int i=0; far[i]!=0; i++ )
	{
		for ( int j=pos+far[i]; ; j+=far[i] )
		{
			Effect[j][piece&1] &= ~PieceMoveToEffectFar[-far[i]];
			if ( Board[j] != SPACE  &&
				 Board[j] != (GYOKU^piece&1^1) )  //	�ʂ͓G�̗������Ղ�Ȃ�
				break;
		}
	}

	//	�Ղ��Ă��������𕜊�
	const static int effect[] = { -8, -7, -6, -1, +1, +6, +7, +8 };

	if ( ( Effect[pos][0] & 0xff ) != 0  &&
		 piece != GYOKU+1 )					//	�ʂ͓G�̗������Ղ�Ȃ�
	{
		for ( int i=0, is=1; i<8; i++, is<<=1 )
		if ( ( Effect[pos][0] & is ) != 0 )
			for ( int j=pos-effect[i]; ; j-=effect[i] )
			{
				Effect[j][0] |= is;
				if ( Board[j] != SPACE  &&
					 Board[j] != GYOKU+1 )  //	�ʂ͓G�̗������Ղ�Ȃ�
					break;
			}
	}

	if ( ( Effect[pos][1] & 0xff ) != 0  &&
		 piece != GYOKU )					//	�ʂ͓G�̗������Ղ�Ȃ�
	{
		for ( int i=0, is=1; i<8; i++, is<<=1 )
		if ( ( Effect[pos][1] & is ) != 0 )
			for ( int j=pos-effect[i]; ; j-=effect[i] )
			{
				Effect[j][1] |= is;
				if ( Board[j] != SPACE  &&
					 Board[j] != GYOKU )  //	�ʂ͓G�̗������Ղ�Ȃ�
					break;
			}
	}

	Board[pos] = SPACE;
}

void CBoard::RemoveHand( int piece )
{
	//	�]��
	Evaluator.RemoveHand( this, piece );

	RemoveHandNoValue( piece );
}

void CBoard::RemoveHandNoValue( int piece )
{
	//	�n�b�V��
	Hash ^= HashHand[piece][ Hand[piece] ];

	Hand[piece]--;
}



/*
 *	IsFriend/Enemy
 *		p������/�G���ǂ�������
 */
bool CBoard::IsFriend( int p ) const
{
	return ( p & ~1 ) != 0  &&  ( p&1 ^ Turn ^ 1 ) != 0;
}

bool CBoard::IsEnemy( int p ) const
{
	return ( p & ~1 ) != 0  &&  ( p&1 ^ Turn ) != 0;
}
