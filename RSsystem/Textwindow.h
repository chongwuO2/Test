#pragma once


// CTextwindow 对话框

class CTextwindow : public CDialog
{
	DECLARE_DYNAMIC(CTextwindow)

public:
	CTextwindow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTextwindow();

// 对话框数据
	enum { IDD = IDD_TEXTWINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeWindow();
	virtual BOOL OnInitDialog();
};
