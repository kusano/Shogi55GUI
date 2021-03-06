
#pragma once



#include	<string>



using namespace std;



class CBoard;



//	動き
struct MOVE
{
	char	from;	//	移動元
	char	to;		//	移動先
	bool	promote;//	成
	char	hand;	//	駒打ちであれば駒番号

	const static MOVE null;		//	無効を示す

			MOVE();
			MOVE( char f, char t, bool p, char h );
			MOVE( unsigned int n );

	bool	operator==( MOVE m ) const;
	bool	operator!=( MOVE m ) const;

	unsigned int ToInt() const;				//	sizeof (MOVE) == 4 を仮定
	void	FromInt( unsigned int m );		//	sizeof (MOVE) == 4 を仮定
	string	ToString( const CBoard *board ) const;
	bool	FromCSA( string csa, const CBoard *board );
};