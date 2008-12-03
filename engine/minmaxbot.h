
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
		DM_NONE,		//	óÔ\¦³µ
		DM_NORMAL,		//	Ê
	};

private:
	struct POSITION
	{
		HASH			hash;		//	nbV
		int				value;		//	]¿l
		MOVE			move;		//	ÅPè
		signed char		bound;		//	-1:>value 0:=value 1:<value
		unsigned char	depth;		//	[³
		unsigned char	maxdepth;	//	Åå[³

		#if SEQUENCE_IN_POSITION_TABLE
		vector<MOVE>	sequence;	//	ÇÝØ
		#endif
	};

	static const int	POSSIZE		= POSITION_TABLE_SIZE;			//	ÇÊ\TCY
	static const int	QPOSSIZE	= QPOSITION_TABLE_SIZE;			//	Ã~TõÇÊ\TCY
	static const int	INF			= 0x10000000;		//	³Àå
	static const int	MAXDEPTH	= 128;				//	Tõ·éÅå[³

	static POSITION		Position[POSSIZE];			//	ÇÊ\
	static POSITION		QPosition[QPOSSIZE];		//	Ã~TõÇÊ\

	MOVE		BestMove;				//	ÅPè
	int			BestScore;				//	ÅPèÌ]¿l
	MOVE		KillerMove[MAXDEPTH];	//	L[è
	vector<MOVE>BestSequence[MAXDEPTH];	//	[³²ÆÌÇÝØ
	bool		NullMoveSearch;			//	NullMoveTõ

	int			MaxDepth;				//	Åå[³
	int			TimeLimit;				//	§ÀÔ [ms]@0Å³ø
	int			ValueDelta;				//	]¿ÖÏ»ª

	clock_t		StartTime;				//	TõJn
	int			EvaluateCount;			//	ÕÊ]¿Ìñ
	int			DisplayMode;			//	óÔÌ\¦û@

	MOVE		GetMinMax( const CBoard *board );
	int			MinMax( CBoard *board, int depth, int maxdepth, int alpha, int beta );
	int			Quiescence( CBoard *board, int depth, int maxdepth, int alpha, int beta );
	int			Evaluate( const CBoard *board );

public:
				CMinMaxBot();
	void		Initialize();
	MOVE		GetNext( const CBoard *board, vector<MOVE> *sequence=NULL, int *score=NULL );

	void		SetMaxDepth( int depth );
	void		SetTimeLimit( int time );
	void		SetDisplayMode( DISPLAYMODE mode );
	void		SetValueDelta( int delta );
};
