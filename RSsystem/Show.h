#pragma once
#include "Image_kappa_evaluation.h"

// CShow 对话框

class CShow : public CDialog
{
	DECLARE_DYNAMIC(CShow)

public:
	CShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShow();
	CBrush m_brush;
	CFont m_font;
	float* sortdata;
	//Ccaiyang传递
	int nWidth,nHeight;//打开图像的大小
	int window_size;
	int sample_num;    //每类采样次数
	int m_classnum;    //类别数
	int feature_mark[6];      //特征标记
	Point* CYArea;            //采样区域坐标
	static const int MAXNUM_POINT= 50;	//最大采样点个数
	Point CYPoint[MAXNUM_POINT];		//采样原图像点坐标
	int* countPixel;          //桶
	int maxPixel;             //直方图最大频数
	int sumPixel;             //采样总数目
	int sumPoint;			  //采样点数目
	//采样和KAPPA估计传递
	float xstart,xend;		  //x坐标起始
	float xvalue_interval;    //值间隔
	int count_num;			  //桶大小
	int xaxis_interval;		  //x坐标间隔	
	bool flag;				  //0表示特征窗口，1表示Kappa窗口
	//combo references
	int* feature_name;
	int curfeature_index, curcategory_index; //当前特征、类别下标

// 对话框数据
	enum { IDD = IDD_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCbnSelchangeComboCategory();
	afx_msg void OnCbnSelchangeComboFeature();
	CComboBox m_combo_category;
	CComboBox m_combo_feature;
//	afx_msg void OnBnClickedOk();

private:
	void SHOWBucket_sort(int length, int max_x);
	void SHOWBucket_init();
	int SHOWMaxBucket(int max_x);
	void SHOWCalc_sortdata();
};
