// PromoteDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Shogi55GUI.h"
#include "PromoteDialog.h"


// CPromoteDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CPromoteDialog, CDialog)

CPromoteDialog::CPromoteDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPromoteDialog::IDD, pParent)
	, Promote(0)
{

}

CPromoteDialog::~CPromoteDialog()
{
}

void CPromoteDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_PROMOTE, Promote);
}


BEGIN_MESSAGE_MAP(CPromoteDialog, CDialog)
END_MESSAGE_MAP()


// CPromoteDialog ���b�Z�[�W �n���h��
