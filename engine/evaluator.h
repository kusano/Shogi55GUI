
#pragma once



class CBoard;



class CEvaluator
{
public:
	static const int NW			= 2;					//	���Ή��l���v�Z���镝
	static const int NH			= 2;					//	���Ή��l���v�Z���鍂��
	static const int ELEMNUM	= 10 + 6*2 + 10*25 + 100*(NW+1)*(2*NH+1);	//	�]���v�f�̌�

private:
	static bool		Initialized;
	static int		ValuePiece[22];						//	��l
	static int		ValueHand[14][2];					//	����l
	static int		ValuePosition[22][49];				//	�ʒu���l
	static int		ValueRelative[22][22][32];			//	���Έʒu���l
	static int		Near[49][64];						//	�ߖT
	static int		PositionToDiff[49][64];			//	�ʒu��ValueRelative�̓Y���ɕϊ�

	int				Value;								//	�]��

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
};
