
#include	"minmaxbot.h"
#include	"mt19937ar.h"
#include	<algorithm>



using namespace std;



//	局面表
CMinMaxBot::POSITION CMinMaxBot::Position[CMinMaxBot::POSSIZE];
CMinMaxBot::POSITION CMinMaxBot::QPosition[CMinMaxBot::QPOSSIZE];

//	探索状態
STATE CMinMaxBot::State;



/*
 *	CMinMaxBot
 *		コンストラクタ
 */
CMinMaxBot::CMinMaxBot() :
	MaxDepth( 99 ),
	TimeLimit( 999000 ),
	DisplayMode( DM_NONE )
{
	//	State::tree.child初期化
	for ( int i=0; i<sizeof State.tree/sizeof State.tree[0]; i++ )
		for ( int j=0; j<4; j++ )
			State.tree[i].child[j] = -1;
		
	int cur = 1;
	//	ルート
	for ( int j=0; j<5; j++ )
		State.tree[0].child[j] = cur++;
	State.tree[0].childnummax = 5;

	//	深さ１
	for ( int i=1; i<6; i++ )
	{
		for ( int j=0; j<4; j++ )
			State.tree[i].child[j] = cur++;
		State.tree[i].childnummax = 4;
	}
	//	深さ２
	for ( int i=6; i<26; i++ )
	{
		for ( int j=0; j<3; j++ )
			State.tree[i].child[j] = cur++;
		State.tree[i].childnummax = 3;
	}
	//	深さ３
	for ( int i=26; i<86; i++ )
	{
		for ( int j=0; j<2; j++ )
			State.tree[i].child[j] = cur++;
		State.tree[i].childnummax = 2;
	}
	//	深さ４
	for ( int i=86; i<206; i++ )
	{
		State.tree[i].childnummax = 0;
	}
}



/*
 *	Initialize
 *		KillerMove, PositionTableをクリア
 */
void CMinMaxBot::Initialize()
{
	for ( int i=0; i<QPOSSIZE; i++ )
		QPosition[i].hash = 0;
	for ( int i=0; i<MAXDEPTH; i++ )
		KillerMove[i] = MOVE::null;
}



/*
 *	GetNext
 *		次の手を返す
 *		sequence　非NULLでMinMax探索を行ったならば読み筋を格納
 */
MOVE CMinMaxBot::GetNext( const CBoard *board, vector<MOVE> *sequence/*=NULL*/, int *score/*=NULL*/ )
{
	StartTime = clock();

	if ( DisplayMode == DM_NORMAL )
	{
		board->Display();
		printf( "TimeLimit %.2fs\n", TimeLimit/1000.0 );
	}

	if ( sequence != NULL )
		sequence->clear();

	HaltFlag = false;

	MOVE move = GetMinMax( board );
	
	if ( DisplayMode == DM_NORMAL )
		printf( "BestMove %s\n\n", move.ToString(board).c_str() );

	if ( sequence != NULL )
		*sequence = BestSequence[0];
	
	if ( score != NULL )
		*score = BestScore;

	return move;
}



/*
 *	Halt
 *		強制終了
 */
void CMinMaxBot::Halt()
{
	HaltFlag = true;
}



/*
 *	GetState
 *		探索状態を取得
 */
STATE *CMinMaxBot::GetState() const
{
	return &State;
}



/*
 *	GetMinMax
 *		MinMax法で最善手を探す
 */
