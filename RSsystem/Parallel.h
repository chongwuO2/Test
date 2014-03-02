#pragma once
#include "afxwin.h"


// CParallel 对话框

class CParallel : public CDialogEx
{
	DECLARE_DYNAMIC(CParallel)

public:
	CParallel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParallel();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
//	afx_msg void OnStnClickedStatic3();
	
	afx_msg void OnBnClickedSelsingle();
	afx_msg void OnBnClickedSelall();
	afx_msg void OnBnClickedDelsingle();
	afx_msg void OnBnClickedDelall();
	afx_msg void OnEnChangeEdit1();
	//afx_msg void OnEnChangeWinsize();
	//afx_msg void OnBnClickedPararule();
	CListBox m_listBox1;
	CListBox m_listBox2;
	CEdit m_leibie;
	int m_classnum;
	CBrush m_brush;
	CFont m_font;
	CEdit m_windowsize;
	//afx_msg void OnBnClickedSerrule();
	//afx_msg void OnBnClickedPararule();
//	afx_msg void OnCbnSelchangerule();
};
