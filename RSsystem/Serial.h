#pragma once


// CSerial �Ի���

class CSerial : public CDialogEx
{
	DECLARE_DYNAMIC(CSerial)

public:
	CSerial(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSerial();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	CBrush m_brush;
    CFont m_font;
	afx_msg void OnBnClickedButton1();
	CDC* pDC1;
	HDC hDC1;
	CString SrcPath;
};
