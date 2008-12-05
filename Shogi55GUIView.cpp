// Shogi55GUIView.cpp : CShogi55GUIView クラスの実装
//

#include "stdafx.h"
#include "Shogi55GUI.h"

#include "Shogi55GUIDoc.h"
#include "Shogi55GUIView.h"
#include "NewGameDialog.h"
#include "PromoteDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
	ON_WM_CREATE()
	ON_COMMAND(ID_ROTATE, &CShogi55GUIView::OnRotate)
	ON_COMMAND(ID_NEWGAME, &CShogi55GUIView::OnNewgame)
	ON_COMMAND(ID_UNDO, &CShogi55GUIView::OnUndo)
	ON_COMMAND(ID_STOP, &CShogi55GUIView::OnStop)
	ON_COMMAND(ID_EDIT, &CShogi55GUIView::OnEdit)
END_MESSAGE_MAP()

// CShogi55GUIView コンストラクション/デストラクション

CShogi55GUIView::CShogi55GUIView()
	: ImageBackground( _T("data\\background.png") )
	, ImageBoard( _T("data\\board.png") )
	, ImagePiece( _T("data\\piece.png") )
	, ImageSelect( _T("data\\select.png") )
	, ImageSearch( _T("data\\search.png") )
	, ImageTimer( _T("data\\timer.png") )
	, Mode( M_HUMANTURN )
	, SelectPosition( -1 )
	, TurnBoard( false )
	, SearchThread( NULL )
{
	PlayerType[0] = 1;
	PlayerType[1] = 0;

	TimeLimit = 20 * 60 * 1000;

	//	タイマーの精度を上げる
	::timeBeginPeriod( 1 );

	//	盤面・評価関数の初期化
	Board.Initialize();

	FILE *f;
	bool failed = false;
	int weight[CBoard::ELEMNUM];

	fopen_s( &f, "data\\weight.txt", "r" );

	if ( f != NULL )
	{
		for ( int i=0; i<CBoard::ELEMNUM; i++ )
			if ( fscanf_s( f, "%d", &weight[i] ) != 1 )
				failed = true;
		fclose( f );
	}
	else
		failed = true;

	if ( ! failed )
		Board.SetWeight( weight );
	else
		::AfxMessageBox( _T("評価関数の読み込みに失敗しました") );
	
	Bot.SetTimeLimit( 10000 );
	Bot.SetDisplayMode( CMinMaxBot::DM_NORMAL );
}

CShogi55GUIView::~CShogi55GUIView()
{
	::timeEndPeriod( 1 );
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

	//	更新リージョンがタイマーの場所ならタイマーを描画して終了
	//	Vistaだとうまく動かない？
	CRect update;
	GetUpdateRect( &update );

	if ( update == CRect(452,12,452+9*12,12+4*24) )
	{
		Graphics g( *pDC );
		DrawTimer( &g );
	}

	//	バッファ
	Bitmap backbmp( PW*13, PH*7 );
	Graphics g( &backbmp );

	//	背景
	g.DrawImage( &ImageBackground, 0, 0 );
    g.DrawImage( &ImageBoard, 0, 0 );

	//	タイマー
	DrawTimer( &g );

	//	選択カーソル
	if ( SelectPosition >= 0 )
	{
		int x, y;
		PositionToXY( SelectPosition, &x, &y );
		g.DrawImage( &ImageSelect, x, y, Board.GetTurn()*PW*2, 0, PW, PH, UnitPixel );
	}

	//	最終手表示
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

	//	駒
	for ( int y=0; y<5; y++ )
	for ( int x=0; x<5; x++ )
	{
		int p = Board.GetPiece( x+1, y+1 );
		int xx, yy;
		PositionToXY( y*5+x+6, &xx, &yy );

		int px = p;
		if ( px != 0  &&  TurnBoard )
			px ^= 1;

		g.DrawImage( &ImagePiece, xx, yy, px*PW, 0, PW, PH, UnitPixel );
	}

	for ( int m=2; m<14; m++ )
	{
		int n = Board.GetHand( m );
		if ( n >= 1 )
		{
			int x, y;
			if ( m % 2 == 0 )
				PositionToXY( 36-(m-2)/2, &x, &y );
			else
				PositionToXY( (m-3)/2, &x, &y );

			int px = m;
			if ( px != 0  &&  TurnBoard )
				px ^= 1;

			for ( int i=0; i<n; i++ )
				g.DrawImage( &ImagePiece, x+(n-i-1)*(m%2==0?8:-8), y, px*PW, 0, PW, PH, UnitPixel );
		}
	}

	//	探索中
	if ( Mode == M_BOTTUNRN )
		g.DrawImage( &ImageSearch, 0, 0 );

	//	転送
	Graphics graphics( *pDC );
	graphics.DrawImage( &backbmp, 0, 0 );
}

