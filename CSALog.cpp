// CSALog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Shogi55GUI.h"
#include "CSALog.h"


// CCSALog ダイアログ

IMPLEMENT_DYNAMIC(CCSALog, CDialog)

CCSALog::CCSALog(CWnd* pParent /*=NULL*/)
	: CDialog(CCSALog::IDD, pParent)
{

}

CCSALog::~CCSALog()
{
}

void CCSALog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCSALog, CDialog)
END_MESSAGE_MAP()


// CCSALog メッセージ ハンドラ
