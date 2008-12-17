// Shogi55GUIView.h : CShogi55GUIView �N���X�̃C���^�[�t�F�C�X
//


#pragma once


#include "engine/minmaxbot.h"
#include "statedialog.h"
#include "CSADialog.h"
#include "PieceSelectDialog.h"



class CShogi55GUIView : public CView
{
	friend UINT SearchMove( CShogi55GUIView *view );
	friend UINT PreSearchMove( CShogi55GUIView *view );

public:
	enum MODE
	{
		M_HUMANTURN,
		M_BOTTUNRN,
		M_EDIT
	};

private:

	static const int	PW		= 44;		//	�
	static const int	PH		= 48;		//	�

	static const int	TIMER_SEARCH	= 100;	//	�T�����I���������`�F�b�N
	static const int	TIMER_PRESEARCH	= 101;	//	�O�T�����I���������`�F�b�N
	static const int	TIMER_TIMER		= 102;	//	���v��i�߂�
	static const int	TIMER_STATE		= 103;	//	�T����ԕ\���p

	struct LOG
	{
		int		remain[2];
		int		lastmovefrom;
		int		lastmoveto;
	};

	CBoard		Board;
	CMinMaxBot	Bot;
	MOVE		BestMove;

	Image		ImageBackground;
	Image		ImageBoard;
	Image		ImagePiece;
	Image		ImageSelect;
	Image		ImageSearch;
	Image		ImageTimer;

	CStateDialog	StateDialog;
	CCSADialog		CSADialog;
	CPieceSelectDialog	PieceSelectDialog;

	int			PlayerType[2];		//	0 CPU	1 �l
	int			TimeLimit;

	MODE		Mode;
	int			SelectPosition;
	int			LastMoveFrom;
	int			LastMoveTo;
	int			StartTime;			//	�l���J�n����
	int			RemainTime[2];		//	�c����
	int			ElapseTime[2];		//	�g�p����
	bool		TurnBoard;			//	�Ֆʂ𔽓]���Ă��邩
	vector<LOG>	Log;				//	�҂����p

	CWinThread *SearchThread;
	CWinThread *PreSearchThread;

	bool		PositionToXY( int pos, int *x, int *y );
	int			XYToPosition( int x, int y );
	void		Move( MOVE move );
	void		GetBotMove();
	void		GetBotPreMove();
	void		DrawTimer( Graphics *g );
	void		NewGame();
	void		Undo();

protected: // �V���A��������̂ݍ쐬���܂��B
	CShogi55GUIView();
	DECLARE_DYNCREATE(CShogi55GUIView)

// ����
public:
	CShogi55GUIDoc* GetDocument() const;

// ����
public:

// �I�[�o�[���C�h
public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ����
public:
	virtual ~CShogi55GUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnUpdateNewgame(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRotate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateHint(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEdit(CCmdUI *pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRotate();
	afx_msg void OnNewgame();
	afx_msg void OnUndo();
	afx_msg void OnStop();
	afx_msg void OnEdit();
};

#ifndef _DEBUG  // Shogi55GUIView.cpp �̃f�o�b�O �o�[�W����
inline CShogi55GUIDoc* CShogi55GUIView::GetDocument() const
   { return reinterpret_cast<CShogi55GUIDoc*>(m_pDocument); }
#endif

