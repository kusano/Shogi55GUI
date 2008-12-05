#pragma once


// CNewGameDialog ダイアログ

class CNewGameDialog : public CDialog
{
	DECLARE_DYNAMIC(CNewGameDialog)

public:
	CNewGameDialog(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CNewGameDialog();

// ダイアログ データ
	enum { IDD = IDD_NEWGAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	int Black;
	int White;
public:
	int TimeLimit;
};
