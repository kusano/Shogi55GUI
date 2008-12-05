// PieceSelectDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Shogi55GUI.h"
#include "PieceSelectDialog.h"
#include "SHogi55GUIDoc.h"
#include "Shogi55GUIView.h"


// CPieceSelectDialog �_�C�A���O

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


// CPieceSelectDialog ���b�Z�[�W �n���h��

void CPieceSelectDialog::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

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
