// CSADialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Shogi55GUI.h"
#include "CSADialog.h"


// CCSADialog �_�C�A���O

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


// CCSADialog ���b�Z�[�W �n���h��

void CCSADialog::OnCancel()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	//CDialog::OnCancel();
}

void CCSADialog::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

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
