
// RSsystemView.h : CRSsystemView ��Ľӿ�
//

#pragma once

class CRSsystemView : public CView
{
protected: // �������л�����
	CRSsystemView();
	DECLARE_DYNCREATE(CRSsystemView)

// ����
public:
	CRSsystemDoc* GetDocument() const;

// ����
public:

// ��д
public:
//	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CRSsystemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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
   /* int state;    //λͼ��ʾ״̬
     CString BmpName;    //����ͼ���ļ��ļ���
     CString extname;    //����ͼ���ļ���չ��
     CBitmap m_bitmap;    //����λͼ����
    void ShowBitmap(CString BmpName);    //������ʾָ��λͼ�ĺ���
     BOOL ShowJpgGif(CDC* pDC,CString strPath, int x, int y);*/
	int bands;//������
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
	BOOL OpenFlag;//��Ǵ�ͼ��
	unsigned char * poBandBlock_Gray;
    unsigned char * poBandBlock_R;
    unsigned char * poBandBlock_G;
	unsigned char * poBandBlock_B;
	 afx_msg void OnFileOpen();
	 virtual void OnDraw(CDC* /*pDC*/);
	 afx_msg void OnAssess();
};

#ifndef _DEBUG  // RSsystemView.cpp �еĵ��԰汾
inline CRSsystemDoc* CRSsystemView::GetDocument() const
   { return reinterpret_cast<CRSsystemDoc*>(m_pDocument); }
#endif

