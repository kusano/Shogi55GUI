// Shogi55GUI.h : Shogi55GUI �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��


// CShogi55GUIApp:
// ���̃N���X�̎����ɂ��ẮAShogi55GUI.cpp ���Q�Ƃ��Ă��������B
//

class CShogi55GUIApp : public CWinApp
{
	ULONG_PTR GdiToken;

public:
	CShogi55GUIApp();


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CShogi55GUIApp theApp;