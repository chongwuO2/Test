#pragma once
#include "afxwin.h"


// CClassificationrule 对话框

class CClassificationrule : public CDialog
{
	DECLARE_DYNAMIC(CClassificationrule)

public:
	CClassificationrule(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClassificationrule();

// 对话框数据
	enum { IDD = IDD_CLASSRULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_classrule;
	afx_msg void OnBnClickedPararule();
	afx_msg void OnBnClickedSerrule();
	afx_msg void OnCbnSelchangerule();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnOpenRulefile();
	afx_msg void OnSeldag();
	afx_msg void OnSelbt();
public:
	int  m_SPflag;
};