void CShogi55GUIView::DrawTimer( Graphics *g )
{
	int turn = Board.GetTurn();
	int elapse = ::timeGetTime() - StartTime;

	int time[4] = {
		RemainTime[0] - ( turn==0 ? elapse : 0 ),	//	先手残時間
		turn == 0 ? elapse : ElapseTime[0],			//	先手使用時間
		RemainTime[1] - ( turn==1 ? elapse : 0 ),	//	後手残時間
		turn == 1 ? elapse : ElapseTime[1],			//	後手使用時間
	};

	for ( int i=0; i<4; i++ )
	{
		if ( time[i] < 0 )
			time[i] = 0;

		int dig[9];

		dig[0] = time[i] / 60 / 1000 / 10 % 10;
		if ( dig[0] == 0 )  dig[0] = 12;
		dig[1] = time[i] / 60 / 1000 % 10;
		dig[2] = 10;
		dig[3] = time[i] / 1000 / 10 % 6;
		dig[4] = time[i] / 1000 % 10;
		dig[5] = 11;
		dig[6] = time[i] / 100 % 10;
		dig[7] = time[i] / 10 % 10;
		dig[8] = time[i] % 10;

		for ( int j=0; j<9; j++ )
			g->DrawImage( &ImageTimer, 452+j*12, 12+i*24, dig[j]*12, 0, 12, 24, UnitPixel );
	}
}


/*
 *	PositionToXY
 *	XYToPosition
 *
 *	┌───┬───────┐
 *	│ 2  3 │10  9  8  7  6│
 *	│ 1  4 │15 14 13 12 11├───┐
 *	│ 0  5 │20 19 18 17 16│31 36 │
 *	└───┤25 24 23 22 21│32 35 │
 *	        │30 29 28 27 26│33 34 │
 *	        └───────┴───┘
 */
bool CShogi55GUIView::PositionToXY( int pos, int *x, int *y )
{
	int xy[37][2] = {
		{ 1, 3}, { 1, 2}, { 1, 1}, { 2, 1}, { 2, 2}, { 2, 3}, { 8, 1}, { 7, 1}, 
		{ 6, 1}, { 5, 1}, { 4, 1}, { 8, 2}, { 7, 2}, { 6, 2}, { 5, 2}, { 4, 2}, 
		{ 8, 3}, { 7, 3}, { 6, 3}, { 5, 3}, { 4, 3}, { 8, 4}, { 7, 4}, { 6, 4}, 
		{ 5, 4}, { 4, 4}, { 8, 5}, { 7, 5}, { 6, 5}, { 5, 5}, { 4, 5}, {10, 3}, 
		{10, 4}, {10, 5}, {11, 5}, {11, 4}, {11, 3}, 
	};

	if ( TurnBoard )
		pos = 36 - pos;

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
		{ -1,  2,  3, -1, 10,  9,  8,  7,  6, -1, -1, -1, -1 },
		{ -1,  1,  4, -1, 15, 14, 13, 12, 11, -1, -1, -1, -1 },
		{ -1,  0,  5, -1, 20, 19, 18, 17, 16, -1, 31, 36, -1 },
		{ -1, -1, -1, -1, 25, 24, 23, 22, 21, -1, 32, 35, -1 },
		{ -1, -1, -1, -1, 30, 29, 28, 27, 26, -1, 33, 34, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	};

	int xx = x / PW;
	int yy = y / PH;

	if ( xx < 0  ||  13 <= xx  ||  yy < 0  ||  7 <= yy )
		return -1;
	else
	{
		int pos = position[yy][xx];

		if ( pos < 0 )
			return pos;
		else
			return TurnBoard ? 36 - pos : pos;
	}
}


