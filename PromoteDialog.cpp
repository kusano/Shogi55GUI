// PromoteDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Shogi55GUI.h"
#include "PromoteDialog.h"


// CPromoteDialog ダイアログ

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


// CPromoteDialog メッセージ ハンドラ
