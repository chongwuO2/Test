
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "gdal_priv.h"
typedef struct Canshu
{
	CString str;
	int bands_sec; //波段选择
	int m_classnum;//类别数
	int dim_num;   //维数
	float ** Init_Mu;   //记录全部的样本均值方差
	float ** Init_Sigma;

}Canshu;
class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:
	unsigned char * poBandBlock_Gray;
    unsigned char * poBandBlock_R;
    unsigned char * poBandBlock_G;
	unsigned char * poBandBlock_B;
	BOOL BandsType;
	int nXsize;
	int nYsize;
	Canshu canshu;
   
	int Classifyruleflag;//Classifyruleflag=0表示最小距离，Classifyruleflag=1表示最小错误率
	int window_size;//提取纹理特征时开窗的大小
	int feature_mark[6];//特征（依次为灰度，能量，对比度，逆差矩，熵，相关性）选择标记，若选中该特征则为1，未选中则为0，

	float * Init_Mu_real;//记录真实选中的样本初值
	float * Init_Sigma_real;
	float * IteratRegion;//保存迭代区间
	int step;//迭代步长
	int **Strategy;//串行规则文件//空间未释放

	int D_B_flag;//标志选择DAG或者二叉树,D_B_flag=0 选择的是DAG，D_B_flag=1表示选择的是二叉树
	int m_SPflag;//m_SPflag=0 表示并联选择特征使用规则，m_SPflag=1 表示选择特征串联使用规则
// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

};