/*
 *	NewGame
 *		新規ゲーム開始
 */
void CShogi55GUIView::NewGame()
{
	Board.Initialize();
	Log.clear();

	CSADialog.Clear();
	CSADialog.Output( _T("V1-MSK") );
	CSADialog.Output( PlayerType[0]==0 ? _T("N+Tohske") : _T("N+????") );
	CSADialog.Output( PlayerType[1]==0 ? _T("N-Tohske") : _T("N-????") );
	int minute = TimeLimit / 60 / 1000;
	CString temp;
	temp.Format( _T("$TIME_LIMIT:%02d:%02d+00"), minute/60, minute%60 );
	CSADialog.Output( temp );
	CSADialog.Output( _T("PI") );
	CSADialog.Output( _T("+") );

	LastMoveFrom = -1;
	LastMoveTo = -1;
	SelectPosition = -1;

	RemainTime[0] = RemainTime[1] = TimeLimit;	//	20分
	ElapseTime[0] = ElapseTime[1] = 0;
	StartTime = ::timeGetTime();

	LOG log;
	log.remain[0] = log.remain[1] = TimeLimit;
	log.lastmovefrom = -1;
	log.lastmoveto = -1;
	Log.push_back( log );

	if ( PlayerType[0] == 0 )
	{
		Mode = M_BOTTUNRN;
		GetBotMove();
	}
	else
	{
		Mode = M_HUMANTURN;
	}

	InvalidateRect( NULL, FALSE );
}


/*
 *	Undo
 *		待った
 */
void CShogi55GUIView::Undo()
{
	if ( Board.GetStep() >= 2 )
	{
		Board.Undo();
		Board.Undo();

		LOG log = Log[ Log.size() - 3 ];
		Log.pop_back();
		Log.pop_back();

		RemainTime[0] = log.remain[0];
		RemainTime[1] = log.remain[1];
		ElapseTime[0] = 0;
		ElapseTime[1] = 0;
		LastMoveFrom = log.lastmovefrom;
		LastMoveTo = log.lastmoveto;

		StartTime = ::timeGetTime();
		SelectPosition = -1;

		CSADialog.Output( _T("%MATTA") );

		InvalidateRect( NULL, FALSE );
	}
}



