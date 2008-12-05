#pragma once


// CCSALog ダイアログ

class CCSALog : public CDialog
{
	DECLARE_DYNAMIC(CCSALog)

public:
	CCSALog(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CCSALog();

// ダイアログ データ
	enum { IDD = IDD_CSALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
