// Shogi55GUIView.cpp : CShogi55GUIView クラスの実装
//

#include "stdafx.h"
#include "Shogi55GUI.h"

#include "Shogi55GUIDoc.h"
#include "Shogi55GUIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const int	PW		= 44;		//	駒幅
static const int	PH		= 48;		//	駒高

static const int	TIMER_SEARCH	= 100;	//	探索が終了したかチェック

static UINT SearchMove( CShogi55GUIView *view );

// CShogi55GUIView

IMPLEMENT_DYNCREATE(CShogi55GUIView, CView)

BEGIN_MESSAGE_MAP(CShogi55GUIView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_NEWGAME, &CShogi55GUIView::OnUpdateNewgame)
	ON_UPDATE_COMMAND_UI(ID_UNDO, &CShogi55GUIView::OnUpdateUndo)
	ON_UPDATE_COMMAND_UI(ID_ROTATE, &CShogi55GUIView::OnUpdateRotate)
	ON_UPDATE_COMMAND_UI(ID_STOP, &CShogi55GUIView::OnUpdateStop)
	ON_UPDATE_COMMAND_UI(ID_HINT, &CShogi55GUIView::OnUpdateHint)
	ON_UPDATE_COMMAND_UI(ID_EDIT, &CShogi55GUIView::OnUpdateEdit)
END_MESSAGE_MAP()

// CShogi55GUIView コンストラクション/デストラクション

CShogi55GUIView::CShogi55GUIView()
	: ImageBackground( _T("background.png") )
	, ImageBoard( _T("board.png") )
	, ImagePiece( _T("piece.png") )
	, ImageSelect( _T("select.png") )
	, ImageSearch( _T("search.png") )
	, Mode( M_HUMANTURN )
	, SelectPosition( -1 )
	, SearchThread( NULL )
{
	PlayerType[0] = 1;
	PlayerType[1] = 0;

	::timeBeginPeriod( 1 );

	Board.Initialize();
	Bot.SetTimeLimit( 10000 );
}

CShogi55GUIView::~CShogi55GUIView()
{
}

BOOL CShogi55GUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CShogi55GUIView 描画

void CShogi55GUIView::OnDraw(CDC* pDC)
{
	CShogi55GUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect client;
	GetClientRect( &client );

	Bitmap backbmp( PW*13, PH*7 );
	Graphics g( &backbmp );

	g.DrawImage( &ImageBackground, 0, 0 );
    g.DrawImage( &ImageBoard, 0, 0 );

	if ( SelectPosition >= 0 )
	{
		int x, y;
		PositionToXY( SelectPosition, &x, &y );
		g.DrawImage( &ImageSelect, x, y, Board.GetTurn()*PW*2, 0, PW, PH, UnitPixel );
	}
	if ( LastMoveFrom >= 0 )
	{
		int x, y;
		if ( PositionToXY( LastMoveFrom, &x, &y ) )
			g.DrawImage( &ImageSelect, x, y, (Board.GetTurn()^1)*PW*2+PW, 0, PW, PH, UnitPixel );
	}
	if ( LastMoveTo >= 0 )
	{
		int x, y;
		if ( PositionToXY( LastMoveTo, &x, &y ) )
			g.DrawImage( &ImageSelect, x, y, (Board.GetTurn()^1)*PW*2, 0, PW, PH, UnitPixel );
	}


	for ( int y=0; y<5; y++ )
	for ( int x=0; x<5; x++ )
	{
		int p = Board.GetPiece( x+1, y+1 );
		int xx, yy;
		PositionToXY( y*5+x+6, &xx, &yy );
		g.DrawImage( &ImagePiece, xx, yy, p*PW, 0, PW, PH, UnitPixel );
	}

	for ( int m=2; m<14; m++ )
	{
		int n = Board.GetHand( m );
		if ( n >= 1 )
		{
			int x, y;
			if ( m % 2 == 0 )
				PositionToXY( (m-2)/2+31, &x, &y );
			else
				PositionToXY( (m-3)/2, &x, &y );

			for ( int i=0; i<n; i++ )
				g.DrawImage( &ImagePiece, x+(n-i-1)*(m%2==0?8:-8), y, m*PW, 0, PW, PH, UnitPixel );
		}
	}

	//if ( TurnBoard )
	//	backbmp.RotateFlip( Gdiplus::Rotate180FlipNone );

	if ( Mode == M_BOTTUNRN )
		g.DrawImage( &ImageSearch, 0, 0 );

	Graphics graphics( *pDC );
	graphics.DrawImage( &backbmp, 0, 0 );
}

/*
 *	PositionToXY
 *	XYToPosition
 *
 *	┌───┬───────┐
 *	│ 3  2 │10  9  8  7  6│
 *	│ 4  1 │15 14 13 12 11├───┐
 *	│ 5  0 │20 19 18 17 16│31 36 │
 *	└───┤25 24 23 22 21│32 35 │
 *	        │30 29 28 27 26│33 34 │
 *	        └───────┴───┘
 */
