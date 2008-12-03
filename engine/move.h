
#pragma once



#include	<string>



using namespace std;



class CBoard;



//	����
struct MOVE
{
	char	from;	//	�ړ���
	char	to;		//	�ړ���
	bool	promote;//	��
	char	hand;	//	��ł��ł���΋�ԍ�

	const static MOVE null;		//	����������

			MOVE();
			MOVE( char f, char t, bool p, char h );
			MOVE( unsigned int n );

	bool	operator==( MOVE m ) const;
	bool	operator!=( MOVE m ) const;

	unsigned int ToInt() const;				//	sizeof (MOVE) == 4 ������
	void	FromInt( unsigned int m );		//	sizeof (MOVE) == 4 ������
	string	ToString( const CBoard *board ) const;
	bool	FromCSA( string csa, const CBoard *board );
};