#pragma once
#include "afxwin.h"


// CIteratRegion 对话框

class CIteratRegion : public CDialog
{
	DECLARE_DYNAMIC(CIteratRegion)

public:
	CIteratRegion(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIteratRegion();

// 对话框数据
	enum { IDD = IDD_ITERATION_REGION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeClassf();
	virtual BOOL OnInitDialog();
	CComboBox m_classname;
	CComboBox m_startnum;
	CComboBox m_endnum;
	afx_msg void OnfinishPinput();
	afx_msg void OnBnClickedSavevalue();
	afx_msg void OnBnClickedReselect();
public:
	int classnum;
	int dimnum;
	int t;//存迭代区间的标记
	float * IteratRegion;
	afx_msg void OnBnClickedFlag();
	CButton m_flag;
	afx_msg void OnEnChangeStep();
};
