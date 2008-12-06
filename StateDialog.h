#pragma once

#include	"engine/minmaxbot.h"

// CStateDialog �_�C�A���O

class CStateDialog : public CDialog
{
	DECLARE_DYNAMIC(CStateDialog)

	const static int	PW		= 12;
	const static int	PH		= 12;

	STATE *		State;

	Image		ImageBackground;
	Image		ImagePiece;
	Image		ImageBoard;
	Image		ImageHash;

	void		DrawNode( Graphics *g, const NODE *tree, int node );
	void		DrawBoard( Graphics *g, const NODE *node );
	void		MoveNode( NODE *tree, int node, int depth, int x, int y );

public:
	CStateDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CStateDialog();

	void		Update( STATE *state );

// �_�C�A���O �f�[�^
	enum { IDD = IDD_STATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	afx_msg void OnPaint();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void OnCancel();
};
