#pragma once
#include "afxwin.h"
#include "Image_kappa_evaluation.h"


// Ccaiyang �Ի���


class Ccaiyang : public CDialogEx
{
	DECLARE_DYNAMIC(Ccaiyang)

public:
	Ccaiyang(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Ccaiyang();
	void InitChoiceMenu();
	// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	unsigned char * poBandBlock_Gray;//�Ҷ�ͼ�񻺴�ռ�
	unsigned char * poBandBlock_R;//��ɫͼ�񻺴�ռ�
	unsigned char * poBandBlock_G;
	unsigned char * poBandBlock_B;
	int m_num1;			//����Ĳ�������
	BOOL BandsType;		//��ʾ��������
	bool state;
	bool m_quote;		//���û���״̬
	bool textureFlag;	//�Ƿ���ȡ����
	bool pointFlag;		//�Ƿ�ʼ�����
	CPoint m_start;//���ο�ʼ��
	//CPoint m_end;//���ν�����
	CPoint m_old;//��һ�����εĽ�����
	int n;	     //��ǰ�����������
	int p;		 //��ǰ���������
	int nWidth,nHeight;//��ͼ��Ĵ�С
	int width,height;//��ʾ����Ŀ�͸�
	float xratio;	// x����ԭͼ��С/ͼ�������С
	float yratio;	// y����ԭͼ��С/ͼ�������С
	int windowsize; // ���ڴ�С
	//int yseparatorrect; ���޷ָ��ߣ�ֱ����height
	CBrush m_brush;//�޸�static��������ɫ
	CFont m_font;//�޸�static�������С������
	CPoint mypoint;
	int m_classnum;
	Point* CYArea;//����ԭͼ����������
	Point* CYAxis;//�����ͻ���������
	static const int MAXNUM_POINT= 50;	//�����������
	Point CYPoint[MAXNUM_POINT];//����ԭͼ�������
	float *Init_Mu_real;
	float *Init_Sigma_real;
	CEdit m_DisMu;
	CEdit m_DisSigma;
	int feature_mark[6];
	int feature1;//��־������ѡ�еĵ�һ�����������
	int k_class,k_feature;
	int k_class1,k_feature1;

	//�����������ݽṹ
	Point** SamArea;
	Point** SamAxis;
	int* SamNum;

	//CComboBox m_Mu;
	//CComboBox m_Sigma;
	//CComboBox m_Mu_feature;
	//CComboBox m_Sigma_feature;
	//CComboBox m_CYMode;
	
};
