// Shogi55GUIDoc.cpp : CShogi55GUIDoc �N���X�̎���
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


// CShogi55GUIDoc �R���X�g���N�V����/�f�X�g���N�V����

CShogi55GUIDoc::CShogi55GUIDoc()
{
	// TODO: ���̈ʒu�� 1 �x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CShogi55GUIDoc::~CShogi55GUIDoc()
{
}

BOOL CShogi55GUIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}




// CShogi55GUIDoc �V���A����

void CShogi55GUIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �i�[����R�[�h�������ɒǉ����Ă��������B
	}
	else
	{
		// TODO: �ǂݍ��ރR�[�h�������ɒǉ����Ă��������B
	}
}


// CShogi55GUIDoc �f�f

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


// CShogi55GUIDoc �R�}���h
