// NewGameDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Shogi55GUI.h"
#include "NewGameDialog.h"


// CNewGameDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CNewGameDialog, CDialog)

CNewGameDialog::CNewGameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNewGameDialog::IDD, pParent)
	, Black(0)
	, White(1)
{

}

CNewGameDialog::~CNewGameDialog()
{
}

void CNewGameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_BHUMAN, Black);
	DDX_Radio(pDX, IDC_WHUMAN, White);
}


BEGIN_MESSAGE_MAP(CNewGameDialog, CDialog)
END_MESSAGE_MAP()


// CNewGameDialog ���b�Z�[�W �n���h��
