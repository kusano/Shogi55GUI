
#pragma once



class CBoard;



class CEvaluator
{
public:
	static const int NW			= 2;					//	相対価値を計算する幅
	static const int NH			= 2;					//	相対価値を計算する高さ
	static const int ELEMNUM	= 10 + 6*2 + 10*25 + 100*(NW+1)*(2*NH+1);	//	評価要素の個数

private:
	static bool		Initialized;
	static int		ValuePiece[22];						//	駒価値
	static int		ValueHand[14][2];					//	持駒価値
	static int		ValuePosition[22][49];				//	位置価値
	static int		ValueRelative[22][22][32];			//	相対位置価値
	static int		Near[49][64];						//	近傍
	static int		PositionToDiff[49][64];			//	位置をValueRelativeの添字に変換

	int				Value;								//	評価

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
