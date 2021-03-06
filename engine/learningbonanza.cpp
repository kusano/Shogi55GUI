#include	<math.h>
#include	<algorithm>
#include	"minmaxbot.h"
#include	"mt19937ar.h"



using namespace std;



//static void	GetLeaf( CBoard *board, MOVE move, CMinMaxBot *bot, int *value, int elem[CBoard::ELEMNUM] );
static void Constrain( int weight[ CBoard::ELEMNUM ] );



template<class T>
static int sign( T a ) { return a > 0 ? 1 : a < 0 ? -1 : 0; }



void LearningBonanza()
{
	int weight[ CBoard::ELEMNUM ] = { 3914, 19354, 22938, 27066, 26730, 0, 6528, 13584, 53824, 60080, 17360, 17360, 15120, 17056, 19120, 19536, 22288, 18928, 24864, 21584 };

	for ( int i=22; i<CBoard::ELEMNUM; i++ )
		weight[i] = genrand_int32() % 15 - 7;

	Constrain( weight );

	double prevtime = 0;

	//	棋譜
	FILE *kifu;
	fopen_s( &kifu, "kifu.txt", "r" );
	if ( kifu == NULL )
		return;

	for ( int step=0; ; step++ )
	{
		bool renewleaf = step % 10 == 0;		//	このステップで探索を実行するか

		CBoard board;
		board.Initialize();
		board.SetWeight( weight );

		CMinMaxBot bot;
		bot.SetMaxDepth( 0 );	//	静止探索のみ実行

		fseek( kifu, 0, SEEK_SET );

		//	末端局面までの手順
		FILE *leaf;
		fopen_s( &leaf, "leaf.bin", renewleaf?"wb":"rb" );
		if ( leaf == NULL )
		{
			printf( "Failed to open leaf.bin\n" );
			renewleaf = true;
		}

		int freq[ CBoard::ELEMNUM ] = { 0 };	//	特徴の出現回数
		double dJ[ CBoard::ELEMNUM ] = { 0 };	//	∇vJ
		int J = 0;

		char	buf[1024];

		int n = 0;

		while ( fgets( buf, sizeof buf, kifu ) != 0 )
		{
			char			sboard[1024];
			unsigned int	smove;

			if ( sscanf_s( buf, "%s %x", sboard, sizeof sboard, &smove ) != 2 )
				continue;
			if ( ! board.FromString( sboard ) )
				continue;

			if ( n % 100 == 0 )
				fprintf( stderr, "step = %d, n = %d    \r", step, n );

			MOVE m0;	//	棋譜の手
			m0.FromInt( smove );

			//	評価関数のチェック
			//int v1 = board.GetValue();
			//board.SetWeight( weight );
			//int v2 = board.GetValue();
			//int v3 = 0;
			//int e[CBoard::ELEMNUM];
			//board.GetElement( e );
			//for ( int i=0; i<CBoard::ELEMNUM; i++ )
			//	v3 += weight[i] * e[i];
			//printf( "%8d %8d %8d\n", v1, v2, v3 );

			//	末端までの手順を取得
			MOVE move[ CBoard::MAXMOVE ];
			int movenum = board.GetMove( move );
			vector<MOVE> sequence[ CBoard::MAXMOVE ];	//	最善手までの手順

			if ( renewleaf )
			{
				for ( int i=0; i<movenum; i++ )
				{
					board.Move( move[i] );
					bot.GetNext( &board, &sequence[i] );
					board.Undo();

					//	保存
					if ( leaf != NULL )
					{
						unsigned char b;
						unsigned int t;
						b = (unsigned char)sequence[i].size();
						fwrite( &b, 1, 1, leaf );
						for ( int j=0; j<(int)sequence[i].size(); j++ )
							t = sequence[i][j].ToInt(),
							fwrite( &t, 4, 1, leaf );
					}
				}
			}
			else
			{
				for ( int i=0; i<movenum; i++ )
				{
					//	読み込み
					unsigned char b;
					unsigned int t;
					fread( &b, 1, 1, leaf );
					sequence[i].resize( b );
					for ( int j=0; j<(int)sequence[i].size(); j++ )
						fread( &t, 4, 1, leaf ),
						sequence[i][j].FromInt( t );

					//	正当性をチェック
					bool f = true;

					HASH h = board.GetHash();

					board.Move( move[i] );

					for ( int j=0; j<(int)sequence[i].size(); j++ )
						if ( board.CheckMove( sequence[i][j] ) )
							board.Move( sequence[i][j] );
						else
							f = false;
					
					//	元の盤面に戻す
					while ( board.GetHash() != h )
						board.Undo();

					if ( ! f )
					{
						board.Move( move[i] ),
						bot.GetNext( &board, &sequence[i] ),
						board.Undo();
					}
				}
			}

			//	棋譜の手の評価・評価要素を取得
			int xi0;					//	棋譜の手の評価
			int e0[ CBoard::ELEMNUM ];	//	棋譜の手の評価要素

			for ( int m=0; m<movenum; m++ )
			if ( move[m] == m0 )
			{
				board.Move(move[m]);
				for ( int j=0; j<(int)sequence[m].size(); j++ )
					board.Move( sequence[m][j] );

				xi0 = board.GetValue();
				board.GetElement( e0 );

				for ( int j=0; j<(int)sequence[m].size(); j++ )
					board.Undo();
				board.Undo();
			}
			
			//	学習
			for ( int m=0; m<movenum; m++ )
			if ( move[m] != m0 )
			{
				//	評価・評価要素を取得
				int xi;						//	評価
				int e[ CBoard::ELEMNUM ];	//	評価要素
			
				board.Move(move[m]);
				for ( int j=0; j<(int)sequence[m].size(); j++ )
					board.Move( sequence[m][j] );

				xi = board.GetValue();
				board.GetElement( e );

				for ( int j=0; j<(int)sequence[m].size(); j++ )
					board.Undo();
				board.Undo();

				//
				int turn = board.GetTurnSign();

				if ( turn > 0  &&  xi >= xi0  ||
					 turn < 0  &&  xi <= xi0 )
					J++;

				double T = 1 / ( 1 + exp(-3*(xi-xi0)/8000.0) );
				double dT = T * ( 1 - T );

				for ( int i=0; i<CBoard::ELEMNUM; i++ )
				if ( e[i] != e0[i] )
				{
					dJ[i] += dT * ( e[i] - e0[i] ) * turn;
					if ( i >= 22 )
						dJ[i] += dT * abs( e[i] - e0[i] ) * weight[i] / 2000.0;
					freq[i]++;
				}
			}

			n++;

			//if ( n >= 10000 )
			//	break;
		}

		for ( int i=0;i<CBoard::ELEMNUM; i++ )
		//for ( int i=0;i<22; i++ )
			if ( freq[i] >= n / 50 )
				weight[i] -= 8 * sign( dJ[i] );
		Constrain( weight );

		if ( leaf != NULL )
			fclose( leaf );

		FILE *f;
		fopen_s( &f, "weightlog.txt", "a" );
		if ( f != NULL )
		{
			for ( int i=0; i<CBoard::ELEMNUM; i++ )
				fprintf( f, " %d", weight[i] );
			fprintf( f, "\n" );
			fclose( f );
		}
		else
			printf( "!! Failed to open weightlog.txt\n" );

		fopen_s( &f, "freqlog.txt", "w" );
		if ( f != NULL )
		{
			for ( int i=0; i<CBoard::ELEMNUM; i++ )
				fprintf( f, " %d", freq[i] );
			fprintf( f, "\n" );
			fclose( f );
		}
		else
			printf( "!! Failed to open freqlog.txt\n" );

		double time = (double)clock() / CLOCKS_PER_SEC;

		//printf( "STEP %d, J %.2f, time %.2f, dt %.2f\n", step, (double)J/n, time, time-prevtime );
		for ( int i=0; i<10; i++ )
		{
			printf( " %d", weight[i] );
		}
		printf( "\n" );

		prevtime = time;
	}
}



/*
 *	Constarin
 *		拘束条件
 */
void Constrain( int weight[ CBoard::ELEMNUM ] )
{
	//	歩銀角飛金　の価値の合計を一定値にする
	int sum = 0;
	for ( int i=0; i<5; i++ )
		sum += weight[i];

	int d = ( 100000 - sum ) / 5;

	for ( int i=0; i<5; i++ )
		weight[i] += d;

	//	駒割は０以上を仮定
	for ( int i=0; i<22; i++ )
		weight[i] = max( weight[i], 0 );
}
