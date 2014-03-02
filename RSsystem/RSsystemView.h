
// RSsystemView.h : CRSsystemView 类的接口
//

#pragma once

class CRSsystemView : public CView
{
protected: // 仅从序列化创建
	CRSsystemView();
	DECLARE_DYNCREATE(CRSsystemView)

// 特性
public:
	CRSsystemDoc* GetDocument() const;

// 操作
public:

// 重写
public:
//	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CRSsystemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSerial();
	afx_msg void OnParallel();
	afx_msg void OnDialog3();
//	afx_msg void OnPaint();
//	afx_msg void OnIddCanshuguji();
//	afx_msg void OnUpdateIddCanshuguji(CCmdUI *pCmdUI);
//	afx_msg void OnIddCanshuguji();
	afx_msg void OnCanshuguji();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnOpenimg();
//	afx_msg void OnOpenfile();

//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnCanshu();
public:
   /* int state;    //位图显示状态
     CString BmpName;    //保存图像文件文件名
     CString extname;    //保存图像文件扩展名
     CBitmap m_bitmap;    //创建位图对象
    void ShowBitmap(CString BmpName);    //用来显示指定位图的函数
     BOOL ShowJpgGif(CDC* pDC,CString strPath, int x, int y);*/
	int bands;//波段数
	BOOL BandsType;
	int nXsize;
	int nYsize;
	int m_Bands;
	int BdCGray;
	int BdCR;
    int BdCG;
	int BdCB;	
	CBrush m_brush;
	CFont m_font;
	BOOL OpenFlag;//标记打开图像
	unsigned char * poBandBlock_Gray;
    unsigned char * poBandBlock_R;
    unsigned char * poBandBlock_G;
	unsigned char * poBandBlock_B;
	 afx_msg void OnFileOpen();
	 virtual void OnDraw(CDC* /*pDC*/);
	 afx_msg void OnAssess();
};

#ifndef _DEBUG  // RSsystemView.cpp 中的调试版本
inline CRSsystemDoc* CRSsystemView::GetDocument() const
   { return reinterpret_cast<CRSsystemDoc*>(m_pDocument); }
#endif

