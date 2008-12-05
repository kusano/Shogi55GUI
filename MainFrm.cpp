// MainFrm.cpp : CMainFrame クラスの実装
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


// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
{
	// TODO: メンバ初期化コードをここに追加してください。
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
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}

	HBITMAP toolbar = (HBITMAP)::LoadImage( NULL, _T("data\\toolbar.bmp"), IMAGE_BITMAP,
							0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	HBITMAP disable = (HBITMAP)::LoadImage( NULL, _T("data\\toolbard.bmp"), IMAGE_BITMAP,
							0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	HBITMAP mask = (HBITMAP)::LoadImage( NULL, _T("data\\toolbarm.bmp"), IMAGE_BITMAP,
							0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	if ( toolbar == NULL  ||  mask == NULL )
	{
		TRACE0("ツールバー画像の読み込みに失敗しました。\n");
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
	//	TRACE0("ダイアログバーの作成に失敗しました。\n");
	//	return -1;		// 作成できませんでした。
	//}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar)/* ||
		!m_wndReBar.AddBar(&m_wndDlgBar)*/)
	{
		TRACE0("rebar の作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}

	// TODO: ツール ヒントを表示しない場合は、削除してください。
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
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 /*| WS_THICKFRAME*/ | WS_MINIMIZEBOX | WS_SYSMENU;

	return TRUE;
}


// CMainFrame 診断

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


// CMainFrame メッセージ ハンドラ




void CMainFrame::OnClose()
{
	int id = ::AfxMessageBox( _T("終了しますか？"), MB_ICONQUESTION | MB_YESNO );

	if ( id == IDYES )
	{
		CFrameWnd::OnClose();
	}
}
