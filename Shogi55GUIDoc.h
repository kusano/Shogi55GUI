// Shogi55GUIDoc.h : CShogi55GUIDoc �N���X�̃C���^�[�t�F�C�X
//


#pragma once


class CShogi55GUIDoc : public CDocument
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CShogi55GUIDoc();
	DECLARE_DYNCREATE(CShogi55GUIDoc)

// ����
public:

// ����
public:

// �I�[�o�[���C�h
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ����
public:
	virtual ~CShogi55GUIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
};


