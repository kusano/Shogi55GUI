#pragma once
#include "afxwin.h"


// CCSADialog �_�C�A���O

class CCSADialog : public CDialog
{
	DECLARE_DYNAMIC(CCSADialog)

public:
	CCSADialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CCSADialog();

	void	Clear();
	void	Output( CString str );

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CSA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void OnOK();
public:
	CEdit Log;
};
