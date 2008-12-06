
#include <stdio.h>
#include <time.h>
#include "config.h"
#include "board.h"
#include "minmaxbot.h"
#include "mt19937ar.h"



static void		LoadCSA();
static void		TestBot();
static void		GenerateKifu();
static void		LoadKifu();
void			LearningBonanza();



int main()
{
	setvbuf( stdout, NULL, _IONBF, 0 );

#if INITIALIZE_RANDOM
	init_genrand( (unsigned int)time(NULL) );
#endif

	//LoadCSA();
	TestBot();
	//GenerateKifu();
	//LearningBonanza();
	//LoadKifu();
	return 0;

	CBoard b;
	b.Initialize();
	b.FromString( "alahnaanhaafafkcaaaauaaai100000000000+" );
	b.Display();
	return 0;

	for ( int i=0; i<100; i++ )
	{
		MOVE move[CBoard::MAXMOVE];

		int movenum = b.GetMove( move );

		b.Move( move[genrand_int32()%movenum] );
		b.Display();
	}

	return 0;
}



void LoadCSA()
{
	FILE *f;
	fopen_s( &f, "kifu.csa", "r" );
	if ( f == NULL )
		return;

	CBoard board;
	board.Initialize();

	char buf[10240];

	while ( fgets( buf, sizeof buf, f ) != NULL )
	{
		if ( buf[strlen(buf)-1] == '\n' )
			buf[strlen(buf)-1] = '\0';
		
		if ( strcmp( buf, "PI" ) == 0 )
		{
			//printf( "========\n" );
			board.Initialize();
		}
		else
		{
			MOVE move;
			if ( move.FromCSA( buf, &board ) )
			{
				board.Move( move );
				//board.Display();
				printf( "%s\n", board.ToString().c_str() );
			}
		}
	}

	fclose( f );
}



