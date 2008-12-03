// NewGameDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Shogi55GUI.h"
#include "NewGameDialog.h"


// CNewGameDialog ダイアログ

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


// CNewGameDialog メッセージ ハンドラ
