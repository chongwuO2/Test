#pragma once
#include "afxwin.h"
#include "gdal_priv.h"


// CDisplaySpacesDlg �Ի���
typedef struct  
	{
		BOOL m_showGray;
		int grayBand;
		int RBand;
		int GBand;
		int BBand;
	}DispPara;

class CDisplaySpacesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDisplaySpacesDlg)

public:
	CDisplaySpacesDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDisplaySpacesDlg();

// �Ի�������
	enum { IDD = IDD_DISPLAYSPECS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_BandList;
	CString m_static_Gray;
	CString m_static_R;
	CString m_static_G;
	CString m_static_B;
	BOOL BandsType;
	int bandCount;
	GDALDataset* InputDataset;//����ĳ������ݼ�
	DispPara m_dispara;//�����ʾ�Ĳ���
	virtual BOOL OnInitDialog();
	afx_msg void OnSelcancelBandlist();
	afx_msg void OnLbnDblclkBandlist();
	afx_msg void OnGray();
	afx_msg void OnRgb();
	afx_msg void OnOk();
	afx_msg void OnSelchangeBandlist();
};
