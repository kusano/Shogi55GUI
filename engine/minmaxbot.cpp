
#include	"minmaxbot.h"
#include	"mt19937ar.h"
#include	<algorithm>



using namespace std;



//	�ǖʕ\
CMinMaxBot::POSITION CMinMaxBot::Position[CMinMaxBot::POSSIZE];
CMinMaxBot::POSITION CMinMaxBot::QPosition[CMinMaxBot::QPOSSIZE];



/*
 *	CMinMaxBot
 *		�R���X�g���N�^
 */
CMinMaxBot::CMinMaxBot() :
	MaxDepth( 99 ),
	TimeLimit( 999000 ),
	DisplayMode( DM_NONE )
{
}



/*
 *	Initialize
 *		KillerMove, PositionTable���N���A
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
 *		���̎��Ԃ�
 *		sequence�@��NULL��MinMax�T�����s�����Ȃ�Γǂ݋؂��i�[
 */
MOVE CMinMaxBot::GetNext( const CBoard *board, vector<MOVE> *sequence/*=NULL*/, int *score/*=NULL*/ )
{
	StartTime = clock();

	if ( sequence != NULL )
		sequence->clear();

	HaltFlag = false;

	MOVE move = GetMinMax( board );
	
	if ( DisplayMode == DM_NORMAL )
		printf( "�]���l�@%8d�@�Ֆʕ]���񐔁@%8d\n", BestScore, EvaluateCount );

	if ( sequence != NULL )
		*sequence = BestSequence[0];
	
	if ( score != NULL )
		*score = BestScore;

	return move;
}



/*
 *	Halt
 *		�����I��
 */
void CMinMaxBot::Halt()
{
	HaltFlag = true;
}



/*
 *	GetMinMax
 *		MinMax�@�ōőP���T��
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
		double prevtime = 0;	//	�O��̒T�����I����������

		for ( int depth=0; depth<=MaxDepth; depth+=1 )
		{
			MinMax( &b, 0, depth, -INF, INF );

			bestmove = BestMove;
			bestsequence = BestSequence[0];

			double time = (double)( clock() - StartTime ) / CLOCKS_PER_SEC;

			if ( DisplayMode == DM_NORMAL )
				printf( " %d", depth );

			//	���̐[���Ɍ��݂̐[���ȏ�̎��Ԃ�������Ɖ��肵��
			//	�������Ԃ��I�[�o�[�������Ȃ���߂�
			if ( time + ( time - prevtime ) > (double)TimeLimit/1000 )
				break;
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
 *		�T���ő�[����ݒ�
 */
void CMinMaxBot::SetMaxDepth( int depth )
{
	MaxDepth = depth;
}


/*
 *	SetTimeLimit
 *		��������[ms]��ݒ�
 */
void CMinMaxBot::SetTimeLimit( int time )
{
	TimeLimit = time;
}



/*
 *	SetDisplayMode
 *		��Ԃ̕\�����@��ݒ�
 */
void CMinMaxBot::SetDisplayMode( DISPLAYMODE mode )
{
	DisplayMode = mode;
}



/*
 *	SetValueDelta
 *		ValueDelta��ݒ�
 */
void CMinMaxBot::SetValueDelta( int delta )
{
	ValueDelta = delta;
}



/*
 *	MinMax
 *		MinMax�T��
 */
int CMinMaxBot::MinMax( CBoard *board, int depth, int maxdepth, int alpha, int beta )
{
	//	�ǖʕ\
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

	//	���[
	if ( depth >= maxdepth ||  board->IsFinished() )
	{
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

			int value = -MinMax( board, depth+1, maxdepth-R, -beta-1, -beta );

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

		bool checkprevbest	= false;	//	�O��̍őP��
		bool checkkiller	= false;	//	�L���[��
		bool generatemove	= false;	//	�w����𐶐�������
		
		//	MinMax
		for ( int m=0; ; m++ )
		{
			if ( generatemove )
			{
				//	�w����𐶐��ς݂Ȃ�I������̂�
				if ( m >= movenum )
					break;
			}
			else
			{
				bool getprevbest	= false;	//	�O��̍őP�肪�L��������
				bool getkiller		= false;	//	�L���[�肪�L��������

				//	�O��̍őP��
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
				
				//	�L���[��
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

				//	�w���萶��
				if ( ! getprevbest  &&  ! getkiller )
				{
					movenum = board->GetMove( move + m );

					if ( m >= movenum )
						break;

					//	����܂łɐ������Ă��������̂��폜
					for ( int i=0; i<m; i++ )
					{
						bool f = false;

						for ( int j=m; j<m+movenum; j++ )
							if ( f )
								move[j-1] = move[j];
							else
								f = move[j] == move[i];
					}

					//	���ёւ�
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
			//	 //	TODO�@is_check(move)
			//	 standpat + board->GetValueDifference( move[m] ) <= alpha )
			//	continue;

			//	�w��
			board->Move( move[m] );

			//	���ʂɉ��肪���������܂ܑ���Ɏ�Ԃ�n�����Ƃ͂Ȃ�
			if ( board->IsCheckedGyoku( board->GetTurn()^1 )  &&
				 ! board->IsFinished() )
			{
				board->Undo();
				continue;
			}

			int ext = 0;		//	�w�艄��

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
			value = -MinMax( board, depth+1, maxdepth+ext, -beta, -best );
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
	//	�ǖʕ\
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

	//	�L���[��
	if ( bestmove != MOVE::null )
		KillerMove[depth] = bestmove;

	return best;
}



/*
 *	Quiescence
 *		�Î~�T��
 */
int CMinMaxBot::Quiescence( CBoard *board, int depth, int maxdepth, int alpha, int beta )
{
	//	�ǖʕ\
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

	//	���݂̕]���l
	int standpat = Evaluate( board ) * board->GetTurnSign();
	if ( standpat > best )
		best = standpat;

	if ( best < beta  &&  ! board->IsFinished() )
	{
		MOVE move[ CBoard::MAXMOVE ];
		int movenum = 0;

		bool checkkiller	= false;	//	�L���[��
		bool generatemove	= false;	//	�w����𐶐�������

		//	MinMax
		for ( int m=0; ; m++ )
		{
			if ( generatemove )
			{
				//	�w����𐶐��ς݂Ȃ�I������̂�
				if ( m >= movenum )
					break;
			}
			else
			{
				bool getkiller		= false;	//	�L���[�肪�L��������

				//	�L���[��
				if ( ! checkkiller )
				{
					if ( board->CheckMove( KillerMove[depth], CBoard::MV_CAPTURE ) )
					{
						move[m] = KillerMove[depth];
						getkiller = true;
					}

					checkkiller = true;
				}

				//	�w���萶��
				if ( ! getkiller )
				{
					movenum = board->GetMove( move + m, CBoard::MV_CAPTURE );

					if ( m >= movenum )
						break;

					//	����܂łɐ������Ă��������̂��폜
					for ( int i=0; i<m; i++ )
					{
						bool f = false;

						for ( int j=m; j<m+movenum; j++ )
							if ( f )
								move[j-1] = move[j];
							else
								f = move[j] == move[i];
					}

					//	���ёւ�
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
			//if ( //	TODO�@is_check(move)
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

	//	�ǖʕ\
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

	//	�L���[��
	if ( bestmove != MOVE::null )
		KillerMove[depth] = bestmove;

	return best;
}



/*
 *	Evaluate
 *		�Ֆʂ�]��
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
