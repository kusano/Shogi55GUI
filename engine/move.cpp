
#include	"move.h"
#include	"board.h"



const MOVE MOVE::null = MOVE( 0, 0, false, 0 );



/*
 *	MOVE
 *		ƒRƒ“ƒXƒgƒ‰ƒNƒ^
 */
MOVE::MOVE()
{
}

MOVE::MOVE( char f, char t, bool p, char h ) : from(f), to(t), promote(p), hand(h)
{
}

MOVE::MOVE( unsigned int n )
{
	FromInt(n);
}



/*
 *	== !=
 *		‰‰ŽZŽq
 */
bool MOVE::operator==( MOVE m ) const
{
	return from == m.from  &&
		   to == m.to  &&
		   promote == m.promote  &&
		   hand == m.hand;
}

bool MOVE::operator!=( MOVE m ) const
{
	return ! ( *this == m );
}



/*
 *	To/FromInt
 *		intŒ^‚É•ÏŠ·
 *		sizeof (MOVE) == 4 ‚ð‰¼’è
 */
unsigned int MOVE::ToInt() const
{
	return *(unsigned int *)this;
}

void MOVE::FromInt( unsigned int m )
{
	*this = *(MOVE *)&m;
}



/*
 *	ToString
 *		•¶Žš—ñ‚É•ÏŠ·
 */
string MOVE::ToString( const CBoard *board ) const
{
	const char *arabic[] = { "‚O", "‚P", "‚Q", "‚R", "‚S", "‚T", "‚U", "‚V", "‚W", "‚X", "10" };
	const char *chinese[] = { "Z", "ˆê", "“ñ", "ŽO", "Žl", "ŒÜ", "˜Z", "Žµ", "”ª", "‹ã", "\" };
	const char *piece[] = { "~", "•à", "‹â", "Šp", "”ò", "‹à", "‹Ê", "‚Æ", "‘S", "”n", "—´" };

	int p = hand == 0 ? board->GetPiece(from) : hand;

	char buf[32];

	sprintf_s( buf, sizeof buf, "%s%s%s%s%s%s",
		board->GetTurn() == 0 ? "£" : "¢",
		arabic[ to % 7],
		chinese[ to / 7 ],
		piece[p/2],
		promote ? "¬" : "",
		hand > 0 ? "‘Å" : "" );

	return buf;
}



/*
 *	FromCSA
 *		CSAŒ`Ž®‚©‚ç•ÏŠ·
 */
bool MOVE::FromCSA( string csa, const CBoard *board )
{
	if ( csa.length() != 7 )
		return false;

	int turn;

	if ( csa[0] == '+' )  turn = 0;
	else if ( csa[0] == '-' )  turn = 1;
	else  return false;

	int fx = csa[1] - '0';
	if ( fx < 0  ||  5 < fx )  return false;
	int fy = csa[2] - '0';
	if ( fy < 0  ||  5 < fy )  return false;
	int tx = csa[3] - '0';
	if ( tx < 0  ||  5 < tx )  return false;
	int ty = csa[4] - '0';
	if ( ty < 0  ||  5 < ty )  return false;

	int piece = -1;
	string name[] = { "XX", "FU", "GI", "KA", "HI", "KI", "OU", "TO", "NG", "UM", "RY", };
	for ( int i=0; i<11; i++ )
		if ( name[i] == csa.c_str() + 5 )
			piece = i * 2 + turn;
	if ( piece == -1 )
		return false;

	MOVE temp;
	temp.from = fy * 7 + fx;
	temp.to = ty * 7 + tx;

	if ( temp.from > 0 )
	{
		temp.hand = 0;
		temp.promote = board->GetPiece(temp.from) != piece;
	}
	else
	{
		temp.hand = piece;
		temp.promote = false;
	}

	if ( board->CheckMove( temp ) )
	{
		*this = temp;
		return true;
	}
	else
	{
		//printf( "!" );
		return false;
	}
}