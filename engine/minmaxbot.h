
#pragma once



#include	"config.h"
#include	<time.h>
#include	<vector>
#include	"board.h"



using namespace std;



//	探索木のノード
struct NODE
{
	const static int	MAXCHARACTER	= 32;

	BOARD	board;			//	盤面
	int		child[5];		//	子ノード
	int		childnummax;	//	子ノード最大数
	int		childnum;		//	子ノードの数
	int		value;			//	評価値
	bool	current;		//	現在探索中か
	int		x, y;			//	位置
	bool	draw;			//	このノードを描画する
	CHARACTER character[MAXCHARACTER];	//	評価の高い評価要素
	int		characternum;	//	↑の個数
};

//	探索状態
struct STATE
{
	const static int	HASHNUM			= 1024;

	struct HASH
	{
		bool	used;		//	使用されている
		bool	current;	//	今回のハッシュ
		bool	alpha, beta;//	上限・下限
		int		depth;		//	深さ
	};

	NODE	tree[1+5+20+60+120];	//	探索木
	HASH	hash[HASHNUM];			//	ハッシュの状況
};



class CMinMaxBot
{
public:
	enum DISPLAYMODE
	{
		DM_NONE,		//	状態表示無し
		DM_NORMAL,		//	普通
	};

private:
	struct POSITION
	{
		HASH			hash;		//	ハッシュ
		int				value;		//	評価値
		MOVE			move;		//	最善手
		signed char		bound;		//	-1:>value 0:=value 1:<value
		unsigned char	depth;		//	深さ
		unsigned char	maxdepth;	//	最大深さ

		#if SEQUENCE_IN_POSITION_TABLE
		vector<MOVE>	sequence;	//	読み筋
		#endif
	};

	static const int	POSSIZE		= POSITION_TABLE_SIZE;			//	局面表サイズ
	static const int	QPOSSIZE	= QPOSITION_TABLE_SIZE;			//	静止探索局面表サイズ
	static const int	INF			= 0x10000000;		//	無限大
	static const int	MAXDEPTH	= 128;				//	探索する最大深さ

	static POSITION		Position[POSSIZE];			//	局面表
	static POSITION		QPosition[QPOSSIZE];		//	静止探索局面表

	static STATE		State;						//	探索状態

	MOVE		BestMove;				//	最善手
	int			BestScore;				//	最善手の評価値
	MOVE		KillerMove[MAXDEPTH];	//	キラー手
	vector<MOVE>BestSequence[MAXDEPTH];	//	深さごとの読み筋
	bool		NullMoveSearch;			//	NullMove探索中
	int			CurrentDepth;			//	現在の探索深さ

	int			MaxDepth;				//	最大深さ
	int			TimeLimit;				//	制限時間 [ms]　0で無効
	int			ValueDelta;				//	評価関数変化分
	clock_t		StartTime;				//	探索開始時刻
	int			EvaluateCount;			//	盤面評価の回数
	int			DisplayMode;			//	状態の表示方法
	bool		HaltFlag;				//	trueになったら強制終了

	MOVE		GetMinMax( const CBoard *board );
	int			MinMax( CBoard *board, int depth, int maxdepth, int alpha, int beta, int nodeid );
	int			Quiescence( CBoard *board, int depth, int maxdepth, int alpha, int beta );
	int			Evaluate( const CBoard *board );

public:
				CMinMaxBot();
	void		Initialize();
	MOVE		GetNext( const CBoard *board, vector<MOVE> *sequence=NULL, int *score=NULL );
	void		Halt();
	STATE *		GetState() const;

	void		SetMaxDepth( int depth );
	void		SetTimeLimit( int time );
	void		SetDisplayMode( DISPLAYMODE mode );
	void		SetValueDelta( int delta );
};
