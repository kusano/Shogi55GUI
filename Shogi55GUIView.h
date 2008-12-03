// Shogi55GUIView.h : CShogi55GUIView クラスのインターフェイス
//


#pragma once


#include "engine/minmaxbot.h"




class CShogi55GUIView : public CView
{
	friend UINT SearchMove( CShogi55GUIView *view );

public:
	enum MODE
	{
		M_HUMANTURN,
		M_BOTTUNRN,
		M_EDIT
	};

private:
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

	int			PlayerType[2];		//	0 CPU	1 人

	MODE		Mode;
	int			SelectPosition;
	int			LastMoveFrom;
	int			LastMoveTo;
	int			StartTime;			//	考慮開始時刻
	int			RemainTime[2];		//	残時間
	int			ElapseTime[2];		//	使用時間
	bool		TurnBoard;			//	盤面を反転しているか
	vector<LOG>	Log;				//	待った用

	CWinThread *SearchThread;

	bool		PositionToXY( int pos, int *x, int *y );
	int			XYToPosition( int x, int y );
	void		Move( MOVE move );
	void		GetBotMove();
	void		DrawTimer( Graphics *g );
	void		NewGame();
	void		Undo();

protected: // シリアル化からのみ作成します。
	CShogi55GUIView();
	DECLARE_DYNCREATE(CShogi55GUIView)

// 属性
public:
	CShogi55GUIDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 実装
public:
	virtual ~CShogi55GUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
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
};

#ifndef _DEBUG  // Shogi55GUIView.cpp のデバッグ バージョン
inline CShogi55GUIDoc* CShogi55GUIView::GetDocument() const
   { return reinterpret_cast<CShogi55GUIDoc*>(m_pDocument); }
#endif

