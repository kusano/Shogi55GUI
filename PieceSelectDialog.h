#pragma once


// CPieceSelectDialog �_�C�A���O

class CPieceSelectDialog : public CDialog
{
	DECLARE_DYNAMIC(CPieceSelectDialog)

public:
	CPieceSelectDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CPieceSelectDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PIECESELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	int Piece;
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedPiece( UINT nID );
};
