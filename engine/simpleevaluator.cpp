
#include	"simpleevaluator.h"
#include	"board.h"



using namespace std;



/*
 *	��l
 */
int CSimpleEvaluator::ValuePiece[22] = {				//	��l
	     0, 0,	//	��
	  8000, 0,	//	��
	 50000, 0,	//	��
	 50000, 0,	//	�p
	 60000, 0,	//	��
	 60000, 0,	//	��
	     0, 0,	//	��
	 35000, 0,	//	��
	 20000, 0,	//	�S
	100000, 0,	//	�n
	100000, 0,	//	��
};

int CSimpleEvaluator::ValueHand[14] = {					//	����l
	     0, 0,	//	��
	 20000, 0,	//	��
	 25000, 0,	//	��
	 30000, 0,	//	�p
	 50000, 0,	//	��
	 40000, 0,	//	��
	     0, 0,	//	��
};



/*
 *	Update
 *		board�̕]�����v�Z
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
 *		pos��piece��u��
 */
void CSimpleEvaluator::Put( const CBoard *board, int pos, int piece )
{
	Value += ValuePiece[piece];
}



/*
 *	PutHand
 *		�����piece��u��
 */
void CSimpleEvaluator::PutHand( const CBoard *board, int piece )
{
	Value += ValueHand[piece];
}



/*
 *	Remove
 *		pos�̋����菜��
 */
void CSimpleEvaluator::Remove( const CBoard *board, int pos )
{
	int piece = board->Board[pos];

	Value -= ValuePiece[piece];
}



/*
 *	RemoveHand
 *		�����piece����菜��
 */
void CSimpleEvaluator::RemoveHand( const CBoard *board, int piece )
{
	Value -= ValueHand[piece];
}



/*
 *	GetValue
 *		�]�����擾
 */
int CSimpleEvaluator::GetValue( const CBoard *board ) const
{
	return Value;
}

/*
 *	SetValue
 *		�]����ݒ�
 *		Undo�ł̍������Ɏg�p
 */
void CSimpleEvaluator::SetValue( int value )
{
	Value = value;
}



/*
 *	Initialize
 *		������
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
