#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// Canshuguji 对话框

class Canshuguji : public CDialog
{
	DECLARE_DYNAMIC(Canshuguji)

public:
	Canshuguji(CWnd* pParent = NULL);   // 标准构造函数
	Canshuguji *pro;
	virtual ~Canshuguji();

// 对话框数据
	enum { IDD = IDD_CANSHUGUJI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush m_brush;
    CFont m_font;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnPara_estimation();
	CEdit m_Kapppa;
	afx_msg void OnBnClickedTest();
	void  SetprogressPos(int nPos);
	CProgressCtrl m_ProcessControl;
	afx_msg void OnBnClickedButtonKappa();
};
