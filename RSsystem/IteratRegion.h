#pragma once
#include "afxwin.h"


// CIteratRegion �Ի���

class CIteratRegion : public CDialog
{
	DECLARE_DYNAMIC(CIteratRegion)

public:
	CIteratRegion(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIteratRegion();

// �Ի�������
	enum { IDD = IDD_ITERATION_REGION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	int t;//���������ı��
	float * IteratRegion;
	afx_msg void OnBnClickedFlag();
	CButton m_flag;
	afx_msg void OnEnChangeStep();
};
