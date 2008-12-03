
#include	"simpleevaluator.h"
#include	"board.h"



using namespace std;



/*
 *	駒価値
 */
int CSimpleEvaluator::ValuePiece[22] = {				//	駒価値
	     0, 0,	//	空
	  8000, 0,	//	歩
	 50000, 0,	//	銀
	 50000, 0,	//	角
	 60000, 0,	//	飛
	 60000, 0,	//	金
	     0, 0,	//	玉
	 35000, 0,	//	と
	 20000, 0,	//	全
	100000, 0,	//	馬
	100000, 0,	//	龍
};

int CSimpleEvaluator::ValueHand[14] = {					//	持駒価値
	     0, 0,	//	空
	 20000, 0,	//	歩
	 25000, 0,	//	銀
	 30000, 0,	//	角
	 50000, 0,	//	飛
	 40000, 0,	//	金
	     0, 0,	//	玉
};



/*
 *	Update
 *		boardの評価を計算
 */
void CSimpleEvaluator::Update( const CBoard *board )
{
	Initialize();

	Value = 0;

	for ( int i=0; i<49; i++ )
		Value += ValuePiece[ board->Board[i] ];

	for ( int i=2; i<14; i++ )
		Value += ValueHand[i] * board->Hand[i];
}



/*
 *	Put
 *		posにpieceを置く
 */
void CSimpleEvaluator::Put( const CBoard *board, int pos, int piece )
{
	Value += ValuePiece[piece];
}



/*
 *	PutHand
 *		持駒にpieceを置く
 */
void CSimpleEvaluator::PutHand( const CBoard *board, int piece )
{
	Value += ValueHand[piece];
}



/*
 *	Remove
 *		posの駒を取り除く
 */
void CSimpleEvaluator::Remove( const CBoard *board, int pos )
{
	int piece = board->Board[pos];

	Value -= ValuePiece[piece];
}



/*
 *	RemoveHand
 *		持駒からpieceを取り除く
 */
void CSimpleEvaluator::RemoveHand( const CBoard *board, int piece )
{
	Value -= ValueHand[piece];
}



/*
 *	GetValue
 *		評価を取得
 */
int CSimpleEvaluator::GetValue( const CBoard *board ) const
{
	return Value;
}

/*
 *	SetValue
 *		評価を設定
 *		Undoでの高速化に使用
 */
void CSimpleEvaluator::SetValue( int value )
{
	Value = value;
}



/*
 *	Initialize
 *		初期化
 */
void CSimpleEvaluator::Initialize()
{
	static int initialized = false;

	if ( initialized )
		return;

	for ( int i=1; i<22; i+=2 )
		ValuePiece[i] = -ValuePiece[i-1];

	for ( int i=1; i<14; i+=2 )
		ValueHand[i] = -ValueHand[i-1];

	initialized = true;
}
