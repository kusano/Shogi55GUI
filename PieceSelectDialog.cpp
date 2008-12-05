// PieceSelectDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Shogi55GUI.h"
#include "PieceSelectDialog.h"
#include "SHogi55GUIDoc.h"
#include "Shogi55GUIView.h"


// CPieceSelectDialog ダイアログ

IMPLEMENT_DYNAMIC(CPieceSelectDialog, CDialog)

CPieceSelectDialog::CPieceSelectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPieceSelectDialog::IDD, pParent)
	, Piece(0)
{

}

CPieceSelectDialog::~CPieceSelectDialog()
{
}

void CPieceSelectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_SPACE, Piece);
}


BEGIN_MESSAGE_MAP(CPieceSelectDialog, CDialog)
	//ON_BN_CLICKED(IDC_SPACE, &CPieceSelectDialog::OnBnClickedPiece)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_SPACE, IDC_WRYU, &CPieceSelectDialog::OnBnClickedPiece)
END_MESSAGE_MAP()


// CPieceSelectDialog メッセージ ハンドラ

void CPieceSelectDialog::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//CDialog::OnOK();
}

void CPieceSelectDialog::OnCancel()
{
	((CShogi55GUIView *)((CFrameWnd *)::AfxGetApp()->GetMainWnd())->GetActiveView())->OnEdit();

	//CDialog::OnCancel();
}

void CPieceSelectDialog::OnBnClickedPiece( UINT nID )
{
	UpdateData( TRUE );
}
