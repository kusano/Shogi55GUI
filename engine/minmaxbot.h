
#pragma once



#include	"config.h"
#include	<time.h>
#include	<vector>
#include	"board.h"



using namespace std;



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

	MOVE		BestMove;				//	最善手
	int			BestScore;				//	最善手の評価値
	MOVE		KillerMove[MAXDEPTH];	//	キラー手
	vector<MOVE>BestSequence[MAXDEPTH];	//	深さごとの読み筋
	bool		NullMoveSearch;			//	NullMove探索中

	int			MaxDepth;				//	最大深さ
	int			TimeLimit;				//	制限時間 [ms]　0で無効
	int			ValueDelta;				//	評価関数変化分
	clock_t		StartTime;				//	探索開始時刻
	int			EvaluateCount;			//	盤面評価の回数
	int			DisplayMode;			//	状態の表示方法
	bool		HaltFlag;				//	trueになったら強制終了

	MOVE		GetMinMax( const CBoard *board );
	int			MinMax( CBoard *board, int depth, int maxdepth, int alpha, int beta );
	int			Quiescence( CBoard *board, int depth, int maxdepth, int alpha, int beta );
	int			Evaluate( const CBoard *board );

public:
				CMinMaxBot();
	void		Initialize();
	MOVE		GetNext( const CBoard *board, vector<MOVE> *sequence=NULL, int *score=NULL );
	void		Halt();

	void		SetMaxDepth( int depth );
	void		SetTimeLimit( int time );
	void		SetDisplayMode( DISPLAYMODE mode );
	void		SetValueDelta( int delta );
};