bool CShogi55GUIView::PositionToXY( int pos, int *x, int *y )
{
	int xy[37][2] = {
		{ 2, 3}, { 2, 2}, { 2, 1}, { 1, 1}, { 1, 2}, { 1, 3}, { 8, 1}, { 7, 1}, 
		{ 6, 1}, { 5, 1}, { 4, 1}, { 8, 2}, { 7, 2}, { 6, 2}, { 5, 2}, { 4, 2}, 
		{ 8, 3}, { 7, 3}, { 6, 3}, { 5, 3}, { 4, 3}, { 8, 4}, { 7, 4}, { 6, 4}, 
		{ 5, 4}, { 4, 4}, { 8, 5}, { 7, 5}, { 6, 5}, { 5, 5}, { 4, 5}, {10, 3}, 
		{10, 4}, {10, 5}, {11, 5}, {11, 4}, {11, 3}, 
	};

	if ( pos < 0  ||  37 <= pos )
	{
		return false;
	}
	else
	{
		*x = xy[pos][0] * PW;
		*y = xy[pos][1] * PH;
		return true;
	}
}

int CShogi55GUIView::XYToPosition( int x, int y )
{
	int position[7][13] = {
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1,  3,  2, -1, 10,  9,  8,  7,  6, -1, -1, -1, -1 },
		{ -1,  4,  1, -1, 15, 14, 13, 12, 11, -1, -1, -1, -1 },
		{ -1,  5,  0, -1, 20, 19, 18, 17, 16, -1, 31, 36, -1 },
		{ -1, -1, -1, -1, 25, 24, 23, 22, 21, -1, 32, 35, -1 },
		{ -1, -1, -1, -1, 30, 29, 28, 27, 26, -1, 33, 34, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	};

	int xx = x / PW;
	int yy = y / PH;

	if ( xx < 0  ||  13 <= xx  ||  yy < 0  ||  7 <= yy )
		return -1;
	else
		return position[yy][xx];
}



void CShogi55GUIView::Move( MOVE move )
{
	//DWORD current = ::timeGetTime();
	//int elapse = ( current - StartTime ) / 1000 * 1000;	//	切り捨て
	//if ( elapse <= 0 )  elapse = 1000;					//	最低１秒
	//RemainTime[Board.GetTurn()] -= elapse;
	//ElapseTime[Board.GetTurn()] = current - StartTime;
	//ElapseTime[Board.GetTurn()^1] = 0;
	//StartTime = ::timeGetTime();

	Board.Move( move );
	//Log.push_back( Board );
	
	//LPCTSTR piece[] = { _T(""), _T("FU"), _T("GI"), _T("KA"), _T("HI"), _T("KI"), _T("OU"),
	//				   _T("TO"), _T("NG"), _T("UM"), _T("RY") };
	//CString m;
	//m.Format( _T("%c%d%d%d%d%s"),
	//	Board.GetTurn()==0 ? '-' : '+',
	//	move->IsPut() ? 0 : move->gfx()+1,
	//	move->IsPut() ? 0 : move->gfy()+1,
	//	move->gtx()+1,
	//	move->gty()+1,
	//	piece[ Board.GetPiece(move->gtx(),move->gty()) / 2 ] );
	//CSALogDialog.Output( m );

	//CString t;
	//t.Format( _T("T%d"), elapse / 1000 );
	//CSALogDialog.Output( t );

	if ( move.hand > 0 )
		if ( Board.GetTurn() == 0 )
			LastMoveFrom = move.hand/2-1;
		else
			LastMoveFrom = move.hand/2-1 + 31;
	else
		LastMoveFrom = (move.from/7-1) * 5 + (move.from%7-1) + 6;
	LastMoveTo = (move.to/7-1) * 5 + (move.to%7-1) + 6;

	//	詰みチェック
	if ( /*Board.IsCheckmate()  ||*/
		 Board.IsFinished() )
	{
		Mode = M_HUMANTURN;
		InvalidateRect( NULL, FALSE );

		if ( Board.GetTurn() == 0 )
			::AfxMessageBox( _T("後手の勝ちです") );
		else
			::AfxMessageBox( _T("先手の勝ちです") );
	}
	else
	{
		if ( PlayerType[ Board.GetTurn() ] == 0 )
		{
			Mode = M_BOTTUNRN;
			GetBotMove();
		}
		else
		{
			Mode = M_HUMANTURN;

		//	if ( PlayerType != 0 )
		//		GetPreCPUMove();
		}
	}

	InvalidateRect( NULL, FALSE );
}


void CShogi55GUIView::GetBotMove()
{
	SearchThread = ::AfxBeginThread( (AFX_THREADPROC)SearchMove, this,
					THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED );
	SearchThread->m_bAutoDelete = FALSE;
	SearchThread->ResumeThread();

	SetTimer( TIMER_SEARCH, 30, NULL );
}



// CShogi55GUIView 診断

#ifdef _DEBUG
void CShogi55GUIView::AssertValid() const
{
	CView::AssertValid();
}

void CShogi55GUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CShogi55GUIDoc* CShogi55GUIView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CShogi55GUIDoc)));
	return (CShogi55GUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CShogi55GUIView メッセージ ハンドラ

void CShogi55GUIView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//if ( TurnBoard )
	//{
	//	point.x = 880 - point.x;
	//	point.y = 520 - point.y;
	//}

	int pos = XYToPosition( point.x, point.y );

	if ( pos < 0 )
	{
		SelectPosition = -1;
		CView::OnLButtonDblClk(nFlags, point);
		return;
	}
	
	switch ( Mode )
	{
	case M_HUMANTURN:
		{
		//	マスを選択していなければ選択して終了
		if ( SelectPosition < 0 )
		{
			SelectPosition = pos;
			break;
		}

		//	駒台は移動先にならない
		if ( pos <= 5  ||  31 <= pos )
		{
			SelectPosition = -1;
			break;
		}

		MOVE move;
		
		//	選択されていた場所
		if ( SelectPosition <= 5 )
		{
			move.from = 0;
			move.hand = SelectPosition * 2 + 3;
		}
		else if ( 31 <= SelectPosition )
		{
			move.from = 0;
			move.hand = (SelectPosition-31) * 2 + 2;
		}
		else
		{
			int x = (SelectPosition-6) % 5;
			int y = (SelectPosition-6) / 5;
			move.from = (y+1)*7 + (x+1);
			move.hand = 0;
		}

		move.to = ( (pos-6)/5 + 1 ) * 7 + ( (pos-6)%5 + 1 );
			
		//	成り不成り選択
		bool cancel = false;

		move.promote = false;
		bool narazu = Board.CheckMove( move );
		move.promote = true;
		bool nari = Board.CheckMove( move );

		if ( ! narazu  &&  nari )
			move.promote = true;
		else if ( narazu  &&  nari )
		{
			//CPromoteDialog dialog;
			//dialog.SetPiece( Board.GetPiece( move.fx, move.fy ) );

			//ModalDialog = true;
			//INT_PTR nari = dialog.DoModal();
			//ModalDialog = false;

			//switch ( nari )
			//{
			//case 1: break;
			//case 2: move.ty += 0x80;  break;
			//default:  cancel = true;  break;
			//}
		}
		else
			move.promote = false;


		//	移動可能ならば移動してCPUに手番
		if ( ! cancel  &&  Board.CheckMove( move ) )
		{
			Move( move );
			SelectPosition = -1;
		}
		else
		{
			SelectPosition = -1;
		}

		break;
		}

	case M_EDIT:
		{
		//int piece = PieceSelectDialog.GetPiece();
		//int motigoma[14];
		//int board[5][5];
		//Board.GetBoard( board, motigoma );

		//Board.Initialize();

		//if ( pos <= 5 )
		//{
		//	if ( piece == 0 )
		//		if ( motigoma[pos*2+3] > 0 )
		//			motigoma[pos*2+3]--;
		//	if ( 3 <= piece  &&  piece <= 11  &&  piece % 2 == 1 )
		//		motigoma[piece]++;
		//}
		//
		//if ( 31 <= pos )
		//{
		//	if ( piece == 0 )
		//		if ( motigoma[(pos-31)*2+2] > 0 )
		//			motigoma[(pos-31)*2+2]--;
		//	if ( 2 <= piece  &&  piece <= 10  &&  piece % 2 == 0 )
		//		motigoma[piece]++;
		//}

		//if ( 6 <= pos  &&  pos <= 30 )
		//	board[(pos-6)%5][(pos-6)/5] = piece;

		//Board.SetBoard( board, motigoma );

		break;
		}

	case M_BOTTUNRN:
		break;
	}

	InvalidateRect( NULL, FALSE );

	CView::OnLButtonDown(nFlags, point);
}



static UINT SearchMove( CShogi55GUIView *view )
{
	view->BestMove = view->Bot.GetNext( &view->Board );

	return 0;
}
void CShogi55GUIView::OnTimer(UINT_PTR nIDEvent)
{
	switch ( nIDEvent )
	{
	case TIMER_SEARCH:
		{
		DWORD code;
		::GetExitCodeThread( SearchThread->m_hThread, &code );
		if ( code != STILL_ACTIVE )
		{
			SearchThread->Delete();
			SearchThread = NULL;

			KillTimer( TIMER_SEARCH );

			Move( BestMove );
		}
		}
	}

	CView::OnTimer(nIDEvent);
}

void CShogi55GUIView::OnUpdateNewgame(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( SearchThread == NULL );
}

void CShogi55GUIView::OnUpdateUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( SearchThread == NULL );
}

void CShogi55GUIView::OnUpdateRotate(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}

void CShogi55GUIView::OnUpdateStop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( SearchThread != NULL );
}

void CShogi55GUIView::OnUpdateHint(CCmdUI *pCmdUI)
{
}

void CShogi55GUIView::OnUpdateEdit(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( SearchThread == NULL );
}
