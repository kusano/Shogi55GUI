// Shogi55GUI.h : Shogi55GUI アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CShogi55GUIApp:
// このクラスの実装については、Shogi55GUI.cpp を参照してください。
//

class CShogi55GUIApp : public CWinApp
{
	ULONG_PTR GdiToken;

public:
	CShogi55GUIApp();


// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CShogi55GUIApp theApp;