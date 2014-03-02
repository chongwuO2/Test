#pragma once
#include "afxwin.h"


// CTextparameter 对话框

class CTextparameter : public CDialog
{
	DECLARE_DYNAMIC(CTextparameter)

public:
	CTextparameter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTextparameter();

// 对话框数据
	enum { IDD = IDD_TEXTPARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeWidth();
	afx_msg void OnEnChangeHeight();
	CEdit m_width;
	CEdit m_height;
	virtual BOOL OnInitDialog();
public:
	int height;
	int width;
	CEdit m_Windowsize;
	afx_msg void OnEnChangeWsize();
};
