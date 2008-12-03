
#include	"evaluator.h"
#include	<algorithm>
#include	"board.h"
#include	"mt19937ar.h"



using namespace std;



bool CEvaluator::Initialized = false;



int CEvaluator::ValuePiece[22];						//	駒価値
int CEvaluator::ValueHand[14][2];					//	持駒価値
int CEvaluator::ValuePosition[22][49];				//	位置価値
int CEvaluator::ValueRelative[22][22][32];			//	相対位置価値
int CEvaluator::Near[49][64];						//	近傍
int CEvaluator::PositionToDiff[49][64];				//	位置をValueRelativeの添字に変換



/*
 *	SetWeight
 *		評価関数の重みを設定
 *		   0 -     9　駒価値
 *		  10 -    21　持駒価値（１枚目の価値、２枚目の価値…８枚目以上の価値）
 *		  22 -   271　位置価値
 *		 272 -      　相対位置価値
 */
void CEvaluator::SetWeight( const CBoard *board, const int weight[ELEMNUM] )
{
	memset( ValuePiece, 0, sizeof ValuePiece );
	memset( ValueHand, 0, sizeof ValueHand );
	memset( ValuePosition, 0, sizeof ValuePosition );
	memset( ValueRelative, 0, sizeof ValueRelative );

	for ( int i=0; i<10; i++ )
		ValuePiece[i*2+2] = weight[i],
		ValuePiece[i*2+3] = -weight[i];
	
	for ( int i=0; i<6; i++ )
	for ( int j=0; j<2; j++ )
		ValueHand[i*2+2][j] = weight[10+i*2+j],
		ValueHand[i*2+3][j] = -weight[10+i*2+j];

	for ( int i=0; i<10; i++ )
	for ( int y=0; y<5; y++ )
	for ( int x=0; x<5; x++ )
		ValuePosition[i*2+2][(y+1)*7+(x+1)] = weight[22+i*49+y*5+x],
		ValuePosition[i*2+3][49-1-(y+1)*7-(x+1)] = -weight[22+i*49+y*5+x];

	const static int piece[11][22] = {
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1,  1,  2,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1,  4,  5,  6,  7,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1,  9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, 16, 17, 18, 19, 20, 21, 22, 23, 24, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, -1, -1, -1, -1, -1 },
		{ -1, -1, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, -1, -1, -1 },
		{ -1, -1, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, -1 },
	};

	for ( int i=2; i<22; i+=2 )
	for ( int j=2; j<=i; j++ )
	{
		for ( int x=0; x<NW+1; x++ )
		for ( int y=0; y<2*NH+1; y++ )
			ValueRelative[i][j][x*(2*NH+1)+y] = weight[272+piece[i/2][j]*(NW+1)*(2*NH+1)+x*(2*NH+1)+y];
	}

	//	同駒は上下対称
	for ( int i=2; i<22; i+=2 )
	{
		for ( int x=0; x<NW+1; x++ )
		for ( int y=0; y<NH; y++ )
			ValueRelative[i][i][x*(2*NH+1)+y] = ValueRelative[i][i][x*(2*NH+1)+2*NH-y];
	}

	//	敵駒
	for ( int i=3; i<22; i+=2 )
	for ( int j=2; j<=i; j++ )
	{
		for ( int x=0; x<NW+1; x++ )
		for ( int y=0; y<2*NH+1; y++ )
			ValueRelative[i][j][x*(2*NH+1)+y] = -ValueRelative[i^1][j^1][x*(2*NH+1)+2*NH-y];
	}

	//	残り
	for ( int i=2; i<22; i++ )
	for ( int j=i+1; j<22; j++ )
	{
		for ( int x=0; x<NW+1; x++ )
		for ( int y=0; y<2*NH+1; y++ )
			ValueRelative[i][j][x*(2*NH+1)+y] = ValueRelative[j][i][x*(2*NH+1)+2*NH-y];
	}
	
	Update( board );
}



/*
 *	Update
 *		boardの評価を計算
 */
void CEvaluator::Update( const CBoard *board )
{
	Initialize();

	Value = 0;

	for ( int i=0; i<49; i++ )
	{
		Value += ValuePiece[ board->Board[i] ];
		Value += ValuePosition[ board->Board[i] ][i];
	}

	for ( int i=2; i<14; i++ )
	for ( int j=0; j<2; j++ )
		if ( board->Hand[i] > j )
			Value += ValueHand[i][j];

	for ( int f=0; f<49; f++ )  if ( board->Board[f] >= FU )
	for ( int *t=Near[f]; *t>=0; t++ )  if ( *t >= f  &&  board->Board[*t] >= FU )
		Value += GetRelativeValue( board, f, *t );
}



/*
 *	Put
 *		posにpieceを置く
 */
void CEvaluator::Put( const CBoard *board, int pos, int piece )
{
	Value += ValuePiece[piece];
	Value += ValuePosition[piece][pos];

	int (*relative)[32] = ValueRelative[board->Board[pos]];
	int *diff = PositionToDiff[pos];
	for ( int *t=Near[pos]; *t>=0; t++ )
		Value += relative[board->Board[*t]][diff[*t]];
}



/*
 *	PutHand
 *		持駒にpieceを置く
 */
void CEvaluator::PutHand( const CBoard *board, int piece )
{
	if ( board->Hand[piece] <= 2 )
		Value += ValueHand[piece][ board->Hand[piece] - 1];
}



/*
 *	Remove
 *		posの駒を取り除く
 */
