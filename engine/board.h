
#pragma once



#include	<vector>
#include	<string>
#include	"move.h"
#include	"simpleevaluator.h"
#include	"evaluator.h"



using namespace std;



typedef unsigned long long int HASH;



//	��
enum PIECE
{
	//	+1�ő����
	//	+PROMOTE�Ő���

	SPACE	=  0,	//	��
	WALL	=  1,	//	��
	FU		=  2,	//	��
	GIN		=  4,	//	��
	KAKU	=  6,	//	�p
	HI		=  8,	//	��
	KIN		= 10,	//	��
	GYOKU	= 12,	//	��
	TO		= 14,	//	��
	NGIN	= 16,	//	�S
	UMA		= 18,	//	�n
	RYU		= 20,	//	��

	PROMOTE	= 12,	//	��
};

//	�������߂���悤�ɋL�^
struct LOG
{
	MOVE	move;
	int		remove;		//	��菜������
	int		value;		//	�]���l
};

//	�Ֆ�
struct BOARD
{
	char	board[5][5];	//	[x][y]
	char	hand[6][2];		//	[piece][b/w]
	int		step;
	int		turn;
};



/*
 *	��
 */
class CBoard
{
	friend CSimpleEvaluator;
	friend CEvaluator;

public:
	static const int MAXMOVE	= 1024;					//	�ő咅��\�萔�i593�����ǂ�����Ɨ]�T�������āj
	static const int ELEMNUM	= CEvaluator::ELEMNUM;	//	�]���֐��̗v�f��
	static const int REPETCOUNT	= 0x10000;				//	�����΍��p�e�[�u���T�C�Y

	enum
	{
		MV_CAPTURE		= 1,							//	�������̂ݗL��
	};

private:
	static int		PieceMoveNear[30][16];				//	��̓����i�P�}�X�j
	static int		PieceMoveFar[30][16];				//	��̓����i���j
	static int		*PieceMoveToEffectNear;				//	�Y���̈ړ��ɑΉ���������̃r�b�g�i�P�}�X�j
	static int		PieceMoveToEffectNearTemp[21];		//	
	static int		*PieceMoveToEffectFar;				//	�Y���̈ړ��ɑΉ���������̃r�b�g�i���j
	static int		PieceMoveToEffectFarTemp[21];		//	

	static HASH		HashBoard[49][32];					//	�Ֆʃn�b�V��
	static HASH		HashHand[14][32];					//	������n�b�V��
	static HASH		HashTurn;							//	��ԃn�b�V��
	
	char			Board[49];							//	�Ֆ�
	char			Hand[14];							//	������
	int				Turn;								//	���
	int				Step;								//	�萔

	vector<LOG>		Log;								//	����܂łɎw������
	HASH			Hash;								//	�n�b�V��
	CEvaluator		Evaluator;							//	�]���֐�

	bool			Fu[7][2];							//	���̗L��
	int				GyokuPosition[2];					//	�ʂ̈ʒu
	int				Effect[49][2];						//	�����@���ʂ��� -8n -7n -6n -1n +1n +6n +7n +8n -8 -7 -6 -1 +1 +6 +7 +8 �̕�������̌���������
	char			RepetitionCount[REPETCOUNT];		//	�o���񐔂𐔂��Ă���

	void			GlobalInitialize();
	void			Update();
	bool			CheckMoveCore( MOVE move, int flag ) const;
	void			Put( int pos, int piece );
	void			PutNoValue( int pos, int piece );
	void			PutHand( int piece );
	void			PutHandNoValue( int piece );
	void			Remove( int pos );
	void			RemoveNoValue( int pos );
	void			RemoveHand( int piece );
	void			RemoveHandNoValue( int piece );
	bool			IsFriend( int p ) const;
	bool			IsEnemy( int p ) const;

public:
	void			Initialize();

	string			ToString() const;
	bool			FromString( string s );

	int				GetMove( MOVE move[MAXMOVE], int flag=0 ) const;
	bool			CheckMove( MOVE move, int flag=0 ) const;
	void			Move( MOVE move );
	void			MoveNull();
	void			Undo();
	void			LogClear();

	int				GetPiece( int pos ) const;
	int				GetPiece( int x, int y ) const;
	int				GetHand( int hand ) const;
	int				GetTurn() const;
	int				GetTurnSign() const;
	int				GetStep() const;
	bool			IsFinished() const;
	bool			IsFinished2() const;
	bool			IsEffected( int pos, int turn, bool far=false ) const;
	bool			IsCheckmated( int turn ) const;
	bool			IsCheckedGyoku( int turn ) const;
	int				GetValue() const;
	HASH			GetHash() const;
	void			GetBoard( BOARD *board ) const;
	void			SetBoard( const BOARD *board );

	void			SetWeight( const int weight[ELEMNUM] );
	void			GetElement( int element[ELEMNUM] );
	int				GetCharacter( int threshold, CHARACTER character[], int maxnumber ) const;

	void			Display() const;
	void			DisplayEffect() const;
};