void TestBot()
{
	const char *state[] = {
		"nlfhjdaaaaaaaaaaaeacigakm000000000000-", "nlahjdaafaaaaaaaaeacigakm000000000000+", "nlahjdaafaaaaeaaaaacigakm000000000000-", "nlahjdaaaaaaafaaaaacigakm000100000000+", "nlahjdaaaaaaagaaaaaciaakm001100000000-", "nlahjdaaaaaaagaafaaciaakm001000000000+", "nlahjdaaeaaaagaafaaciaakm000000000000-", "nlaajdaaehaaagaafaaciaakm000000000000+", "nlaajdaaehaaaaaafgaciaakm000000000000-", "nlaajdaaehaaaaaaagacraakm000000010000+", "nlaaqdaaahaaaaaaagacraakm000000110000-", "nlaaqdaaaaaaaaaaahacraakm000001110000+", "nlaaqdaaaaaaaaaaakacraaam000011110000-", "nlaaqdhaaaaaaaaaakacraaam000010110000+", "nlaaqdhaaaaaaaaaakgcraaam000000110000-", "nlaaqdaaaaaaaaaaakhcraaam000001110000+", "nlaaqdaaaaaaaaaaakmcraaaa000011110000-", "nlaaqdhaaaaaaaaaakmcraaaa000010110000+", "nlaaqdhaaaaagaaaakmcraaaa000000110000-", "nlaaqdaaaaaahaaaakmcraaaa000001110000+", "nlaaqdaaaaaakaaaaamcraaaa000011110000-", "nlaaqdaaaaaakaaaaamcrjaaa000011100000+", "nlaaqdaaaaaakamaaaacrjaaa000011100000-", "nlaaqdaaaaaakamaaaacrjhaa000010100000+", "nlaaqdaaaaaakmaaaaacrjhaa000010100000-", "nlaaqdaaaaaakmaataacrjaaa000010100000+", "nlaaqdaaaaaakmaatagcrjaaa000000100000-", "nlaaqdaaaaaakmaatagcraava000000100000+", "nlaiqdaaaaaakmaatagcraava000000000000-", "nlaiqdaaaaaakmaatavcraaaa000001000000+", "nlaiqdaaaaaakaaatamcraaaa000001100000-", "nlaiqdaaaaaataaaaamcraaaa000001100100+",
		"nlaiqdaaaaaamaaaaaacraaaa000011100100-", "nlliqdaaaaaamaaaaaacraaaa000011100000+", "nllaqdaaaaaamuaaaaacraaaa000011100000-", "nllaqdaaaaaamuaraaacaaaaa000011100000+", "nllaqdaaaaaamaarauacaaaaa000011100000-", "nllaqdaahaaamaarauacaaaaa000010100000+", "nllaqdaahaaaamarauacaaaaa000010100000-", "nllahdaaaaaaamarauacaaaaa000110100000+", "nllahdaaaaaaamarauacaaaag000100100000-", "nllahdfaaaaaamarauacaaaag000000100000+", "nllahdfaaaaaamauaaacaaaag001000100000-", "nllaadfaaaaahmauaaacaaaag001000100000+", "nllaadfaaaaahmauaaecaaaag000000100000-", "nalaadflaaaahmauaaecaaaag000000100000+", "nalaadflaaaahaauamecaaaag000000100000-", "nalaadflaaaaaaauamhcaaaag000100100000+", "nalaadflaaaaaaauaamcaaaag000110100000-", "nalaadfaaaaalaauaamcaaaag000110100000+", "nalaadfaaaaalaauaaacaamag000110100000-", "nalaadfaaafalaauaaacaamag000010100000+", "nalaadfaaafalaaaaaacaumag000010100000-", "nalaadfaaaaalaaafaacaumag000010100000+", "nalaadfaaaaalaaauaacaamag001010100000-", "nalaadfaaaaaaaaalaacaamag001010110000+", "nalaadfaaaaaaaaamaacaaaag001010111000-", "nalaadfaaaaaaaaamaacaaajg001010101000+", "nalaadfaaaaaaaaamagcaaajg001000101000-", "nalaadfaaaaaaaaamagcaaaav001001101000+", "nalaadfaaaaaaaaamaacaaaag001001201000-", "anlaadfaaaaaaaaamaacaaaag001001201000+", "anlaadgaaaaaaaaamaacaaaaa002001201000-", "anaaadlaaaaaaaaamaacaaaaa002002201000+",
		"aniaadlaaaaaaaaamaacaaaaa002002101000-", "aanaadlaaaaaaaaamaacaaaaa002002111000+", "aanaadlaeaaaaaaamaacaaaaa001002111000-", "aaaaadlanaaaaaaamaacaaaaa001102111000+", "aaaaadlanaaaaaeamaacaaaaa000102111000-", "aaaaadlaaaaaaneamaacaaaaa000102111000+", "aaaaadlaiaaaaneamaacaaaaa000102011000-", "nlfhjdaaaaaaaaaaagaciaekm000000000000-", "nlfajdahaaaaaaaaagaciaekm000000000000+", "nlfajdahaaaaaaaaegaciaakm000000000000-", "nlaajdfhaaaaaaaaegaciaakm000000000000+", "nlaajdfhaaaaaaaaegkciaaam000000000000-", "nlajadfhaaaaaaaaegkciaaam000000000000+", "nlajadfhaaaakaaaegaciaaam000000000000-", "nlajadahaaaafaaaegaciaaam000000000100+", "nlajadahaaaaeaaaagaciaaam001000000100-", "nlajadlhaaaaeaaaagaciaaam001000000000+", "nlajadlhaaaaeaaaegaciaaam000000000000-", "nlajadahaaaalaaaegaciaaam000100000000+", "nlajadahaaaaeaaaagaciaaam000100001000-", "nlajadahaaafeaaaagaciaaam000000001000+", "nlajadahagafeaaaaaaciaaam000000001000-", "nlaajdahagafeaaaaaaciaaam000000001000+", "nlaajdahkgafeaaaaaaciaaam000000000000-", "nlaaadahkjafeaaaaaaciaaam000001000000+", "nlaaadahakafeaaaaaaciaaam000001100000-", "nlaaadaaakafehaaaaaciaaam000001100000+", "nlaaadaaaaafehkaaaaciaaam000001100000-", "nlaaadaaaaafeakaahaciaaam000001100000+", "nlaiadaaaaafeakaahaciaaam000001000000-", "nlaiadafaaaaeakaahaciaaam000001000000+", "nlaiadaeaaaaaakaahaciaaam001001000000-",
		"nlaiadaeaaaahakaahaciaaam001000000000+", "nlaiadaeaaaahakaaheciaaam000000000000-", "nlaiadaeaaaaaakaahhciaaam000100000000+", "nlaiadaeaaaaaakaahmciaaaa000110000000-", "nlaiadaeaaaaaakaaamciaata000110000000+", "nlaiadaeaaaaaakaaaaciaama000120000000-", "nlaiadaeaaaaaakaafaciaama000020000000+", "nlaiadaeaaaaaakaamaciaaaa001020000000-", "alaiadneaaaaaakaamaciaaaa001020000000+", "auaaadneaaaaaakaamaciaaaa001020001000-", "nlfhjdaaaaaaaaaaagaciaekm000000000000-", "nlahjdaafaaaaaaaagaciaekm000000000000+", "nlahjdaafaaaaaaaegaciaakm000000000000-", "naahjdlafaaaaaaaegaciaakm000000000000+", "naahjdlafaaaaaaaegaciakam000000000000-", "anahjdlafaaaaaaaegaciakam000000000000+", "anahjglafaaaaaaaeaaciakam100000000000-", "anahjlaafaaaaaaaeaaciakam100001000000+", "anahjlaafaacaaaaeaaciakam000001000000-", "hnahjlaafaacaaaaeaaciakam000000000000+", "hnahjlcafaaaaaaaeaaciakam000000000000-", "anahjlhafaaaaaaaeaaciakam010000000000+", "anahjlhafaaaaaaaeakciaaam010000000000-", "anahjlhaaaaaafaaeakciaaam010000000000+", "anahjlhaaaaaefaaaakciaaam010000000000-", "anahjlaaaaaahfaaaakciaaam010100000000+", "anahjlaaaaaahfaaaakciaama010100000000-", "anahjlaaaaaahaaaaafciaama010100000100+", "anahjlaaaaaahaaaamfciaaaa010100000100-", "anahjaaaaaalhaaaamfciaaaa010100000100+", "anahjaaaaaalhmaaaafciaaaa010100000100-", "anaajaahaaalhmaaaafciaaaa010100000100+",
	};

	CBoard board;
	board.Initialize();

	FILE *f;
	int weight[CBoard::ELEMNUM];
	fopen_s( &f, "weight.txt", "r" );
	for ( int i=0; i<CBoard::ELEMNUM; i++ )
		if ( fscanf_s( f, "%d", &weight[i] ) != 1 )
			throw "Failed to load weight.txt";
	fclose( f );
	board.SetWeight( weight );

	CMinMaxBot bot;
	//bot.SetTimeLimit( 10000 );
	bot.SetMaxDepth( 7 );
	bot.SetDisplayMode( CMinMaxBot::DM_NORMAL );

	for ( int i=0; i<sizeof state/sizeof state[0]; i++ )
	if ( i % 7 == 0 )
	{
		board.FromString( state[i] );

		board.Display();

		vector<MOVE> move;
		bot.GetNext( &board, &move );

		for ( int i=0; i<(int)move.size(); i++ )
		{
			printf( " %s", move[i].ToString( &board ).c_str() );
			board.Move( move[i] );
		}
		for ( int i=0; i<(int)move.size(); i++ )
			board.Undo();

		printf( "\n\n" );
	}

	printf( "%f [s]\n", (double)clock() / CLOCKS_PER_SEC );
}



