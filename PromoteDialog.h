#pragma once


// CPromoteDialog �_�C�A���O

class CPromoteDialog : public CDialog
{
	DECLARE_DYNAMIC(CPromoteDialog)

public:
	CPromoteDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CPromoteDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PROMOTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	int Promote;
};
