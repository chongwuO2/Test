#pragma once


// CTextwindow �Ի���

class CTextwindow : public CDialog
{
	DECLARE_DYNAMIC(CTextwindow)

public:
	CTextwindow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTextwindow();

// �Ի�������
	enum { IDD = IDD_TEXTWINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeWindow();
	virtual BOOL OnInitDialog();
};