void CEvaluator::Remove( const CBoard *board, int pos )
{
	int piece = board->Board[pos];

	Value -= ValuePiece[piece];
	Value -= ValuePosition[piece][pos];

	int (*relative)[32] = ValueRelative[board->Board[pos]];
	int *diff = PositionToDiff[pos];
	for ( int *t=Near[pos]; *t>=0; t++ )
		Value -= relative[board->Board[*t]][diff[*t]];
}



/*
 *	RemoveHand
 *		持駒からpieceを取り除く
 */
void CEvaluator::RemoveHand( const CBoard *board, int piece )
{
	if ( board->Hand[piece] <= 2 )
		Value -= ValueHand[piece][ board->Hand[piece] - 1];
}



/*
 *	GetValue
 *		評価を取得
 */
int CEvaluator::GetValue( const CBoard *board ) const
{
	return Value;
}

/*
 *	SetValue
 *		評価を設定
 *		Undoでの高速化に使用
 */
void CEvaluator::SetValue( int value )
{
	Value = value;
}



/*
 *	GetElement
 *		評価関数の算出に使う要素を取得
 *		   0 -     9　駒価値
 *		  10 -    21　持駒価値（１枚目の価値、２枚目の価値…８枚目以上の価値）
 *		  22 -   271　位置価値
 *		 272 -      　相対位置価値
 */
void CEvaluator::GetElement( const CBoard *board, int element[ELEMNUM] ) const
{
	for ( int i=0; i<ELEMNUM; i++ )
		element[i] = 0;

	//	駒価値
	for ( int i=0; i<49; i++ )
		if ( board->Board[i] >= FU )
			element[board->Board[i]/2-1] += 1 - board->Board[i]%2*2;

	//	持駒価値
	for ( int i=0; i<6; i++ )
	for ( int j=0; j<2; j++ )
	{
		if ( board->Hand[i*2+2] > j )
			element[10+i*2+j]++;
		if ( board->Hand[i*2+3] > j )
			element[10+i*2+j]--;
	}

	//	位置価値
	for ( int i=0; i<49; i++ )
	{
		int x = i % 7 - 1;
		int y = i / 7 - 1;

		if ( board->Board[i] >= FU  &&
			 ( board->Board[i] & 1 ) == 0 )
			element[22+(board->Board[i]/2-1)*25+y*5+x]++;

		if ( board->Board[49-1-i] >= FU  &&
			 ( board->Board[49-1-i] & 1 ) == 1 )
			element[22+(board->Board[49-1-i]/2-1)*25+y*5+x]--;
	}

	//	相対位置価値
	const static int piece[11][22] = {
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1,  1,  2,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1,  4,  5,  6,  7,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1,  9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, 16, 17, 18, 19, 20, 21, 22, 23, 24, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, -1, -1, -1, -1, -1 },
		{ -1, -1, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, -1, -1, -1 },
		{ -1, -1, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, -1 },
	};

	for ( int f=0; f<49; f++ )  if ( board->Board[f] >= FU )
	for ( int t=f; t<49; t++ )  if ( board->Board[t] >= FU )
	{
		int dx = abs( t%7 - f%7 );
		int dy = t/7 - f/7;

		if (               NW < dx  ||
			 dy < -NH  ||  NH < dy )
			continue;

		int p1 = board->Board[f];
		int p2 = board->Board[t];

		//	p1 >= p2 とする
		if ( p1 < p2 )
			swap( p1, p2 ),
			dy *= -1;

		//	同種の敵駒ならば打ち消し合う
		if ( ( p1 ^ 1 ) == p2 )
			continue;

		//	同種の駒はdy>0でカウント
		if ( p1 == p2 )
			if ( ( p1 & 1 ) == 0 )  dy = abs(dy);
			else					dy = -abs(dy);

		if ( ( p1 & 1 ) == 0 )
			element[272+piece[p1/2][p2]*(NW+1)*(2*NH+1)+dx*(2*NH+1)+(NH+dy)]++;
		else
			element[272+piece[p1/2][p2^1]*(NW+1)*(2*NH+1)+dx*(2*NH+1)+(NH-dy)]--;
	}
}



/*
 *	Initialize
 *		初期化
 */
void CEvaluator::Initialize()
{
	if ( Initialized )
		return;

	//	Near
	for ( int i=0; i<49; i++ )
	for ( int j=0; j<64; j++ )
		Near[i][j] = -1;

	for ( int i=0; i<49; i++ )
	{
		int cx = i % 7;
		int cy = i / 7;

		if ( cx == 0  ||  cx == 6  ||
			 cy == 0  ||  cy == 6 )
			continue;

		int n = 0;

		for ( int dy=-NH; dy<=NH; dy++ )
		for ( int dx=-NW; dx<=NW; dx++ )
		{
			int x = cx + dx;
			int y = cy + dy;

			if ( 1 <= x  &&  x <= 5  &&
				 1 <= y  &&  y <= 5 )
				Near[i][n++] = y*7 + x;
		}
	}

	//	PositionToDiff
	for ( int i=0; i<49; i++ )
	for ( int j=0; j<49; j++ )
	{
		PositionToDiff[i][j] = abs(j%7-i%7)*(2*NH+1) + j/7-i/7+NH;
	}

	Initialized = true;
}



/*
 *	GetRelativeValue
 *		pos1とpos2の駒の関係の価値を返す
 */
int CEvaluator::GetRelativeValue( const CBoard *board, int pos1, int pos2 ) const
{
	int p1 = board->Board[pos1];
	int p2 = board->Board[pos2];

	return ValueRelative[p1][p2][PositionToDiff[pos1][pos2]];
}