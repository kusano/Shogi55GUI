#pragma once


// CNewGameDialog �_�C�A���O

class CNewGameDialog : public CDialog
{
	DECLARE_DYNAMIC(CNewGameDialog)

public:
	CNewGameDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CNewGameDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_NEWGAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	int Black;
	int White;
public:
	int TimeLimit;
};
