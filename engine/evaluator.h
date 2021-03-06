
#pragma once



class CBoard;



//	]ΏΦΜvf
struct CHARACTER
{
	char	x1, y1, x2, y2;
	int		value;
};



class CEvaluator
{
public:
	static const int NW			= 2;					//	ΞΏlπvZ·ι
	static const int NH			= 2;					//	ΞΏlπvZ·ι³
	static const int ELEMNUM	= 10 + 6*2 + 10*25 + 100*(NW+1)*(2*NH+1);	//	]ΏvfΜΒ

private:
	static bool		Initialized;
	static int		ValuePiece[22];						//	ξΏl
	static int		ValueHand[14][2];					//	ξΏl
	static int		ValuePosition[22][49];				//	ΚuΏl
	static int		ValueRelative[22][22][32];			//	ΞΚuΏl
	static int		Near[49][64];						//	ίT
	static int		PositionToDiff[49][64];			//	ΚuπValueRelativeΜYΙΟ·

	int				Value;								//	]Ώ

	void			Initialize();
	int				GetRelativeValue( const CBoard *board, int pos1, int pos2 ) const;

public:
	void			SetWeight( const CBoard *board, const int weight[ELEMNUM] );
	void			Update( const CBoard *board );
	void			Put( const CBoard *board, int pos, int piece );
	void			PutHand( const CBoard *board, int piece );
	void			Remove( const CBoard *board, int pos );
	void			RemoveHand( const CBoard *board, int piece );
	int				GetValue( const CBoard *board ) const;
	void			SetValue( int value );
	void			GetElement( const CBoard *board, int element[ELEMNUM] ) const;
	int				GetCharacter( const CBoard *board, int threshold, CHARACTER character[], int maxnumber ) const;
};