MOVE CMinMaxBot::GetMinMax( const CBoard *board )
{
	for ( int i=0; i<POSSIZE; i++ )
		Position[i].hash = 0;

	BestMove = MOVE::null;
	BestScore = -INF;
	for ( int i=0; i<MAXDEPTH; i++ )
		BestSequence[i].clear();
	NullMoveSearch = false;
	EvaluateCount = 0;

	CBoard b = *board;

	MOVE bestmove = MOVE::null;
	vector<MOVE> bestsequence;

	try
	{
		double prevtime = 0;	//	前回の探索が終了した時刻
		double pptime = 0;		//	前回の所要時間

		for ( int depth=0; depth<=MaxDepth; depth+=1 )
		{
#if UPDATE_SEARCH_TREE
			//	探索木のルート
			board->GetBoard( &State.tree[0].board );
			State.tree[0].childnum = 0;
			State.tree[0].value = 0;
			State.tree[0].current = true;
#endif	

			if ( DisplayMode == DM_NORMAL )
				printf( " %2d", depth );

			int value = MinMax( &b, 0, depth, -INF, INF, 0 );

#if UPDATE_SEARCH_TREE
			State.tree[0].value = value;
			State.tree[0].current = false;
#endif

			bestmove = BestMove;
			bestsequence = BestSequence[0];

			double time = (double)( clock() - StartTime ) / CLOCKS_PER_SEC;

			if ( DisplayMode == DM_NORMAL )
			{
				//	時間
				//printf( "  %5.2fs %5.2fs %4.1f", time, time-prevtime, (time-prevtime)/pptime );
				printf( "  %5.2fs %5.2fs", time, time-prevtime );

				//	評価値
				printf( "  %8d(%+8d)", value,
						value - b.GetValue()*b.GetTurnSign() );

				//	ハッシュ使用率
				int pos = 0, quies = 0;
				for ( int i=0; i<10000; i++ )
				{
					if ( Position[i].hash != 0 )
						pos++;
					if ( QPosition[i].hash != 0 )
						quies++;
				}
				printf( "  %5.2f%% %5.2f%%", pos/10000.0*100, quies/10000.0*100 );

				//	探索局面数
				printf( "  %10d %4.1fknps", EvaluateCount, (double)EvaluateCount/time/1000 );

				printf( "\n" );
				printf( "[%d]", bestsequence.size() );

				for ( vector<MOVE>::const_iterator m=bestsequence.begin(); m!=bestsequence.end(); m++ )
					printf( "%s", m->ToString(&b).c_str() ),
					b.Move( *m );
				for ( vector<MOVE>::const_iterator m=bestsequence.begin(); m!=bestsequence.end(); m++ )
					b.Undo();
				printf( "\n" );
			}

			//	次の深さに現在の深さの４倍以上の時間がかかると仮定して
			//	制限時間をオーバーしそうなら諦める
			if ( time + 4*( time - prevtime ) > (double)TimeLimit/1000 )
				break;
			pptime = time - prevtime;
			prevtime = time;
		}
	}
	catch ( ... ) {}

	if ( DisplayMode == DM_NORMAL )
		printf( "\n" );

	if ( bestmove != MOVE::null )
		BestMove = bestmove;
	if ( bestsequence.size() != 0 )
		BestSequence[0] = bestsequence;

	return BestMove;
}



/*
 *	SetMaxDepth
 *		探索最大深さを設定
 */
void CMinMaxBot::SetMaxDepth( int depth )
{
	MaxDepth = depth;
}


/*
 *	SetTimeLimit
 *		制限時間[ms]を設定
 */
void CMinMaxBot::SetTimeLimit( int time )
{
	TimeLimit = time;
}



/*
 *	SetDisplayMode
 *		状態の表示方法を設定
 */
void CMinMaxBot::SetDisplayMode( DISPLAYMODE mode )
{
	DisplayMode = mode;
}



/*
 *	SetValueDelta
 *		ValueDeltaを設定
 */
void CMinMaxBot::SetValueDelta( int delta )
{
	ValueDelta = delta;
}



/*
 *	MinMax
 *		MinMax探索
 */
