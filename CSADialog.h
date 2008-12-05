#pragma once
#include "afxwin.h"


// CCSADialog ダイアログ

class CCSADialog : public CDialog
{
	DECLARE_DYNAMIC(CCSADialog)

public:
	CCSADialog(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CCSADialog();

	void	Clear();
	void	Output( CString str );

// ダイアログ データ
	enum { IDD = IDD_CSA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void OnOK();
public:
	CEdit Log;
};
