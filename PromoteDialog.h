#pragma once


// CPromoteDialog ダイアログ

class CPromoteDialog : public CDialog
{
	DECLARE_DYNAMIC(CPromoteDialog)

public:
	CPromoteDialog(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPromoteDialog();

// ダイアログ データ
	enum { IDD = IDD_PROMOTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	int Promote;
};
