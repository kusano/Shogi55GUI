// Shogi55GUIDoc.h : CShogi55GUIDoc クラスのインターフェイス
//


#pragma once


class CShogi55GUIDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CShogi55GUIDoc();
	DECLARE_DYNCREATE(CShogi55GUIDoc)

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 実装
public:
	virtual ~CShogi55GUIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};


