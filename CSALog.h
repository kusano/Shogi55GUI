#pragma once


// CCSALog �_�C�A���O

class CCSALog : public CDialog
{
	DECLARE_DYNAMIC(CCSALog)

public:
	CCSALog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CCSALog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CSALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
