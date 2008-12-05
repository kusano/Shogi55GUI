#pragma once


// CPieceSelectDialog ダイアログ

class CPieceSelectDialog : public CDialog
{
	DECLARE_DYNAMIC(CPieceSelectDialog)

public:
	CPieceSelectDialog(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPieceSelectDialog();

// ダイアログ データ
	enum { IDD = IDD_PIECESELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	int Piece;
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedPiece( UINT nID );
};