int CMinMaxBot::MinMax( CBoard *board, int depth, int maxdepth, int alpha, int beta, int nodeid )
{
	//	局面表
	HASH hash;
	POSITION *position;

	hash = board->GetHash();
	position = &Position[hash%POSSIZE];

	if ( position->hash == hash  &&
		 position->depth == depth  &&
		 position->maxdepth == maxdepth )
	{
		if ( position->bound < 0 )  alpha = max( alpha, position->value );
		if ( position->bound > 0 )  beta = min( beta, position->value );
		if ( position->bound == 0 )  alpha = beta = position->value;

#if SEQUENCE_IN_POSITION_TABLE
		BestSequence[depth] = position->sequence;
#endif

		if ( alpha >= beta )
			return alpha;
	}

	int best = alpha;
	MOVE bestmove = MOVE::null;
	BestSequence[depth].clear();

	bool finished = board->IsFinished();
	bool checkmated = ! finished ? board->IsCheckmated(board->GetTurn()) : false;

	//	末端
	if ( depth >= maxdepth  ||
		 finished  ||  checkmated )
	{
		if ( finished )
			best = Evaluate(board) * board->GetTurnSign();
		else if ( checkmated )
			best = -MATE + 1000 + board->GetStep();
		else
			best = Quiescence( board, depth, maxdepth, alpha, beta );
	}
	else
	{
#if USE_NULLMOVE
		//	NullMove
		if ( ! NullMoveSearch  &&  depth > 0  &&  maxdepth - depth >= 2 )
		{
			int R = maxdepth - depth >= 6 ? 3 : 2;

			board->MoveNull();
			NullMoveSearch = true;

			int value = -MinMax( board, depth+1, maxdepth-R, -beta-1, -beta, -1 );

			board->Undo();
			NullMoveSearch = false;

			if ( value > beta )
			{
				best = beta;
				goto end;
			}
		}
#endif
		MOVE move[ CBoard::MAXMOVE ];
		int movenum = 0;

		bool checkprevbest	= false;	//	前回の最善手
		bool checkkiller	= false;	//	キラー手
		bool generatemove	= false;	//	指し手を生成したか
		
		//	MinMax
		for ( int m=0; ; m++ )
		{
			if ( generatemove )
			{
				//	指し手を生成済みなら終了判定のみ
				if ( m >= movenum )
					break;
			}
			else
			{
				bool getprevbest	= false;	//	前回の最善手が有効だった
				bool getkiller		= false;	//	キラー手が有効だった

				//	前回の最善手
				if ( ! checkprevbest )
				{
					if ( position->hash == hash  &&
						 board->CheckMove( position->move ) )
					{
						move[m] = position->move;
						getprevbest = true;
					}

					checkprevbest = true;
				}
				
				//	キラー手
				if ( ! checkkiller  &&  ! getprevbest )
				{
					if ( ( m == 0  ||  move[0] != KillerMove[depth] )  &&
						 board->CheckMove( KillerMove[depth] ) )
					{
						move[m] = KillerMove[depth];
						getkiller = true;
					}

					checkkiller = true;
				}

				//	指し手生成
				if ( ! getprevbest  &&  ! getkiller )
				{
					movenum = board->GetMove( move + m );

					if ( m >= movenum )
						break;

					//	これまでに生成してあったものを削除
					for ( int i=0; i<m; i++ )
					{
						bool f = false;

						for ( int j=m; j<m+movenum; j++ )
							if ( f )
								move[j-1] = move[j];
							else
								f = move[j] == move[i];
					}

					//	並び替え
					struct M { MOVE m; int v; };
					M temp[ CBoard::MAXMOVE ];
					for ( int i=m; i<movenum; i++ )
						temp[i].m = move[i],
						board->Move( move[i] ),
						temp[i].v = -board->GetTurnSign() * board->GetValue(),
						board->Undo();

					struct cmp{ bool operator()( const M &a, const M &b ) { return a.v > b.v; } };

					sort( temp+m, temp+movenum, cmp() );

					for ( int i=m; i<movenum; i++ )
						move[i] = temp[i].m;

					generatemove = true;
				}
			}

			////	Futility pruning
			//if ( depth + 1 >= maxdepth  &&
			//	 //	TODO　is_check(move)
			//	 standpat + board->GetValueDifference( move[m] ) <= alpha )
			//	continue;

			//	指す
			board->Move( move[m] );

			//	自玉に王手がかかったまま相手に手番を渡すことはない
			if ( board->IsCheckedGyoku( board->GetTurn()^1 )  &&
				 ! board->IsFinished() )
			{
				board->Undo();
				continue;
			}

			int ext = 0;		//	指手延長

			if ( board->IsCheckedGyoku( board->GetTurn() ) )
				ext++;

			int value;

#if USE_SCOUT
			if ( move[m] == position->move  &&  beta - alpha > 1 )
			{
				value = -MinMax( board, depth+1, maxdepth+ext, -best-1, -best );
	
				if ( best < value  &&  value < beta )
					value = -MinMax( board, depth+1, maxdepth+ext, -beta, -value );
			}
			else
			{
				value = -MinMax( board, depth+1, maxdepth+ext, -beta, -best );
			}
#else

			int childnode = -1;	//	子供のノード

#if UPDATE_SEARCH_TREE
			if ( nodeid >= 0  &&  State.tree[nodeid].childnummax > 0 )
			{
				NODE *node = &State.tree[nodeid];	//	現在のノード

				if ( node->childnum < node->childnummax )
				{
					childnode = node->child[ node->childnum ];
					node->childnum++;
				}
				else
				{
					//	評価が最も低いノードを割り当てる
					int minvalue = INF;
					for ( int i=0; i<node->childnum; i++ )
						if ( State.tree[node->child[i]].value < minvalue )
							minvalue = State.tree[node->child[i]].value,
							childnode = node->child[i];
				}

				board->GetBoard( &State.tree[childnode].board );
				State.tree[childnode].childnum = 0;
				State.tree[childnode].value = 0;
				State.tree[childnode].current = true;					
			}

#endif
			value = -MinMax( board, depth+1, maxdepth+ext, -beta, -best, childnode );
#endif

#if UPDATE_SEARCH_TREE
			if ( childnode >= 0 )
			{
				State.tree[childnode].value = value;
				State.tree[childnode].current = false;
			}
#endif

			board->Undo();

			if ( value > best )
			{
				best = value;
				bestmove = move[m];

				BestSequence[depth].clear();
				BestSequence[depth].push_back( move[m] );
				BestSequence[depth].insert( BestSequence[depth].end(),
											BestSequence[depth+1].begin(),
											BestSequence[depth+1].end() );

				if ( depth == 0 )
					BestMove = bestmove,
					BestScore = best;

				if ( best >= beta )
					break;
			}
		}
	}

end:
	//	局面表
	position->hash = hash;
	position->value = best;
	position->depth = depth;
	position->maxdepth = maxdepth;
	position->move = bestmove;

	if ( best <= alpha )		position->bound = 1;
	else if ( best >= beta )	position->bound = -1;
	else						position->bound = 0;

	#if SEQUENCE_IN_POSITION_TABLE
	position->sequence = BestSequence[depth];
	#endif

	//	キラー手
	if ( bestmove != MOVE::null )
		KillerMove[depth] = bestmove;

	return best;
}



