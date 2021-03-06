
#include	"simpleevaluator.h"
#include	"board.h"



using namespace std;



/*
 *	î¿l
 */
int CSimpleEvaluator::ValuePiece[22] = {				//	î¿l
	     0, 0,	//	ó
	  8000, 0,	//	à
	 50000, 0,	//	â
	 50000, 0,	//	p
	 60000, 0,	//	ò
	 60000, 0,	//	à
	     0, 0,	//	Ê
	 35000, 0,	//	Æ
	 20000, 0,	//	S
	100000, 0,	//	n
	100000, 0,	//	´
};

int CSimpleEvaluator::ValueHand[14] = {					//	î¿l
	     0, 0,	//	ó
	 20000, 0,	//	à
	 25000, 0,	//	â
	 30000, 0,	//	p
	 50000, 0,	//	ò
	 40000, 0,	//	à
	     0, 0,	//	Ê
};



/*
 *	Update
 *		boardÌ]¿ðvZ
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
 *		posÉpieceðu­
 */
void CSimpleEvaluator::Put( const CBoard *board, int pos, int piece )
{
	Value += ValuePiece[piece];
}



/*
 *	PutHand
 *		îÉpieceðu­
 */
void CSimpleEvaluator::PutHand( const CBoard *board, int piece )
{
	Value += ValueHand[piece];
}



/*
 *	Remove
 *		posÌîðæè­
 */
void CSimpleEvaluator::Remove( const CBoard *board, int pos )
{
	int piece = board->Board[pos];

	Value -= ValuePiece[piece];
}



/*
 *	RemoveHand
 *		î©çpieceðæè­
 */
void CSimpleEvaluator::RemoveHand( const CBoard *board, int piece )
{
	Value -= ValueHand[piece];
}



/*
 *	GetValue
 *		]¿ðæ¾
 */
int CSimpleEvaluator::GetValue( const CBoard *board ) const
{
	return Value;
}

/*
 *	SetValue
 *		]¿ðÝè
 *		UndoÅÌ¬»Égp
 */
void CSimpleEvaluator::SetValue( int value )
{
	Value = value;
}



/*
 *	Initialize
 *		ú»
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
