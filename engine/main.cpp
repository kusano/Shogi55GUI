
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
static void		GenerateBook();
void			LearningBonanza();



int main()
{
	setvbuf( stdout, NULL, _IONBF, 0 );

#if INITIALIZE_RANDOM
	init_genrand( (unsigned int)time(NULL) );
#endif

	//LoadCSA();
	//TestBot();
	//GenerateKifu();
	//LearningBonanza();
	//LoadKifu();
	GenerateBook();
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



void GenerateBook()
{
	static int weight[CBoard::ELEMNUM] = { 4237, 19517, 23069, 26893, 26285, 0, 6240, 9840, 50224, 53872, 17120, 19392, 14096, 15440, 18816, 16864, 22256, 13664, 22416, 20784, 0, 0, -5, 5, 7, -2, -3, 250, -921, -354, -212, -202, 467, 174, 105, -257, -281, -519, -414, 493, 336, 442, -441, -547, -559, 621, 207, -348, -678, -404, -701, -1004, 645, 418, 13, 81, 175, -397, 118, -79, -128, 300, -510, 399, 126, 73, 11, -897, -759, -131, 368, -309, -314, -335, 46, -1, -187, -897, -406, -133, 253, -268, -323, -1, 458, 86, -251, -455, 128, 261, 719, -148, -322, -352, -334, 365, 239, 420, 279, 542, 566, -26, 81, -324, -243, 211, -204, 508, -592, -111, -167, -14, -345, 39, -179, 201, 37, 138, 192, -92, -189, -62, -348, 189, -394, -375, -363, 185, 308, 300, 84, -240, -246, -42, -325, 76, 394, -286, -235, -9, 326, 313, -718, -109, 443, 76, -548, 2, 3, -119, 267, 371, 244, -98, -429, -17, -42, 52, -521, -594, -170, -199, 82, -124, -336, -407, -15, 25, -99, -140, -13, 665, -7, 6, 313, -132, 76, -7, -5, -1, -4, 97, 1, -1, -1, 1, -3, -5, -2, 7, 1, -4, 1, 1, 7, -3, 4, 7, 773, -349, -51, -571, -3, -6, 265, -166, 1, 3, 4, 3, 1, 7, 6, -5, -5, -3, 5, -6, -1, 3, -6, -7, -539, 53, -210, 140, -266, 409, 508, 86, -63, 236, 55, -772, -255, 52, -62, -320, 745, 214, -365, 1, 243, 289, 5, 1, -1, 198, -42, 147, 62, -249, -255, -316, -128, 35, -205, -42, -883, 109, 484, 82, 209, -108, -252, -291, -84, 54, -58, 282, 287, -6, -5, -1, 13, 3, 293, 3, -5, 348, 462, -601, -5, 1, 377, -681, -185, -1049, -351, -7, -514, 173, -13, 294, 212, -142, -31, 134, -113, 202, 33, -5, -841, -337, -7, -12, -563, 246, 113, 293, -98, 275, 22, -30, 287, -119, -367, 4, 3, 23, 521, -786, -1, 2, 4, 270, -346, -5, 6, -272, -196, -267, 89, 311, -3, -260, -338, 20, 357, 528, 133, 23, -238, -131, 21, 434, 13, -368, -146, -1, -407, -236, -158, -379, -153, -481, -44, 129, 438, -15, 342, -389, 350, 182, 1, 228, 224, -4, -103, 1, 278, -129, -177, 293, 27, -100, -195, 124, -100, 7, -529, 28, 220, -165, 66, -71, -126, 50, -273, -118, -138, -53, -3, 0, -7, -338, -423, 6, 2, 185, -419, 137, 4, -5, -414, 976, -354, -337, -114, 2, -373, -352, 77, 6, -111, -37, 549, -218, 156, -53, -286, 13, -279, -380, 4, 51, 352, -50, -118, -260, -193, -71, -96, -160, -129, 407, 331, 46, -405, 1, -259, -206, 64, 233, 118, 269, -128, -53, 52, -176, -454, 537, -35, -611, -3, 358, 170, -111, -422, 3, -403, 338, 380, -361, -18, 176, 462, -53, -663, 3, 263, -91, -69, 369, -3, 10, 111, 208, 231, -299, -803, -116, -90, -95, -7, 108, 149, 9, -133, -144, -289, 126, 78, 70, -244, 332, -167, -2, -3, 41, 29, -34, 5, -1, -558, 384, -139, -3, 2, -159, -363, 163, 146, -305, 4, -359, -212, 29, 169, 338, -4, 84, -195, 377, 159, -254, 174, -227, -239, -1, -529, -123, -112, -53, 118, 59, 295, 42, -204, 81, -551, 79, -4, 244, -2, -291, -318, 55, 203, -82, 270, 225, -98, -38, 89, 18, 348, -185, -84, 1, -44, -638, -30, -62, 9, 67, -213, 41, 193, 106, -154, -128, 421, 233, -3, 2, 250, 92, -58, 421, 146, 158, -432, -151, -34, 279, 51, 127, 81, -2, 914, -217, 12, 210, 425, -52, -441, -274, -74, -22, 11, 86, 274, -358, 0, 161, -150, -273, 85, 95, -57, -432, -204, -126, 195, 592, -43, 578, 155, -6, 149, -106, 206, -92, -191, -319, -43, -206, -11, -49, -45, -350, 2, 1, 143, -62, 170, 5, 6, -129, -112, -69, 3, -6, -389, 69, -16, 10, 642, 3, 171, 305, -107, -310, -251, -131, -205, -144, -157, -393, -674, -217, 176, -684, 0, 176, -2, -177, 51, -115, 106, 227, -242, -209, -231, -256, -2, 75, -37, 2, -125, 448, -262, 94, 154, -223, -92, 82, 157, -166, -150, -131, -267, -556, 6, 127, 320, -218, -401, 124, -563, 783, 22, -78, 319, -310, 479, -105, 66, -1, -243, 100, -66, 641, 189, -256, -359, 627, 263, -117, -150, -501, 65, 799, 7, 192, 334, -4, -390, -51, -493, 143, -617, 51, -31, -2, -646, 483, 10, -4, -155, 182, -139, 123, -22, -398, -557, -391, -203, 67, -397, -414, -30, -340, 7, -209, 36, 160, 10, 48, -199, 74, 90, 49, -448, -79, 11, 386, -99, -7, -286, -271, 77, 311, 245, -138, -277, -324, -15, 266, -108, -283, 137, -471, 7, 18, -172, -92, -403, -89, 174, -471, -174, -236, 33, 179, 223, 3, -5, 4, 6, 6, 6, -5, -7, 4, 1, 2, 2, 6, 1, 7, -5, 5, 0, 5, -5, -5, 7, 7, -6, 5, 4, 6, -2, -3, -4, -1, -3, -2, 2, -2, 4, 3, -6, -6, -6, 2, 5, -6, -2, 6, -3, 2, 2, -2, -480, -3, 6, -1, 378, -27, -2, -5, -2, -420, 625, -3, -3, -6, -4, 158, 2, -2, 5, -548, -31, -6, -4, -326, -582, -172, -1, 3, 2, 2, 4, -1, -7, 715, -6, 308, 1, -1, 7, -671, -4, 3, -5, -6, 0, -6, -2, -2, 361, 1, 844, -5, -4, 54, 95, 34, 6, -2, -7, -2, -191, -4, 2, 884, 7, 1, -3, -1, -7, -4, 369, -5, 5, 1, -2, -378, 0, 5, 2, 2, -1, -7, 7, 577, 7, 176, -2, -6, 1, -3, 410, 4, 1, -37, 522, -498, -4, 0, 0, 286, 0, -1, -5, -7, 2, 2, 2, -6, -1, -115, 18, 2, 4, 92, -124, -190, 2, 3, 6, 3, -51, 4, 5, 2, 509, -126, -4, -7, 3, 2, 7, -3, 3, -2, -6, 6, 6, 5, -1, -406, -3, 4, 5, 734, 298, 176, 0, -3, 114, 2, -4, 5, -7, -2, 5, 5, 1, -5, 1, -2, -3, 1, -7, 5, -1, -4, -6, -6, 2, -5, 268, 5, 5, 2, 0, 39, 0, 2, 0, 2, -5, -6, 7, -2, 1101, 2, -1, 6, -1, -434, 0, -6, -5, 1, 0, 3, -2, -3, 2, 7, -5, -3, -3, -5, 6, -7, 1, -7, -3, 4, 606, -6, 1, -2, -4, 686, 7, 6, 5, 6, -4, 5, 7, 7, -4, -1, 0, 1, -4, 1, 249, 0, -3, -3, 5, -7, -1, 1, -1, -1, 2, -3, -3, 71, -1, 5, -2, 7, 1, -479, -2, -4, -3, 7, -3, 7, -2, 0, -3, 7, 6, 3, 5, -6, 0, 3, -7, -5, -7, -7, -5, 1, 3, -7, -829, 7, 4, 0, -4, 0, -2, -3, 1, -3, -2, 7, -6, 2, -7, -3, -241, -2, 4, -2, -7, 2, -2, 1, -1, -5, 213, -1, 0, 3, -316, -801, -6, -3, -3, 7, -297, -3, 5, 1, -1, -290, -4, 6, -1, -252, 4, 6, -3, 3, 2, 4, -1, 2, -7, -1, 6, -3, 1, 645, 1, 2, 1, -4, 142, 324, -3, -2, 1, -4, 5, -1, 1, 6, -7, 3, 7, 7, 0, 0, -1, 3, 2, -6, 1, 4, 7, -6, 4, -3, -1, 7, 7, -2, 6, 7, -2, -4, 1, -61, 4, -3, 7, 3, 3, 3, -1, 1, 1, 1, 4, 1, -3, -1, 5, -43, 333, -5, -147, -385, 60, 210, 5, -2, -97, 195, -320, -302, -1, 6, -282, 559, 193, 95, 17, 275, -157, 2, 461, -233, 41, -514, -1, 569, -2, 282, 14, -330, -761, 402, -118, 210, 7, 240, -2, -113, -144, 370, -265, 3, 84, -445, -78, -468, 309, -371, 110, 1, 43, 197, -628, -260, 0, 7, 1, 546, 506, -7, 7, 38, -557, -42, 5, 7, 319, 32, 656, -6, 489, -4, 427, -207, 106, -1039, 239, -214, -45, 3, 107, 310, 62, -575, 4, 618, 3, -191, -394, 0, 561, 96, -288, -182, 1, -30, 393, 78, -119, -1, 1, 0, -484, 262, -178, -241, -255, 77, 209, -6, -427, -283, 320, 881, -3, -6, 3, -532, 293, 6, 5, 67, 109, -337, 2, -454, -148, -125, -465, -23, -445, 1, -69, -130, -124, -657, -409, 256, -218, 524, -229, -348, 37, 203, -4, 1, 4, 6, -73, -6, -5, 110, -490, 394, -6, 301, -7, 179, -252, 222, 6, -7, -51, 272, -42, 581, 187, 85, -112, 118, -74, 53, 265, 196, -3, -7, -4, 2, 3, -2, 7, -1, 3, 0, -2, 0, -5, 0, 6, 6, 2, 0, -5, 0, 2, 6, 0, 6, 1, -3, 5, -2, 3, -2, 4, -3, 2, 6, -1, -3, 5, -5, 5, 0, 4, -2, 7, 5, -5, 7, 3, 1, -3, 6, -4, -5, -2, 2, 7, 3, -3, -2, 3, 7, -3, 0, -59, -2, -7, -2, 1, -2, 3, -5, 0, 5, 6, 6, 5, 6, 3, -1, 18, -57, 325, -7, -14, 68, 243, 4, 255, -11, -177, -63, -7, 2, -6, 355, -33, -169, 402, 57, 201, -131, -208, 17, -167, -41, -281, 556, 890, 5, -79, 192, 1, 319, -225, 144, -569, 233, -406, -527, 502, 222, 223, -49, -6, 14, 94, -64, -355, -543, -42, 85, 53, 47, 65, -230, 142, -4, -522, -6, 203, 436, 271, 33, 478, -148, 639, -1, 460, -114, 50, 47, 290, 110, -6, -37, 490, 9, -355, 192, -26, -204, -331, 418, -215, -238, -209, 5, -1, 0, 126, -45, 3, -315, 434, -212, 1035, 2, -5, 345, 333, -387, 0, -240, 3, -46, 2, 201, 241, 213, -11, -298, -733, 236, 462, 386, -284, 58, -7, -3, -332, 194, -548, -448, 149, 4, -54, 590, 178, 161, 99, -390, -95, -630, -2, -254, -129, -316, 174, 14, 162, -6, 21, -129, 90, 110, 151, 4, 4, -3, -166, 348, -85, 354, -124, -60, -19, 16, -86, 270, 98, -82, 523, -1, 0, 575, -342, -270, -297, 210, 236, -398, 151, 179, 157, -59, 255, 6, 2, 0, 5, 3, 1, -2, -5, -6, 6, 5, -6, 4, 5, 1, -2, 1, -4, -7, 0, 0, 0, -5, 6, 6, 3, -5, -1, 6, -2, -7, -1, 1, -4, 6, 2, -5, -5, -2, 0, 0, 0, 1, 3, 2, 3, -6, -5, -3, -4, -2, -7, 1, -3, 6, 2, 0, 1, 2, -4, -7, 4, 7, -1, 4, -322, -6, -865, 801, 61, 4, -6, -246, -242, -7, -7, 3, -5, 2, -6, -2, 4, -3, 3, 383, 5, -5, -7, -5, -3, 2, -7, -70, -5, 7, -2, 1, 5, 4, 2, 3, 3, -5, 6, -3 };
	CBoard board;
	board.Initialize();
	board.SetWeight( weight );
	CMinMaxBot bot;
	bot.SetMaxDepth( 9 );
	bot.SetDisplayMode( CMinMaxBot::DM_NONE );
	MOVE move;
	vector<MOVE> seq;
	int score;

	move = bot.GetNext( &board, &seq, &score );
	printf( "%s %08x %d\n", board.ToString().c_str(), move.ToInt(), score );

	board.Display();
	printf( "%s\n", move.ToString(&board).c_str() );

	MOVE m[CBoard::MAXMOVE];
	int mn = board.GetMove( m );

	for ( int i=0; i<mn; i++ )
	{
		board.Move( m[i] );

		move = bot.GetNext( &board, &seq, &score );
		printf( "%s %08x %d\n", board.ToString().c_str(), move.ToInt(), score );

		board.Display();
		printf( "%s\n", move.ToString(&board).c_str() );

		board.Undo();
	}
}