void GenerateKifu()
{
	CBoard board;
	board.Initialize();

	CMinMaxBot bot[2];

	bot[0].SetMaxDepth( 5 );
	bot[1].SetMaxDepth( 5 );
	bot[0].SetValueDelta( genrand_int32() );
	bot[1].SetValueDelta( genrand_int32() );

	vector<HASH> log;

	for ( int i=0; ; i++ )
	{
		int t = board.GetTurn();

		bool loop = false;
		HASH hash = board.GetHash();

		for ( int j=0; j<(int)log.size()-1; j++ )
			if ( log[j] == hash )
				loop = true;

		if ( board.IsFinished()  ||
			 board.IsCheckmated(t)  ||
			 loop )
		{
			board.Initialize();
			log.clear();
			bot[0].SetValueDelta( genrand_int32() );
			bot[1].SetValueDelta( genrand_int32() );
			bot[0].Initialize();
			bot[1].Initialize();
			t = board.GetTurn();
		}

		int old = board.GetValue();

		MOVE m;
		int score;
		m = bot[t].GetNext( &board, NULL, &score  );

		if ( old - score > 2000 )
			printf( "%s %08x %4d\n", board.ToString().c_str(), m.ToInt(), board.GetStep() );

		board.Move( m );
		log.push_back( board.GetHash() );
	}
}



void LoadKifu()
{
	//	Šû•ˆ
	FILE *kifu;
	fopen_s( &kifu, "kifu.txt", "r" );
	if ( kifu == NULL )
		return;

	vector<int> step;

	int ps = 0;

	char buf[1024];

	while ( fgets( buf, sizeof buf, kifu ) != 0 )
	{
		char			sboard[1024];
		unsigned int	smove;
		int				s;

		if ( sscanf_s( buf, "%s %x %d", sboard, sizeof sboard, &smove, &s ) != 3 )
			continue;

		if ( s < ps )
			step.push_back( ps );
		ps = s;
	}

	printf( "kifu num = %d\n", step.size() );

	for ( int i=0; i<200; i++ )
	{
		int r = 0;
		int n = 0;

		for ( int j=0; j<(int)step.size(); j++ )
			if ( step[j] > i )
				r += step[j] - i,
				n++;

		printf( "%3d %f\n", i, (double)r/n );
	}
}