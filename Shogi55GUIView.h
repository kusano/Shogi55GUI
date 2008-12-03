// Shogi55GUIView.h : CShogi55GUIView �N���X�̃C���^�[�t�F�C�X
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
	CBoard		Board;
	CMinMaxBot	Bot;
	MOVE		BestMove;

	Image		ImageBackground;
	Image		ImageBoard;
	Image		ImagePiece;
	Image		ImageSelect;
	Image		ImageSearch;

	int			PlayerType[2];		//	0 CPU	1 �l

	MODE		Mode;
	int			SelectPosition;
	int			LastMoveFrom;
	int			LastMoveTo;

	CWinThread *SearchThread;

	bool		PositionToXY( int pos, int *x, int *y );
	int			XYToPosition( int x, int y );
	void		Move( MOVE move );
	void		GetBotMove();

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
};

#ifndef _DEBUG  // Shogi55GUIView.cpp �̃f�o�b�O �o�[�W����
inline CShogi55GUIDoc* CShogi55GUIView::GetDocument() const
   { return reinterpret_cast<CShogi55GUIDoc*>(m_pDocument); }
#endif
