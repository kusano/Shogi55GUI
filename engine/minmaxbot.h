
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
		DM_NONE,		//	��ԕ\������
		DM_NORMAL,		//	����
	};

private:
	struct POSITION
	{
		HASH			hash;		//	�n�b�V��
		int				value;		//	�]���l
		MOVE			move;		//	�őP��
		signed char		bound;		//	-1:>value 0:=value 1:<value
		unsigned char	depth;		//	�[��
		unsigned char	maxdepth;	//	�ő�[��

		#if SEQUENCE_IN_POSITION_TABLE
		vector<MOVE>	sequence;	//	�ǂ݋�
		#endif
	};

	static const int	POSSIZE		= POSITION_TABLE_SIZE;			//	�ǖʕ\�T�C�Y
	static const int	QPOSSIZE	= QPOSITION_TABLE_SIZE;			//	�Î~�T���ǖʕ\�T�C�Y
	static const int	INF			= 0x10000000;		//	������
	static const int	MAXDEPTH	= 128;				//	�T������ő�[��

	static POSITION		Position[POSSIZE];			//	�ǖʕ\
	static POSITION		QPosition[QPOSSIZE];		//	�Î~�T���ǖʕ\

	MOVE		BestMove;				//	�őP��
	int			BestScore;				//	�őP��̕]���l
	MOVE		KillerMove[MAXDEPTH];	//	�L���[��
	vector<MOVE>BestSequence[MAXDEPTH];	//	�[�����Ƃ̓ǂ݋�
	bool		NullMoveSearch;			//	NullMove�T����

	int			MaxDepth;				//	�ő�[��
	int			TimeLimit;				//	�������� [ms]�@0�Ŗ���
	int			ValueDelta;				//	�]���֐��ω���
	clock_t		StartTime;				//	�T���J�n����
	int			EvaluateCount;			//	�Ֆʕ]���̉�
	int			DisplayMode;			//	��Ԃ̕\�����@
	bool		HaltFlag;				//	true�ɂȂ����狭���I��

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
