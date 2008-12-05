// MainFrm.cpp : CMainFrame �N���X�̎���
//

#include "stdafx.h"
#include "Shogi55GUI.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMainFrame �R���X�g���N�V����/�f�X�g���N�V����

CMainFrame::CMainFrame()
{
	// TODO: �����o�������R�[�h�������ɒǉ����Ă��������B
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("�c�[�� �o�[�̍쐬�Ɏ��s���܂����B\n");
		return -1;      // �쐬�ł��܂���ł����B
	}

	HBITMAP toolbar = (HBITMAP)::LoadImage( NULL, _T("data\\toolbar.bmp"), IMAGE_BITMAP,
							0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	HBITMAP disable = (HBITMAP)::LoadImage( NULL, _T("data\\toolbard.bmp"), IMAGE_BITMAP,
							0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	HBITMAP mask = (HBITMAP)::LoadImage( NULL, _T("data\\toolbarm.bmp"), IMAGE_BITMAP,
							0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	if ( toolbar == NULL  ||  mask == NULL )
	{
		TRACE0("�c�[���o�[�摜�̓ǂݍ��݂Ɏ��s���܂����B\n");
		return -1;
	}

	ImageToolBar.Create( 32, 32, ILC_COLOR24 | ILC_MASK, 0, 1 );
	ImageToolBar.Add( CBitmap::FromHandle(toolbar), CBitmap::FromHandle(mask) );
	m_wndToolBar.GetToolBarCtrl().SetImageList( &ImageToolBar );

	ImageToolBarDisable.Create( 32, 32, ILC_COLOR24 | ILC_MASK, 0, 1 );
	ImageToolBarDisable.Add( CBitmap::FromHandle(disable), CBitmap::FromHandle(mask) );
	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList( &ImageToolBarDisable );

	//if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
	//	CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	//{
	//	TRACE0("�_�C�A���O�o�[�̍쐬�Ɏ��s���܂����B\n");
	//	return -1;		// �쐬�ł��܂���ł����B
	//}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar)/* ||
		!m_wndReBar.AddBar(&m_wndDlgBar)*/)
	{
		TRACE0("rebar �̍쐬�Ɏ��s���܂����B\n");
		return -1;      // �쐬�ł��܂���ł����B
	}

	// TODO: �c�[�� �q���g��\�����Ȃ��ꍇ�́A�폜���Ă��������B
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	CRect client;
	/*GetActiveView()->*/GetClientRect( client );

	CRect frame;
	GetWindowRect( frame );

	int dw = 572 - client.Width();
	int dh = 336 - client.Height();

	frame.right += dw;
	frame.bottom += dh + 48;
	MoveWindow( &frame );

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 /*| WS_THICKFRAME*/ | WS_MINIMIZEBOX | WS_SYSMENU;

	return TRUE;
}


// CMainFrame �f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ���b�Z�[�W �n���h��




void CMainFrame::OnClose()
{
	int id = ::AfxMessageBox( _T("�I�����܂����H"), MB_ICONQUESTION | MB_YESNO );

	if ( id == IDYES )
	{
		CFrameWnd::OnClose();
	}
}
