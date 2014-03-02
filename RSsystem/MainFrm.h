
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "gdal_priv.h"
typedef struct Canshu
{
	CString str;
	int bands_sec; //����ѡ��
	int m_classnum;//�����
	int dim_num;   //ά��
	float ** Init_Mu;   //��¼ȫ����������ֵ����
	float ** Init_Sigma;

}Canshu;
class CMainFrame : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
	unsigned char * poBandBlock_Gray;
    unsigned char * poBandBlock_R;
    unsigned char * poBandBlock_G;
	unsigned char * poBandBlock_B;
	BOOL BandsType;
	int nXsize;
	int nYsize;
	Canshu canshu;
   
	int Classifyruleflag;//Classifyruleflag=0��ʾ��С���룬Classifyruleflag=1��ʾ��С������
	int window_size;//��ȡ��������ʱ�����Ĵ�С
	int feature_mark[6];//����������Ϊ�Ҷȣ��������Աȶȣ����أ��أ�����ԣ�ѡ���ǣ���ѡ�и�������Ϊ1��δѡ����Ϊ0��

	float * Init_Mu_real;//��¼��ʵѡ�е�������ֵ
	float * Init_Sigma_real;
	float * IteratRegion;//�����������
	int step;//��������
	int **Strategy;//���й����ļ�//�ռ�δ�ͷ�

	int D_B_flag;//��־ѡ��DAG���߶�����,D_B_flag=0 ѡ�����DAG��D_B_flag=1��ʾѡ����Ƕ�����
	int m_SPflag;//m_SPflag=0 ��ʾ����ѡ������ʹ�ù���m_SPflag=1 ��ʾѡ����������ʹ�ù���
// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

};