void CShogi55GUIView::Move( MOVE move )
{
	int turn = Board.GetTurn();
	int elapse = ::timeGetTime() - StartTime;		//	経過時間
	elapse = max( elapse / 1000 * 1000, 1000 );		//	切り捨て、最低１秒

	RemainTime[turn] -= elapse;
	ElapseTime[turn] = elapse;
	ElapseTime[turn^1] = 0;
	StartTime = ::timeGetTime();

	LPCTSTR piece[] = { _T(""), _T("FU"), _T("GI"), _T("KA"), _T("HI"), _T("KI"), _T("OU"),
					   _T("TO"), _T("NG"), _T("UM"), _T("RY") };
	Board.Move( move );

	CString m;
	m.Format( _T("%c%d%d%d%d%s"),
		Board.GetTurn()==0 ? '-' : '+',		//	指した後なので
		move.hand == 0 ? move.from % 7 : 0,
		move.hand == 0 ? move.from / 7 : 0,
		move.to % 7,
		move.to / 7,
		piece[ Board.GetPiece(move.to) / 2 ] );
	CSADialog.Output( m );

	CString t;
	t.Format( _T("T%d"), elapse / 1000 );
	CSADialog.Output( t );

	if ( move.hand > 0 )
		if ( Board.GetTurn() == 0 )
			LastMoveFrom = move.hand/2-1;
		else
			LastMoveFrom = 36-(move.hand/2-1);
	else
		LastMoveFrom = (move.from/7-1) * 5 + (move.from%7-1) + 6;
	LastMoveTo = (move.to/7-1) * 5 + (move.to%7-1) + 6;

	LOG log;
	log.remain[0] = RemainTime[0];
	log.remain[1] = RemainTime[1];
	log.lastmovefrom = LastMoveFrom;
	log.lastmoveto = LastMoveTo;
	Log.push_back( log );

	//	詰みチェック
	if ( Board.IsFinished()  ||
		 Board.IsCheckmated( 0 )  ||  Board.IsCheckmated( 1 ) )
	{
		Mode = M_HUMANTURN;
		InvalidateRect( NULL, FALSE );

		if ( Board.GetValue() > 0 )
			::AfxMessageBox( _T("先手の勝ちです") );
		else
			::AfxMessageBox( _T("後手の勝ちです") );
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
			move.hand = (36-SelectPosition) * 2 + 2;
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
			CPromoteDialog dialog;
			
			if ( dialog.DoModal() == IDOK )
				move.promote = dialog.Promote == 0;
			else
				cancel = true;
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
		int piece = PieceSelectDialog.Piece;
		BOARD board;
		Board.GetBoard( &board );

		Board.Initialize();

		if ( pos <= 5 )
		{
			if ( piece == 0 )
				if ( board.hand[pos][1] > 0 )
					board.hand[pos][1]--;
			if ( 2 <= piece  &&  piece < 12  &&  piece % 2 == 1 )
				if ( board.hand[piece/2-1][1] < 2 )
					board.hand[piece/2-1][1]++;
		}
		
		if ( 31 <= pos )
		{
			if ( piece == 0 )
				if ( board.hand[36-pos][0] > 0 )
					board.hand[36-pos][0]--;
			if ( 2 <= piece  &&  piece < 12  &&  piece % 2 == 0 )
				if ( board.hand[piece/2-1][0] < 2 )
					board.hand[piece/2-1][0]++;
		}

		if ( 6 <= pos  &&  pos <= 30 )
			board.board[(pos-6)%5][(pos-6)/5] = piece;

		Board.SetBoard( &board );

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
	//	残手数数の予測
	//	現在0手なら残り40手　30手なら16手　この間線形
	//	30手目以降は16手と想定
	int step = view->Board.GetStep();
	double remain;
	if ( step <= 30 )
		remain = 40 - step*(40-16)/30.0;
	else
		remain = 16;

	//	残り時間の設定
	int limit = (int)( view->RemainTime[view->Board.GetTurn()] / ( remain / 2 ) );
	
	view->Bot.SetTimeLimit( limit );

	//	探索実行
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
		break;
	case TIMER_TIMER:
		InvalidateRect( CRect(452,12,452+9*12,12+4*24), FALSE );
		break;
	case TIMER_STATE:
		StateDialog.Update( Bot.GetState() );
		break;
	}

	CView::OnTimer(nIDEvent);
}

void CShogi55GUIView::OnUpdateNewgame(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( SearchThread == NULL );
}

void CShogi55GUIView::OnUpdateUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( SearchThread == NULL  &&  Board.GetStep() >= 2 );
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

int CShogi55GUIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	StateDialog.Create( CStateDialog::IDD, this );

	CSADialog.Create( CCSADialog::IDD, this );
	CRect rect;
	CSADialog.GetWindowRect( &rect );
	CSADialog.MoveWindow( 16, 240, rect.Width(), rect.Height() );

	PieceSelectDialog.Create( CPieceSelectDialog::IDD, this );

	SetTimer( TIMER_TIMER, 137, NULL );
	SetTimer( TIMER_STATE, 30, NULL );

	NewGame();

	return 0;
}

void CShogi55GUIView::OnRotate()
{
	TurnBoard = ! TurnBoard;

	InvalidateRect( NULL, FALSE );
}

void CShogi55GUIView::OnNewgame()
{
	CNewGameDialog dialog;

	dialog.Black = PlayerType[0];
	dialog.White = PlayerType[1];
	dialog.TimeLimit = TimeLimit / 1000 / 60;

	if ( dialog.DoModal() == IDOK )
	{
		PlayerType[0] = dialog.Black;
		PlayerType[1] = dialog.White;
		TimeLimit = dialog.TimeLimit * 60 * 1000;
		NewGame();
	}
}

void CShogi55GUIView::OnUndo()
{
	Undo();
}

void CShogi55GUIView::OnStop()
{
	Bot.Halt();
}

void CShogi55GUIView::OnEdit()
{
	if ( Mode != M_EDIT )
	{
		PieceSelectDialog.ShowWindow( SW_SHOW );
		Mode = M_EDIT;
	}
	else
	{
		PieceSelectDialog.ShowWindow( SW_HIDE );
		Mode = M_HUMANTURN;
	}
}
