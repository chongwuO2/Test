#pragma once


// CSerial 对话框

class CSerial : public CDialogEx
{
	DECLARE_DYNAMIC(CSerial)

public:
	CSerial(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSerial();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
