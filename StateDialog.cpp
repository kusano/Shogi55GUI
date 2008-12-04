// StateDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Shogi55GUI.h"
#include "StateDialog.h"


// CStateDialog ダイアログ

IMPLEMENT_DYNAMIC(CStateDialog, CDialog)

CStateDialog::CStateDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStateDialog::IDD, pParent)
	, ImagePiece( _T("piece_s.png") )
	, ImageBoard( _T("board_s.png") )
	, State( NULL )
{

}

CStateDialog::~CStateDialog()
{
}

void CStateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStateDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CStateDialog メッセージ ハンドラ

void CStateDialog::OnOK()
{
	//CDialog::OnOK();
}



void CStateDialog::Update( STATE *state )
{
	State = state;

	InvalidateRect( NULL, FALSE );
}

void CStateDialog::OnPaint()
{
	CPaintDC dc(this);

	//	バッファ
	CRect client;
	GetClientRect( &client );

	Bitmap backbmp( client.Width(), client.Height() );
	Graphics g( &backbmp );
	g.Clear( Color::White );

	if ( State != NULL )
	{
		DrawNode( &g, State->tree, 0, 0, 10, 10, 10, 10 );
	}

	//	転送
	Graphics graphics( dc );
	graphics.DrawImage( &backbmp, 0, 0 );
}



void CStateDialog::DrawBoard( Graphics *g, const BOARD *board, int x, int y )
{
	g->DrawImage( &ImageBoard, x, y );

	for ( int px=0; px<5; px++ )
	for ( int py=0; py<5; py++ )
		g->DrawImage( &ImagePiece, x+(px+1)*PW, y+(4-py)*PH,
					  board->board[px][py]*PW, 0, PW, PH, UnitPixel );

	for ( int h=0; h<5; h++ )
	{
		if ( board->hand[h][0] > 0 )
			g->DrawImage( &ImagePiece, x+6*PW, y+(4-h)*PH,
						  (h*2+2)*PW, 0, PW, PH, UnitPixel );
		if ( board->hand[h][1] > 0 )
			g->DrawImage( &ImagePiece, x, y+h*PH,
						  (h*2+3)*PW, 0, PW, PH, UnitPixel );
	}
}

int CStateDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect( 0, 0, 800, 800 );

	::AdjustWindowRectEx( &rect, GetStyle(), FALSE, GetExStyle() );

	SetWindowPos( NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOZORDER );

	return 0;
}

void CStateDialog::DrawNode( Graphics *g, const NODE *tree, int node, int depth, int x, int y, int px, int py )
{
	//	線を描画
	Pen pen( Color::Black );
	g->DrawLine( &pen, px+50, py+30, x+50, y+30 );

	//	子供を描画
	int dx[] = { 100, 100, 100, 100, 100 };
	int dy[] = { 210, 70, 70, 0, 0 };

	int childnum = min( tree[node].childnum, tree[node].childnummax-1 );

	for ( int i=0; i<childnum; i++ )
	{
		int cx = x + dx[depth];
		int cy = y + dy[depth] * i;

		if ( depth == 2   &&  i % 2 == 1 )
			cx += dx[depth]*2,
			cy += dy[depth] / 2;

		DrawNode( g, tree, tree[node].child[i], depth+1, cx, cy, x, y );
	}

	//	自分を描画
	DrawBoard( g, &tree[node].board, x, y );
}
