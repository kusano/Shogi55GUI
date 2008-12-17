
#pragma once



#include	<vector>
#include	<string>
#include	"move.h"
#include	"simpleevaluator.h"
#include	"evaluator.h"



using namespace std;



typedef unsigned long long int HASH;



//	駒
enum PIECE
{
	//	+1で相手駒
	//	+PROMOTEで成り

	SPACE	=  0,	//	空
	WALL	=  1,	//	壁
	FU		=  2,	//	歩
	GIN		=  4,	//	銀
	KAKU	=  6,	//	角
	HI		=  8,	//	飛
	KIN		= 10,	//	金
	GYOKU	= 12,	//	玉
	TO		= 14,	//	と
	NGIN	= 16,	//	全
	UMA		= 18,	//	馬
	RYU		= 20,	//	龍

	PROMOTE	= 12,	//	成
};

//	差し手を戻せるように記録
struct LOG
{
	MOVE	move;
	int		remove;		//	取り除いた駒
	int		value;		//	評価値
};

//	盤面
struct BOARD
{
	char	board[5][5];	//	[x][y]
	char	hand[6][2];		//	[piece][b/w]
	int		step;
	int		turn;
};



/*
 *	盤
 */
class CBoard
{
	friend CSimpleEvaluator;
	friend CEvaluator;

public:
	static const int MAXMOVE	= 1024;					//	最大着手可能手数（593だけどちょっと余裕を持って）
	static const int ELEMNUM	= CEvaluator::ELEMNUM;	//	評価関数の要素数
	static const int REPETCOUNT	= 0x10000;				//	千日手対策用テーブルサイズ

	enum
	{
		MV_CAPTURE		= 1,							//	駒を取る手のみ有効
	};

private:
	static int		PieceMoveNear[30][16];				//	駒の動き（１マス）
	static int		PieceMoveFar[30][16];				//	駒の動き（飛駒）
	static int		*PieceMoveToEffectNear;				//	添字の移動に対応する効きのビット（１マス）
	static int		PieceMoveToEffectNearTemp[21];		//	
	static int		*PieceMoveToEffectFar;				//	添字の移動に対応する効きのビット（飛駒）
	static int		PieceMoveToEffectFarTemp[21];		//	

	static HASH		HashBoard[49][32];					//	盤面ハッシュ
	static HASH		HashHand[14][32];					//	持ち駒ハッシュ
	static HASH		HashTurn;							//	手番ハッシュ
	
	char			Board[49];							//	盤面
	char			Hand[14];							//	持ち駒
	int				Turn;								//	手番
	int				Step;								//	手数

	vector<LOG>		Log;								//	これまでに指した手
	HASH			Hash;								//	ハッシュ
	CEvaluator		Evaluator;							//	評価関数

	bool			Fu[7][2];							//	歩の有無
	int				GyokuPosition[2];					//	玉の位置
	int				Effect[49][2];						//	効き　下位から -8n -7n -6n -1n +1n +6n +7n +8n -8 -7 -6 -1 +1 +6 +7 +8 の方向からの効きがある
	char			RepetitionCount[REPETCOUNT];		//	出現回数を数えておく

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
