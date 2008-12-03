
#pragma once



#include	<string>



using namespace std;



class CBoard;



//	“®‚«
struct MOVE
{
	char	from;	//	ˆÚ“®Œ³
	char	to;		//	ˆÚ“®æ
	bool	promote;//	¬
	char	hand;	//	‹î‘Å‚¿‚Å‚ ‚ê‚Î‹î”Ô†

	const static MOVE null;		//	–³Œø‚ğ¦‚·

			MOVE();
			MOVE( char f, char t, bool p, char h );
			MOVE( unsigned int n );

	bool	operator==( MOVE m ) const;
	bool	operator!=( MOVE m ) const;

	unsigned int ToInt() const;				//	sizeof (MOVE) == 4 ‚ğ‰¼’è
	void	FromInt( unsigned int m );		//	sizeof (MOVE) == 4 ‚ğ‰¼’è
	string	ToString( const CBoard *board ) const;
	bool	FromCSA( string csa, const CBoard *board );
};