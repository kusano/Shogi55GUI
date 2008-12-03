// Shogi55GUIDoc.cpp : CShogi55GUIDoc クラスの実装
//

#include "stdafx.h"
#include "Shogi55GUI.h"

#include "Shogi55GUIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CShogi55GUIDoc

IMPLEMENT_DYNCREATE(CShogi55GUIDoc, CDocument)

BEGIN_MESSAGE_MAP(CShogi55GUIDoc, CDocument)
END_MESSAGE_MAP()


// CShogi55GUIDoc コンストラクション/デストラクション

CShogi55GUIDoc::CShogi55GUIDoc()
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}

CShogi55GUIDoc::~CShogi55GUIDoc()
{
}

BOOL CShogi55GUIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}




// CShogi55GUIDoc シリアル化

void CShogi55GUIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		// TODO: 読み込むコードをここに追加してください。
	}
}


// CShogi55GUIDoc 診断

#ifdef _DEBUG
void CShogi55GUIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CShogi55GUIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CShogi55GUIDoc コマンド
