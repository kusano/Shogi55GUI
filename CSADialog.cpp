// CSADialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Shogi55GUI.h"
#include "CSADialog.h"


// CCSADialog ダイアログ

IMPLEMENT_DYNAMIC(CCSADialog, CDialog)

CCSADialog::CCSADialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCSADialog::IDD, pParent)
{

}

CCSADialog::~CCSADialog()
{
}

void CCSADialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CSALOG, Log);
}


BEGIN_MESSAGE_MAP(CCSADialog, CDialog)
END_MESSAGE_MAP()


// CCSADialog メッセージ ハンドラ

void CCSADialog::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//CDialog::OnCancel();
}

void CCSADialog::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//CDialog::OnOK();
}

void CCSADialog::Clear()
{
	Log.SetWindowText( _T("") );
}

void CCSADialog::Output( CString str )
{
	CString org;
	Log.GetWindowText( org );
	org += str + _T("\r\n");
	Log.SetWindowText( org );
	Log.SetSel( org.GetLength(), org.GetLength() );
}
