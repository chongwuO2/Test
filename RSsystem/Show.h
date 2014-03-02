#pragma once
#include "Image_kappa_evaluation.h"

// CShow �Ի���

class CShow : public CDialog
{
	DECLARE_DYNAMIC(CShow)

public:
	CShow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShow();
	CBrush m_brush;
	CFont m_font;
	float* sortdata;
	//Ccaiyang����
	int nWidth,nHeight;//��ͼ��Ĵ�С
	int window_size;
	int sample_num;    //ÿ���������
	int m_classnum;    //�����
	int feature_mark[6];      //�������
	Point* CYArea;            //������������
	static const int MAXNUM_POINT= 50;	//�����������
	Point CYPoint[MAXNUM_POINT];		//����ԭͼ�������
	int* countPixel;          //Ͱ
	int maxPixel;             //ֱ��ͼ���Ƶ��
	int sumPixel;             //��������Ŀ
	int sumPoint;			  //��������Ŀ
	//������KAPPA���ƴ���
	float xstart,xend;		  //x������ʼ
	float xvalue_interval;    //ֵ���
	int count_num;			  //Ͱ��С
	int xaxis_interval;		  //x������	
	bool flag;				  //0��ʾ�������ڣ�1��ʾKappa����
	//combo references
	int* feature_name;
	int curfeature_index, curcategory_index; //��ǰ����������±�

// �Ի�������
	enum { IDD = IDD_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