/*
 *	Quiescence
 *		静止探索
 */
int CMinMaxBot::Quiescence( CBoard *board, int depth, int maxdepth, int alpha, int beta )
{
	//	局面表
	HASH hash;
	POSITION *position;

	hash = board->GetHash();
	position = &QPosition[hash%QPOSSIZE];

	if ( position->hash == hash )
	{
		if ( position->bound < 0 )  alpha = max( alpha, position->value );
		if ( position->bound > 0 )  beta = min( beta, position->value );
		if ( position->bound == 0 )  alpha = beta = position->value;

		#if SEQUENCE_IN_POSITION_TABLE
		BestSequence[depth] = position->sequence;
		#endif

		if ( alpha >= beta )
			return alpha;
	}

	int best = alpha;
	MOVE bestmove = MOVE::null;
	BestSequence[depth].clear();

	//	現在の評価値
	int standpat = Evaluate( board ) * board->GetTurnSign();
	if ( standpat > best )
		best = standpat;

	if ( best < beta  &&  ! board->IsFinished() )
	{
		MOVE move[ CBoard::MAXMOVE ];
		int movenum = 0;

		bool checkkiller	= false;	//	キラー手
		bool generatemove	= false;	//	指し手を生成したか

		//	MinMax
		for ( int m=0; ; m++ )
		{
			if ( generatemove )
			{
				//	指し手を生成済みなら終了判定のみ
				if ( m >= movenum )
					break;
			}
			else
			{
				bool getkiller		= false;	//	キラー手が有効だった

				//	キラー手
				if ( ! checkkiller )
				{
					if ( board->CheckMove( KillerMove[depth], CBoard::MV_CAPTURE ) )
					{
						move[m] = KillerMove[depth];
						getkiller = true;
					}

					checkkiller = true;
				}

				//	指し手生成
				if ( ! getkiller )
				{
					movenum = board->GetMove( move + m, CBoard::MV_CAPTURE );

					if ( m >= movenum )
						break;

					//	これまでに生成してあったものを削除
					for ( int i=0; i<m; i++ )
					{
						bool f = false;

						for ( int j=m; j<m+movenum; j++ )
							if ( f )
								move[j-1] = move[j];
							else
								f = move[j] == move[i];
					}

					//	並び替え
					struct M { MOVE m; int v; };
					M temp[ CBoard::MAXMOVE ];
					for ( int i=m; i<movenum; i++ )
						temp[i].m = move[i],
						board->Move( move[i] ),
						temp[i].v = -board->GetTurnSign() * board->GetValue(),
						board->Undo();

					struct cmp{ bool operator()( const M &a, const M &b ) { return a.v > b.v; } };

					sort( temp+m, temp+movenum, cmp() );

					for ( int i=m; i<movenum; i++ )
						move[i] = temp[i].m;

					generatemove = true;
				}
			}

			////	Futility pruning
			//if ( //	TODO　is_check(move)
			//	 standpat + board->GetValueDifference( move[m] ) <= alpha )
			//	continue;

			board->Move( move[m] );

			int value;
			value = -Quiescence( board, depth+1, maxdepth, -beta, -best );

			board->Undo();

			if ( value > best )
			{
				best = value;
				bestmove = move[m];

				BestSequence[depth].clear();
				BestSequence[depth].push_back( move[m] );
				BestSequence[depth].insert( BestSequence[depth].end(),
											BestSequence[depth+1].begin(),
											BestSequence[depth+1].end() );

				if ( best >= beta )
					break;
			}
		}
	}

	//	局面表
	position->hash = hash;
	position->value = best;
	position->depth = depth;
	position->maxdepth = maxdepth;
	position->move = bestmove;

	if ( best <= alpha )		position->bound = 1;
	else if ( best >= beta )	position->bound = -1;
	else						position->bound = 0;

	#if SEQUENCE_IN_POSITION_TABLE
	position->sequence = BestSequence[depth];
	#endif

	//	キラー手
	if ( bestmove != MOVE::null )
		KillerMove[depth] = bestmove;

	return best;
}



/*
 *	Evaluate
 *		盤面を評価
 */
int CMinMaxBot::Evaluate( const CBoard *board )
{
	EvaluateCount++;

	if ( EvaluateCount % 0x1000 == 0 )
	{
		if ( HaltFlag )
			throw 0;

		if ( TimeLimit > 0  &&
			 clock() - StartTime > TimeLimit * 1000 / CLOCKS_PER_SEC )
			throw 0;
	}

	return board->GetValue() + ((unsigned int)board->GetHash()^ValueDelta)%1024 - 512;
}
