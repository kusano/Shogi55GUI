
#pragma once



class CBoard;



class CSimpleEvaluator
{
private:
	static int		ValuePiece[22];						//	‹î‰¿’l
	static int		ValueHand[14];						//	‹î‰¿’l

	int				Value;								//	•]‰¿

	void			Initialize();

public:
	void			Update( const CBoard *board );
	void			Put( const CBoard *board, int pos, int piece );
	void			PutHand( const CBoard *board, int piece );
	void			Remove( const CBoard *board, int pos );
	void			RemoveHand( const CBoard *board, int piece );
	int				GetValue( const CBoard *board ) const;
	void			SetValue( int value );
};
