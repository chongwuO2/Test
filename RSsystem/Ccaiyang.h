#pragma once
#include "afxwin.h"
#include "Image_kappa_evaluation.h"


// Ccaiyang 对话框


class Ccaiyang : public CDialogEx
{
	DECLARE_DYNAMIC(Ccaiyang)

public:
	Ccaiyang(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Ccaiyang();
	void InitChoiceMenu();
	// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClickedShow();
	afx_msg void OnBnClickedResample();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChoiceCategory1();
	afx_msg void OnChoiceCategory2();
	afx_msg void OnChoiceCategory3();
	afx_msg void OnChoiceCategory4();
	afx_msg void OnEndsampling();
	afx_msg void OnCbnSelchangeJunzhi();
	afx_msg void OnCbnSelchangeFangcha();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCbnSelchangeMuF();
	afx_msg void OnCbnSelchangeSigmaF();
	//afx_msg void OnEnChangeEdit1();
	//afx_msg void OnBnClickedCancel();
	//afx_msg void OnBnClickedButtonPoint();
private:
	BOOL IsSrcImgExist;
public:
	CMenu* pMenu;
	unsigned char * poBandBlock_Gray;//灰度图像缓存空间
	unsigned char * poBandBlock_R;//彩色图像缓存空间
	unsigned char * poBandBlock_G;
	unsigned char * poBandBlock_B;
	int m_num1;			//输入的采样次数
	BOOL BandsType;		//显示波段类型
	bool state;
	bool m_quote;		//设置画笔状态
	bool textureFlag;	//是否提取纹理
	bool pointFlag;		//是否开始点采样
	CPoint m_start;//矩形开始点
	//CPoint m_end;//矩形结束点
	CPoint m_old;//上一个矩形的结束点
	int n;	     //当前区域采样次数
	int p;		 //当前点采样次数
	int nWidth,nHeight;//打开图像的大小
	int width,height;//显示区域的宽和高
	float xratio;	// x方向原图大小/图像区域大小
	float yratio;	// y方向原图大小/图像区域大小
	int windowsize; // 窗口大小
	//int yseparatorrect; 现无分割线，直接用height
	CBrush m_brush;//修改static的字体颜色
	CFont m_font;//修改static的字体大小和类型
	CPoint mypoint;
	int m_classnum;
	Point* CYArea;//采样原图像区域坐标
	Point* CYAxis;//采样客户区域坐标
	static const int MAXNUM_POINT= 50;	//最大采样点个数
	Point CYPoint[MAXNUM_POINT];//采样原图像点坐标
	float *Init_Mu_real;
	float *Init_Sigma_real;
	CEdit m_DisMu;
	CEdit m_DisSigma;
	int feature_mark[6];
	int feature1;//标志参数中选中的第一个特征的序号
	int k_class,k_feature;
	int k_class1,k_feature1;

	//采样新增数据结构
	Point** SamArea;
	Point** SamAxis;
	int* SamNum;

	//CComboBox m_Mu;
	//CComboBox m_Sigma;
	//CComboBox m_Mu_feature;
	//CComboBox m_Sigma_feature;
	//CComboBox m_CYMode;
	
};
