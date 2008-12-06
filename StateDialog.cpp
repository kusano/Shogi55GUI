// StateDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Shogi55GUI.h"
#include "StateDialog.h"


// CStateDialog ダイアログ

IMPLEMENT_DYNAMIC(CStateDialog, CDialog)

CStateDialog::CStateDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStateDialog::IDD, pParent)
	, ImageBackground( L"data\\statebackground.jpg" )
	, ImagePiece( L"data\\piece_s.png" )
	, ImageBoard( L"data\\board_s.png" )
	, ImageHash( L"data\\hash.png" )
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

	//double t = clock() / (double)CLOCKS_PER_SEC;
	//int cx = (int)( cos(t/30*2*3.14) * 8 + 8 );
	//int cy = (int)( sin(t/30*2*3.14) * 8 + 8 );

	MoveNode( State->tree, 0, 0, 0, 0 );

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
	//g.Clear( Color::White );
	g.DrawImage( &ImageBackground, 0, 0 );

	if ( State != NULL )
	{
		for ( int i=0; i<STATE::HASHNUM; i++ )
		if ( State->hash[i].used )
		{
			int x = i % 16;
			int y = i / 16;
			int t;

			if ( ! State->hash[i].current )
				t = 0;
			else if ( ! State->hash[i].beta )
				t = 1;
			else if ( ! State->hash[i].alpha )
				t = 2;
			else
				t = 3;
			
			g.DrawImage( &ImageHash,
					x*64-15, y*16-x%2*8-5,
					t*64, min(State->hash[i].depth,31)*16,
					64,16, UnitPixel );
		}
		
		DrawNode( &g, State->tree, 0 );
	}

	//	転送
	Graphics graphics( dc );
	graphics.DrawImage( &backbmp, 0, 0 );
}



void CStateDialog::DrawBoard( Graphics *g, const NODE *node )
{
	int x = node->x + 2;
	int y = node->y + 4;

	g->DrawImage( &ImageBoard, x, y );

	x += 6;
	y += 6;

	for ( int i=0; i<node->characternum; i++ )
	{
		const CHARACTER &c = node->character[i];
		
		if ( c.x1 == c.x2  &&  c.y1 == c.y2 )
		{
			SolidBrush brush( c.value > 0 ? Color(64,0,255,0) : Color(64,255,0,0) );
			g->FillRectangle( &brush, x+(4-c.x1+1)*PW+2, y+c.y1*PH+2, PW-4, PH-4 );
		}
		else
		{
			Pen pen( c.value > 0 ? Color(128,0,255,0) : Color(128,255,0,0), 2 );
			g->DrawLine( &pen, x+(4-c.x1+1)*PW+PW/2, y+c.y1*PH+PH/2,
						 x+(4-c.x2+1)*PW+PW/2, y+c.y2*PH+PH/2 );
		}
	}

	const BOARD &board = node->board;

	for ( int px=0; px<5; px++ )
	for ( int py=0; py<5; py++ )
		g->DrawImage( &ImagePiece, x+(4-px+1)*PW, y+py*PH,
					  board.board[px][py]*PW, 0, PW, PH, UnitPixel );

	for ( int h=0; h<5; h++ )
	{
		if ( board.hand[h][0] > 0 )
			g->DrawImage( &ImagePiece, x+6*PW, y+(4-h)*PH,
						  (h*2+2)*PW, 0, PW, PH, UnitPixel );
		if ( board.hand[h][1] > 0 )
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

void CStateDialog::DrawNode( Graphics *g, const NODE *tree, int node )
{
	int childnum = min( tree[node].childnum, tree[node].childnummax-1 );

	//	線を描画
	for ( int i=0; i<tree[node].childnum; i++ )
	if ( tree[tree[node].child[i]].draw )
	{
		int c = tree[node].child[i];

		Pen pen( Color::Black, tree[c].current ? 2.0f : 1.0f );
		g->DrawLine( &pen, tree[node].x+2+90, tree[node].y+4+36, tree[c].x+2+6, tree[c].y+4+36 );
	}

	//	子供を描画
	for ( int i=0; i<tree[node].childnum; i++ )
	if ( tree[tree[node].child[i]].draw )
	{
		DrawNode( g, tree, tree[node].child[i] );
	}

	//	自分を描画
	DrawBoard( g, &tree[node] );
}

void CStateDialog::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//CDialog::OnCancel();
}

void CStateDialog::MoveNode( NODE *tree, int node, int depth, int x, int y )
{
	//	目標位置に近づける
	tree[node].x = ( tree[node].x + x ) / 2;
	tree[node].y = ( tree[node].y + y ) / 2;

	if ( abs( tree[node].x - x ) < 2 )  tree[node].x = x;
	if ( abs( tree[node].y - y ) < 2 )  tree[node].y = y;

	//	子供の目標位置を設定
	if ( tree[node].childnum > 0 )
	{
		int dx[] = { 120, 110, 100, 90, 90 };
		int dy[] = { 210, 66, 33, 0, 0 };

		vector<pair<int,int> > child( tree[node].childnum );
		for ( int i=0; i<(int)child.size(); i++ )
			child[i].first = tree[node].child[i],
			child[i].second = tree[tree[node].child[i]].value;

		struct cmp
		{
			bool operator()( pair<int,int> a, pair<int,int> b ) {
				return a.second > b.second;
			}
		};

		sort( child.begin(), child.end(), cmp() );

		if ( tree[node].childnum == tree[node].childnummax  &&
			 tree[child.back().first].current )
			swap( child[child.size()-2], child.back() );

		int num = min( (int)child.size(), tree[node].childnummax-1 );

		for ( int i=0; i<num; i++ )
		{
			int cx = tree[node].x + dx[depth];
			int cy = tree[node].y + dy[depth] * i;

			if ( depth == 2   &&  i % 2 == 1 )
				cx += dx[depth+1]*2 + 10;

			MoveNode( tree, child[i].first, depth+1, cx, cy );
			tree[child[i].first].draw = true;
		}

		if ( tree[node].childnum == tree[node].childnummax )
			tree[child.back().first].draw = false;
	}
}