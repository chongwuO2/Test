// Ccaiyang.cpp : ʵ���ļ�

#include "stdafx.h"
#include "RSsystem.h"
#include "Ccaiyang.h"
#include "afxdialogex.h"
#include "Show.h"
#include "SkinPPWTL.h"
#include "MainFrm.h"
#include "gdal_priv.h"
#include "Image_kappa_evaluation.h"


// Ccaiyang �Ի���

IMPLEMENT_DYNAMIC(Ccaiyang, CDialogEx)

Ccaiyang::Ccaiyang(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ccaiyang::IDD, pParent)
	, IsSrcImgExist(FALSE)
	, m_num1(0)
	, state(false)
{

}

Ccaiyang::~Ccaiyang()
{
	delete [] Init_Mu_real;
	delete [] Init_Sigma_real;
}

void Ccaiyang::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_JUNZHI, m_Mu);
	//DDX_Control(pDX, IDC_FANGCHA, m_Sigma);
	//DDX_Control(pDX, IDC_MU, m_DisMu);
	//DDX_Control(pDX, IDC_SIGMA, m_DisSigma);
	//DDX_Control(pDX, IDC_MU_F, m_Mu_feature);
	//DDX_Control(pDX, IDC_SIGMA_F, m_Sigma_feature);
	//DDX_Control(pDX, IDC_COMBO_MODE, m_CYMode);
}


BEGIN_MESSAGE_MAP(Ccaiyang, CDialogEx)
	//ON_BN_CLICKED(IDC_BUTTON1, &Ccaiyang::OnBnClickedButton1)
	//ON_BN_CLICKED(IDOK, &Ccaiyang::OnBnClickedOk)

	//�˵���Ӧ����
	ON_BN_CLICKED(IDM_SHOW, &Ccaiyang::OnClickedShow)
	ON_BN_CLICKED(IDC_RESAMPLE, &Ccaiyang::OnBnClickedResample)
	ON_BN_CLICKED(IDC_ENDSAMPLING, &Ccaiyang::OnEndsampling)
	ON_BN_CLICKED(ID_CATEGORY + 1, &Ccaiyang::OnChoiceCategory1)
	ON_BN_CLICKED(ID_CATEGORY + 2, &Ccaiyang::OnChoiceCategory2)
	ON_BN_CLICKED(ID_CATEGORY + 3, &Ccaiyang::OnChoiceCategory3)
	ON_BN_CLICKED(ID_CATEGORY + 4, &Ccaiyang::OnChoiceCategory4)
	//ON_EN_CHANGE(IDC_EDIT1, &Ccaiyang::OnEnChangeEdit1)
	//������Ӧ����
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	//ON_CBN_SELCHANGE(IDC_JUNZHI, &Ccaiyang::OnCbnSelchangeJunzhi)
	//ON_CBN_SELCHANGE(IDC_FANGCHA, &Ccaiyang::OnCbnSelchangeFangcha)
	//ON_CBN_SELCHANGE(IDC_MU_F, &Ccaiyang::OnCbnSelchangeMuF)
	//ON_CBN_SELCHANGE(IDC_SIGMA_F, &Ccaiyang::OnCbnSelchangeSigmaF)

	//ON_BN_CLICKED(IDC_BUTTON_POINT, &Ccaiyang::OnBnClickedButtonPoint)
	//ON_BN_CLICKED(IDCANCEL, &Ccaiyang::OnBnClickedCancel)
END_MESSAGE_MAP()

 
BOOL Ccaiyang::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	nWidth = pMain->nXsize;  
	nHeight = pMain->nYsize;
	SetWindowPos(&wndTopMost,0,0,nWidth+23,nHeight+78,SWP_NOMOVE|SWP_SHOWWINDOW);//23Ϊ�ͻ������ұ߿�78Ϊ�ͻ������±߿�
	/*
	//�趨ͼ����ʾ����
	CRect dialogrect;
	CRect clientrect;
	CRect separatorrect;
	CRect imgarea;
	GetWindowRect(&dialogrect);
	GetClientRect(&clientrect);
	GetDlgItem(IDC_FENGE)->GetWindowRect(&separatorrect);
	int edge= (dialogrect.right- dialogrect.left- clientrect.right)/ 2;
	imgarea.left= dialogrect.left+ edge;
	imgarea.top= dialogrect.bottom- clientrect.bottom;
	imgarea.right= dialogrect.right- edge;
	imgarea.bottom= separatorrect.top;
	width=imgarea.right- imgarea.left;
	height=imgarea.bottom- imgarea.top;
	yseparatorrect= separatorrect.top- imgarea.top;
	*/
	//��ʼ����ʾ����
	//skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC1)->m_hWnd);//�޸�����
	//skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC2)->m_hWnd);
	//skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC3)->m_hWnd);
	//skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC4)->m_hWnd);
	//skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC5)->m_hWnd);
	//m_font.CreatePointFont(150,_T("����"));//�޸����пؼ�������
    //m_brush.CreateSolidBrush(RGB(213,222,243));//�޸Ŀռ䱳����ɫ
	
	
	
	//�趨��ֵ�����е���
	//CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	//��ʼ��
	//nWidth = pMain->nXsize;  
	//nHeight = pMain->nYsize;
	width= nWidth;
	height= nHeight; 
	xratio= (float)(width)/nWidth;
	yratio= (float)(height)/nHeight;
	windowsize= pMain->window_size;
	textureFlag= 0;
	CYArea= NULL;
	Init_Mu_real= NULL;
	Init_Sigma_real= NULL;
	CString str;
	for (int i=0;i< MAXNUM_POINT;i++)
	{
		CYPoint[i].x= 0;
		CYPoint[i].y= 0;
	}
	for(int i=0;i<6;i++)
	{
		feature_mark[i]=pMain->feature_mark[i];
	}
	m_classnum=pMain->canshu.m_classnum;
	//��ʼ���˵�
	if(m_classnum>0)
	{
		pMenu = GetMenu();
		CMenu* pChoiceMenu = pMenu->GetSubMenu(0);
		CMenu* pCarryMenu = pMenu->GetSubMenu(1);
		CMenu* pViewMenu = pMenu->GetSubMenu(2);
		pChoiceMenu -> EnableMenuItem(ID_CATEGORY,MF_DISABLED);
		pCarryMenu -> EnableMenuItem(IDC_RESAMPLE,MF_GRAYED);
		pCarryMenu -> EnableMenuItem(IDC_ENDSAMPLING,MF_GRAYED);
		pViewMenu -> EnableMenuItem(IDM_SHOW,MF_GRAYED);

		char* buf;	//��������ͨ��
		buf = new char[2];
		pChoiceMenu -> AppendMenu(MF_SEPARATOR);
		for (int i = 1;i <= m_classnum;i++)
		{
			itoa(i,buf,10);
			pChoiceMenu -> AppendMenu(MF_STRING,i+ID_CATEGORY,CString(buf));
		}
		delete[] buf;
	}
	else
	{
		MessageBox("�������������");
		OnCancel();
	}

	//���ԣ����������
	m_num1= 2;				
	if (CYArea== NULL)
	{
		CYArea= new Point[2*m_num1*m_classnum];
		CYAxis= new Point[2*(m_num1*m_classnum+ MAXNUM_POINT)];
	}
	

	pointFlag= 0;			//�����
	n= 0;					//�����������
	p= 0;					//���������
	Init_Mu_real=NULL;
	Init_Sigma_real=NULL;
	k_feature=k_feature1=0;
	k_class=k_class1=0;
	return TRUE; 
	// return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
	
}


void Ccaiyang::OnPaint()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CDC* pDC=GetDC();
	//����ͼ�񲿷�
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;   
	BandsType=pMain->BandsType;
	if(BandsType==1)
	{
		int i,j;  
		nWidth = pMain->nXsize;  
		nHeight = pMain->nYsize;
		poBandBlock_Gray = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nWidth*nHeight));//���仺��ռ�
		poBandBlock_Gray = pMain->poBandBlock_Gray;
		BITMAPINFO * pBmpInfo = (BITMAPINFO*) new char[sizeof(BITMAPINFO) + sizeof(RGBQUAD)*(256)];  
		pBmpInfo->bmiHeader.biBitCount = 8;  
		pBmpInfo->bmiHeader.biClrImportant = 0;  
		pBmpInfo->bmiHeader.biClrUsed = 0;  
		pBmpInfo->bmiHeader.biCompression = BI_RGB;
		pBmpInfo->bmiHeader.biWidth = nWidth;
		pBmpInfo->bmiHeader.biHeight = nHeight;  
		pBmpInfo->bmiHeader.biPlanes = 1;  
		pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  
		pBmpInfo->bmiHeader.biSizeImage = (nWidth*8+31)/32*4*nHeight;  
		pBmpInfo->bmiHeader.biXPelsPerMeter = 0;  
		pBmpInfo->bmiHeader.biYPelsPerMeter = 0;  

		//2.��ɫ��  
		for (i = 0;i < 256;i++)     
		{  
			pBmpInfo->bmiColors[i].rgbRed   = i;   
			pBmpInfo->bmiColors[i].rgbGreen = i;    
			pBmpInfo->bmiColors[i].rgbBlue  = i;      
			pBmpInfo->bmiColors[i].rgbReserved = 0;  

		} 

		//����������     
		LONG LineBytes = (nWidth*8+31)/32*4;  
		LPBYTE pData = (LPBYTE)new char[LineBytes*nHeight];
		//Ϊ�����ظ�ֵ������������  
		for (i=0;i<nHeight;i++)  
		{  
			for (j=0;j<nWidth;j++)  
			{  
				pData[(nHeight-i-1)*LineBytes + j] = poBandBlock_Gray[i*nWidth+j];//�˱��ʽ�д��о�  
			}  
		}	

		SetStretchBltMode(pDC->m_hDC,BLACKONWHITE|WHITEONBLACK); //����ָ���豸�����е�λͼ����ģʽ 
		StretchDIBits(pDC->m_hDC, 0, 0, width, height, 0, 0, nWidth, nHeight,   
			pData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);  

		UpdateWindow();  
		delete pBmpInfo;  
		delete pData; 
	}

	else 
	{ 
		int i,j,k;  
		nWidth = pMain->nXsize;  
		nHeight =pMain->nYsize;
		poBandBlock_R = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nWidth*nHeight));//���仺��ռ�
		poBandBlock_G = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nWidth*nHeight));
		poBandBlock_B = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nWidth*nHeight));
		//���洢�ռ��е����ݸ��Ƶ���ǰ��ʾ�Ļ�����
		for (i=0;i<nHeight;i++)  
		{  
			for (j=0;j<nWidth;j++)  
			{ 
				poBandBlock_R[i*nWidth+j]= pMain->poBandBlock_R[i*nWidth+j];	//�˱��ʽ�д��о�
				poBandBlock_G[i*nWidth+j]= pMain->poBandBlock_G[i*nWidth+j];
				poBandBlock_B[i*nWidth+j]= pMain->poBandBlock_B[i*nWidth+j];
			}  
		}	 
		BITMAPINFO * pBmpInfo = (BITMAPINFO*) new char[sizeof(BITMAPINFO) + sizeof(RGBQUAD)*(256)];  
		pBmpInfo->bmiHeader.biBitCount = 24;  
		pBmpInfo->bmiHeader.biClrImportant = 0;  
		pBmpInfo->bmiHeader.biClrUsed = 0;  
		pBmpInfo->bmiHeader.biCompression = BI_RGB;  
		pBmpInfo->bmiHeader.biWidth = nWidth;  
		pBmpInfo->bmiHeader.biHeight = nHeight;  
		pBmpInfo->bmiHeader.biPlanes = 1;  
		pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  
		pBmpInfo->bmiHeader.biSizeImage = (nWidth*24+31)/32*4*nHeight;  
		pBmpInfo->bmiHeader.biXPelsPerMeter = 0;  
		pBmpInfo->bmiHeader.biYPelsPerMeter = 0;  
		//2.��ɫ��  
		for (i = 0;i < 256;i++)  
		{  
			pBmpInfo->bmiColors[i].rgbRed   = i;  
			pBmpInfo->bmiColors[i].rgbGreen = i;  
			pBmpInfo->bmiColors[i].rgbBlue  = i;  
			pBmpInfo->bmiColors[i].rgbReserved = 0;  
		}  
		//����������  
		LONG LineBytes = (nWidth*24+31)/32*4;  
		LPBYTE pData = (LPBYTE)new char[LineBytes*nHeight*3];  
		//Ϊ�����ظ�ֵ������������  
		for (i=0;i<nHeight;i++)  
		{  
			for (j=0,k=0;j<nWidth,k<3*nWidth;j++,k+=3)  
			{  
				//  pData[(nHeight-i-1)*LineBytes + j] = pDoc->poBandBlock_Gray[i*nWidth+j];//�˱��ʽ�д��о�  
				pData[(nHeight-i-1)*LineBytes + k] = poBandBlock_B[i*nWidth + j];  
				pData[(nHeight-i-1)*LineBytes + k+1] = poBandBlock_G[i*nWidth + j];  
				pData[(nHeight-i-1)*LineBytes + k+2] = poBandBlock_R[i*nWidth + j];  
			}  
		}  

		SetStretchBltMode(pDC->m_hDC,BLACKONWHITE|WHITEONBLACK); //����ָ���豸�����е�λͼ����ģʽ 
		StretchDIBits(pDC->m_hDC, 0, 0, width, height, 0, 0, nWidth, nHeight,   
			pData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);  		
		UpdateWindow();  
		delete pBmpInfo;  
		delete pData;  

	}
	//���Ʋ������β���
	CPen newpen;
	if (n>= m_num1*m_classnum&& (!pointFlag))
		newpen.CreatePen(PS_SOLID,1,RGB(0,255,0));
	else
		newpen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	CClientDC pdc(this);
	pdc.SelectObject(newpen);
	pdc.SelectStockObject(NULL_BRUSH);
	pdc.SetROP2( R2_COPYPEN );
	if (n> 0)
		for (int i= 0;i< n;i++)
			pdc.Rectangle(CYAxis[2*i].x,CYAxis[2*i].y,CYAxis[2*i+ 1].x,CYAxis[2*i+ 1].y);

}


void Ccaiyang::OnChoiceCategory1()
{
	InitChoiceMenu();
	CString pText;
	pText = "1 ��";
	LPSTR lpStr = pText.GetBuffer();
	CMenu* Mtest = pMenu->GetSubMenu(0);
	Mtest->ModifyMenu(2, MF_STRING | MF_BYPOSITION, ID_CATEGORY + 1, lpStr);
	pText.ReleaseBuffer();
}
void Ccaiyang::OnChoiceCategory2()
{
	InitChoiceMenu();
	CString pText;
	pText = "2 ��";
	LPSTR lpStr = pText.GetBuffer();
	CMenu* Mtest = pMenu->GetSubMenu(0);
	Mtest->ModifyMenu(3, MF_STRING | MF_BYPOSITION, ID_CATEGORY + 2, lpStr);
	pText.ReleaseBuffer();
}
void Ccaiyang::OnChoiceCategory3()
{
	InitChoiceMenu();
	CString pText;
	pText = "1 ��";
	LPSTR lpStr = pText.GetBuffer();
	CMenu* Mtest = pMenu->GetSubMenu(0);
	Mtest->ModifyMenu(4, MF_STRING | MF_BYPOSITION, ID_CATEGORY + 3, lpStr);
	pText.ReleaseBuffer();
}
void Ccaiyang::OnChoiceCategory4()
{
	InitChoiceMenu();
	CString pText;
	pText = "1 ��";
	LPSTR lpStr = pText.GetBuffer();
	CMenu* Mtest = pMenu->GetSubMenu(0);
	Mtest->ModifyMenu(5, MF_STRING | MF_BYPOSITION, ID_CATEGORY + 4, lpStr);
	pText.ReleaseBuffer();
}
void Ccaiyang::OnClickedShow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (Init_Mu_real== NULL)
	{
		AfxMessageBox("���Ƚ��в���");
		return;
	}
	CShow dlg;
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	//TEST : ��������
	dlg.nWidth= nWidth;
	dlg.nHeight= nHeight;
	dlg.sample_num= m_num1;
	dlg.window_size= pMain->window_size;
	dlg.sumPoint= p;
	for (int i= 0;i< 6;i++)
	{
		dlg.feature_mark[i]= feature_mark[i];
	}
	for (int i= 0;i< p;i++)
	{
		dlg.CYPoint[i]= CYPoint[i];
	}
	dlg.CYArea= CYArea;
	dlg.maxPixel= 0;
	dlg.xstart= 0;
	dlg.xend= 255;
	dlg.xvalue_interval= 1;
	dlg.flag= 0;
//	dlg.countPixel = new int[dlg.maxPixel+1];
//	for (int i=0;i < 256;i++)
//	{
//		dlg.countPixel[i] = i;
//	}	
	dlg.m_classnum = m_classnum;
	dlg.DoModal();
}


//void Ccaiyang::OnBnClickedButton7()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CString str;
//	n=0;
//	if(GetDlgItemText(IDC_BUTTON7,str),str==_T("��ʼȡ��"))
//	{
//		SetDlgItemText(IDC_BUTTON7,_T("����ȡ��"));
//	}
//	else
//	{
//		SetDlgItemText(IDC_BUTTON7,_T("��ʼȡ��"));
//		
//	}
//	
//	
//}

/* ԭ�������������
void Ccaiyang::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	m_num1 = GetDlgItemInt(IDC_EDIT1);
	if (CYArea== NULL)
	{
		CYArea= new Point[2*m_num1*m_classnum];
		CYAxis= new Point[2*(m_num1*m_classnum+ MAXNUM_POINT)];
	}
	else
	{
		delete[] CYArea;
		delete[] CYAxis;
		CYArea= new Point[2*m_num1*m_classnum];
		CYAxis= new Point[2*(m_num1*m_classnum+ MAXNUM_POINT)];
	}
	UpdateData(FALSE);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
*/

void Ccaiyang::OnBnClickedResample()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	n= 0;
	p= 0;
	pointFlag= 0;
	m_quote= true;
	state= false;
	//((CEdit*)(GetDlgItem(IDC_EDIT1)))->SetReadOnly(FALSE);
	//SetDlgItemText(IDC_BUTTON_POINT,"��ʼȡ��");
	Invalidate();
}


void Ccaiyang::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��ִ�в���
	if ((n== m_classnum*m_num1||p> 0)&& (!pointFlag))
		return;
	//ִ�в���
	if (point.y<= height)	//�Ƿ�Ϊͼ������
	{	
		bool existTexture = (point.x>= windowsize*xratio)& (point.x<= width- windowsize*xratio)
			& (point.y>= windowsize*yratio)& (point.y<= height- windowsize*yratio);
		if ((!textureFlag)|| existTexture)		//�Ƿ�Ϊ�������ȡ����
		{
			m_quote= 1;
			if (pointFlag)
			//��ģʽ
			{
				m_start= point;
				state= false;
			} 
			//����ģʽ
			else
			{
				m_start= point;
				state= true;
				m_old= point;
				//SetCapture();
				//CRect rect;
				//GetClientRect(&rect);
				//ClientToScreen(&rect);
				//ClipCursor(&rect);
			}
			

			
			
		}
		else
			AfxMessageBox("�����򲻿���ȡ����");
	} 
	else
		AfxMessageBox("����ͼ���������");


	CDialogEx::OnLButtonDown(nFlags, point);
}


void Ccaiyang::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��ִ�в���
	if ((n== m_classnum*m_num1||p> 0)&& (!pointFlag))	//����������������
		return;
	
	//ִ�в���
	if (point.y<= height)	//�Ƿ�Ϊͼ������
	{	
		bool existTexture = (point.x>= windowsize*xratio)& (point.x<= width- windowsize*xratio)
			& (point.y>= windowsize*yratio)& (point.y<= height- windowsize*yratio);
		if ((!textureFlag)|| existTexture)	//�Ƿ�Ϊ�������ȡ����
		{
			if (m_start== point)
				m_quote= 0;
			//����ģʽ
			if(state&& m_quote&&m_start!= point)
			{
				//if(m_start.x<=)
				state=false;
				//ReleaseCapture();
				//ClipCursor( NULL );
				CPen newpen(PS_SOLID,1,RGB(255,0,0));
				CClientDC dc(this);
				CPen *pOldPen = dc.SelectObject(&newpen);
				//dc.SetROP2( R2_NOT );
				dc.SelectStockObject(NULL_BRUSH);
				//dc.Rectangle(m_start.x,m_start.y,m_old.x,m_old.y);//�ϴ�����ƶ�����λ��֮�����ʱ�� 
				dc.SetROP2( R2_COPYPEN );
				dc.Rectangle(m_start.x,m_start.y,point.x,point.y);
				dc.SelectObject(pOldPen);
				CYAxis[2*n].x= m_start.x;
				CYAxis[2*n].y= m_start.y;
				CYAxis[2*n+1].x= point.x;
				CYAxis[2*n+1].y= point.y;
				CYArea[2*n].x= m_start.x*(float(nWidth)/float(width));
				CYArea[2*n].y= m_start.y*(float(nHeight)/float(height));
				CYArea[2*n+1].x= point.x*(float(nWidth)/float(width));
				CYArea[2*n+1].y= point.y*(float(nHeight)/float(height));
				n++;
				//((CEdit*)(GetDlgItem(IDC_EDIT1)))->SetReadOnly();
				if(n>=m_num1*m_classnum)//����������ĸ�������ֹͣ������
				{
					state=false;
					m_quote=0;
					//ReleaseCapture();
					Invalidate();
				}
			}
			//ȡ��ģʽ
			else if (m_start== point&& pointFlag)
			{				
				if(p>=50)//����������ĸ�������ֹͣ����
				{
					AfxMessageBox("�Ѵﵽ���ȡ����Ŀ(50),�������ȡ��");
					return;
				}
				CPen newpen(PS_SOLID,1,RGB(255,0,0));
				CClientDC dc(this);
				//CPen *pOldPen = dc.SelectObject(&newpen);
				//dc.SetROP2( R2_NOT );
				dc.SelectStockObject(NULL_BRUSH);
				//dc.Rectangle(m_start.x,m_start.y,m_old.x,m_old.y);//�ϴ�����ƶ�����λ��֮�����ʱ�� 
				dc.SetROP2( R2_COPYPEN );
				dc.SelectObject(newpen);
				CYAxis[2*n].x= m_start.x;
				CYAxis[2*n].y= m_start.y;
				CYAxis[2*n+ 1].x= point.x+ (int)(xratio);
				CYAxis[2*n+ 1].y= point.y+ (int)(yratio);
				dc.Rectangle(m_start.x,m_start.y,CYAxis[2*n+ 1].x,CYAxis[2*n+ 1].y);
				CYPoint[p].x= m_start.x*(float(nWidth)/float(width));
				CYPoint[p].y= m_start.y*(float(nHeight)/float(height));
				n++;
				p++;

			}
			
			
		}
		else
			AfxMessageBox("�����򲻿���ȡ����");
	} 
	else
		AfxMessageBox("����ͼ���������");
	
	CDialogEx::OnLButtonUp(nFlags, point);
}


void Ccaiyang::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//������������ο�
	if( state&&m_quote )
	{
		CPen newpen(PS_SOLID,1,RGB(255,0,0));//ѡ���ɫ����
		CClientDC dc(this);
		CPen *pOldPen = dc.SelectObject(&newpen);
		dc.SetROP2( R2_NOT );//����ģʽ
		dc.SelectStockObject(NULL_BRUSH);
		dc.Rectangle(m_start.x,m_start.y,m_old.x,m_old.y);
		//dc.MoveTo( m_start );	//�����д������㵽��굱ǰλ�û���
		dc.Rectangle(m_start.x,m_start.y,point.x,point.y);
		dc.SelectObject(pOldPen); 
		m_old = point;	//��굱ǰλ������һ������ƶ��¼���������"��λ��"
	}
			CDialogEx::OnMouseMove(nFlags, point);
	
}


HBRUSH Ccaiyang::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	//�ı䴰��������Ĵ�С
	if (pWnd->GetDlgCtrlID() == IDC_STATIC1)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(213,222,243));
		hbr = CreateSolidBrush(RGB(213,222,243));
		 pDC->SelectObject(&m_font);
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC2)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(213,222,243));
		hbr = CreateSolidBrush(RGB(213,222,243));
		 pDC->SelectObject(&m_font);
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC3)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(213,222,243));
		hbr = CreateSolidBrush(RGB(213,222,243));
		 pDC->SelectObject(&m_font);
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC4)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(213,222,243));
		hbr = CreateSolidBrush(RGB(213,222,243));
		 pDC->SelectObject(&m_font);
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC5)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(213,222,243));
		hbr = CreateSolidBrush(RGB(213,222,243));
		pDC->SelectObject(&m_font);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


//��������,��������ֵ,����
void Ccaiyang::OnEndsampling()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (n< m_num1*m_classnum||pointFlag)
	{
		AfxMessageBox("����δ���");
		return;
	}
	int i, j, k;
	int t=0;
	CString str1;
	CString str2;
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	Image_kappa_evalu * MyImg = Image_kappa_evalu::getInstance();
	CString str =pMain->canshu.str;
	int bands_sec=pMain->canshu.bands_sec; //��ò�����
	int class_num=pMain->canshu.m_classnum;//��������
	int dim_num=pMain->canshu.dim_num;     //���ά��
	int window_size=pMain->window_size;//ע:Ĭ��ֵΪ10
	

	MyImg->init_GrayImg(str,bands_sec);//��ʼ���Ҷ�ͼ
	MyImg->init_Parameter(class_num,dim_num);

	
	  //Ϊ����ľ�ֵ�ͷ���ٶ�ά�ռ�
	pMain->canshu.Init_Mu =new float*[6];
	pMain->canshu.Init_Sigma = new float*[6];
	float **Init_Mu = new float*[6];
	float **Init_Sigma = new float*[6];
	  for(i=0;i<6;i++)
	  {
		  Init_Mu[i]=new float [m_classnum];
		  Init_Sigma[i]=new float [m_classnum];
		  pMain->canshu.Init_Mu[i]=new float [m_classnum];
          pMain->canshu.Init_Sigma[i]=new float [m_classnum];
		 memset(Init_Mu[i],0,m_classnum*sizeof(float));
		 memset(Init_Sigma[i],0,m_classnum*sizeof(float));
	  }

	float  *sum=new float[m_classnum]; //��¼���лҶ�ֵ�ĺ�
	float  *sum1=new float[m_classnum];//��¼���лҶ�ֵ��ƽ����

	//�Ҷ�������������ֵ�ͷ���
		for(k=0;k<m_classnum;k++)
		{
			sum[k]=0;
			sum1[k]=0;
			//sum2[k]=0;
			for(i=CYArea[2*k].y;i<=CYArea[2*k+1].y;i++)
				for(j=CYArea[2*k].x;j<=CYArea[2*k+1].x;j++)
				{
					sum[k]+=MyImg->Image_Gray[i*MyImg->xlen+j];
				}
				Init_Mu[0][k]=sum[k]/float((abs(CYArea[2*k+1].x-CYArea[2*k].x+1)*abs(CYArea[2*k+1].y-CYArea[2*k].y+1)));

				for(i=CYArea[2*k].y;i<=CYArea[2*k+1].y;i++)
					for(j=CYArea[2*k].x;j<=CYArea[2*k+1].x;j++)
					{
						sum1[k]+=pow((MyImg->Image_Gray[i*MyImg->xlen+j]-Init_Mu[0][k]),2);
					}
					Init_Sigma[0][k]=sqrt(sum1[k]/float(((CYArea[2*k+1].x-CYArea[2*k].x+1)*(CYArea[2*k+1].y-CYArea[2*k].y+1))));
		}
		

	//��������(�������Աȶȣ����أ��أ������)��������ֵ�ͷ���
		int realheight = (MyImg->ylen)-2*window_size;//�����ڵ���ʵ��С
		int realwidth=(MyImg->xlen)-2*window_size;
		int real_size=realwidth*realheight;

		if((feature_mark[1]==1)||(feature_mark[2]==1)||(feature_mark[3]==1)||(feature_mark[4]==1)||(feature_mark[5]==1))
		{
			//MyImg->GetImgTextureFeatures(window_size);//�õ���������
			//char *path="d:\\Cjieguo\\TextureFeatures.txt";
			MyImg->Init_TextureFeatures(window_size);
			FILE *fp =fopen("TextureFeatures.txt","r");
			if(fp!=NULL)
			{
				for(int index1=0;index1<real_size;index1++)
					for(int index2=0;index2<5;index2++)
						fscanf(fp,"%f ",&(MyImg->TextureFeatures[index2][index1]));
				fclose(fp);
			}


			for(t=1;t<6;t++)
			{
				for(k=0;k<m_classnum;k++)

				{
					sum[k]=0;
					sum1[k]=0;
					for(i=CYArea[2*k].y-window_size;i<=CYArea[2*k+1].y-window_size;i++)
						for(j=CYArea[2*k].x-window_size;j<=CYArea[2*k+1].x-window_size;j++)
						{
							sum[k]+=MyImg->TextureFeatures[t-1][i*realwidth+j];
						}
						Init_Mu[t][k]=sum[k]/float((abs(CYArea[2*k+1].x-CYArea[2*k].x+1)*abs(CYArea[2*k+1].y-CYArea[2*k].y+1)));

						for(i=CYArea[2*k].y-window_size;i<=CYArea[2*k+1].y-window_size;i++)
							for(j=CYArea[2*k].x-10;j<=CYArea[2*k+1].x-10;j++)
							{
								sum1[k]+=pow((MyImg->TextureFeatures[t-1][i*realwidth+j]-Init_Mu[t][k]),2);
							}
							Init_Sigma[t][k]=sqrt(sum1[k]/float(((CYArea[2*k+1].x-CYArea[2*k].x+1)*(CYArea[2*k+1].y-CYArea[2*k].y+1))));

				}
			}
		}
	for(i=0;i<6;i++)
	{
		if(feature_mark[i]==1)
		{
			feature1=i;
			break;
		}
		
	}

	//�����м�����ĳ�ֵ���MainFrm�б���
	for(i=0;i<6;i++)
		for(j=0;j<class_num;j++)
		{
			pMain->canshu.Init_Mu[i][j]=Init_Mu[i][j];
			pMain->canshu.Init_Sigma[i][j]=Init_Sigma[i][j];
		}

	/* ����ת�Ƶ�CSHOW��
	switch(feature1)
	{
	case 0:
			m_Mu_feature.SelectString(-1,"�Ҷ�");
			m_Sigma_feature.SelectString(-1,"�Ҷ�");
			str1.Format("%f",Init_Mu[0][0]);
			str2.Format("%f",Init_Sigma[0][0]);
			SetDlgItemText(IDC_MU,str1);
			SetDlgItemText(IDC_SIGMA,str2);		
		break;
	case 1:
		m_Mu_feature.SelectString(-1,"����");
		m_Sigma_feature.SelectString(-1,"����");
		str1.Format("%f",Init_Mu[1][0]);
		str2.Format("%f",Init_Sigma[1][0]);
		SetDlgItemText(IDC_MU,str1);
		SetDlgItemText(IDC_SIGMA,str2);
		break;
	case 2:
		m_Mu_feature.SelectString(-1,"�Աȶ�");
		m_Sigma_feature.SelectString(-1,"�Աȶ�");
		str1.Format("%f",Init_Mu[2][0]);
		str2.Format("%f",Init_Sigma[2][0]);
		SetDlgItemText(IDC_MU,str1);
		SetDlgItemText(IDC_SIGMA,str2);
		break;
	case 3:
		m_Mu_feature.SelectString(-1,"����");
		m_Sigma_feature.SelectString(-1,"����");
		str1.Format("%f",Init_Mu[3][0]);
		str2.Format("%f",Init_Sigma[3][0]);
		SetDlgItemText(IDC_MU,str1);
		SetDlgItemText(IDC_SIGMA,str2);
		break;
	case 4:
		m_Mu_feature.SelectString(-1,"��");
		m_Sigma_feature.SelectString(-1,"��");
		str1.Format("%f",Init_Mu[4][0]);
		str2.Format("%f",Init_Sigma[4][0]);
		SetDlgItemText(IDC_MU,str1);
		SetDlgItemText(IDC_SIGMA,str2);
		break;
	case 5:
		m_Mu_feature.SelectString(-1,"�����");
		m_Sigma_feature.SelectString(-1,"�����");
		str1.Format("%f",Init_Mu[5][0]);
		str2.Format("%f",Init_Sigma[5][0]);
		SetDlgItemText(IDC_MU,str1);
		SetDlgItemText(IDC_SIGMA,str2);
		break;
	}
	*/

	//ȡ������Ҫ��������������ֵ��������Init_Mu_real
	Init_Mu_real =new float[class_num*dim_num];
	Init_Sigma_real = new float[class_num*dim_num];
	pMain->Init_Mu_real=new float[class_num*dim_num];//�洢һ����GMM����ʱʹ��
	pMain->Init_Sigma_real=new float[class_num*dim_num];
	t=0;
	for(i=0;i<6;i++)
	{
		if(feature_mark[i]==1)
			{
				for(j=0;j<class_num;j++)
				{
					Init_Mu_real[t*class_num+j]=Init_Mu[i][j];
					Init_Sigma_real[t*class_num+j]=Init_Sigma[i][j];
					pMain->Init_Mu_real[t*class_num+j]=Init_Mu[i][j];
					pMain->Init_Sigma_real[t*class_num+j]=Init_Sigma[i][j];
				}
				t++;
		}
	}
	delete [] sum;
	delete [] sum1;
	for(i=0;i<6;i++)
	{
		delete [] Init_Mu[i];
		delete [] Init_Sigma [i];
	}
	delete [] Init_Mu;
	delete [] Init_Sigma;


}

/*����ת�Ƶ�CSHOW��
//�ı���ʾ����ڱ༭������ʾ����ֵ
void Ccaiyang::OnCbnSelchangeJunzhi()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	int class_num=pMain->canshu.m_classnum;//��������
	 k_class=m_Mu.GetCurSel();
	CString str;
	if(Init_Mu_real!=NULL)
	{
		str.Format("%f",Init_Mu_real[k_feature*class_num+k_class]); 
		SetDlgItemText(IDC_MU,str);
	}
	else
	{
		return;
	}
	
}


void Ccaiyang::OnCbnSelchangeFangcha()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	int class_num=pMain->canshu.m_classnum;//��������
	 k_class1=m_Sigma.GetCurSel();
	CString str;
	if(Init_Sigma_real!=NULL)
	{
		str.Format("%f",Init_Sigma_real[k_feature1*class_num+k_class1]); 
		SetDlgItemText(IDC_SIGMA,str);
	}
	else
	{
		return;
	}
	
}


void Ccaiyang::OnCbnSelchangeMuF()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	int class_num=pMain->canshu.m_classnum;//��������
	k_feature=m_Mu_feature.GetCurSel();
	CString str;
	if(Init_Mu_real!=NULL)
	{
		str.Format("%f",Init_Mu_real[k_feature*class_num+k_class]); 
		SetDlgItemText(IDC_MU,str);
	}
	else
	{
		return;
	}
}


void Ccaiyang::OnCbnSelchangeSigmaF()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	int class_num=pMain->canshu.m_classnum;//��������
	k_feature1=m_Sigma_feature.GetCurSel();
	CString str;
	if(Init_Sigma_real!=NULL)
	{
		str.Format("%f",Init_Sigma_real[k_feature1*class_num+k_class1]); 
		SetDlgItemText(IDC_SIGMA,str);
	}
	else
	{
		return;
	}
}
*/

BOOL Ccaiyang::OnEraseBkgnd(CDC* pDC)
{
	
	return CDialogEx::OnEraseBkgnd(pDC);
}


void Ccaiyang::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (n== m_classnum*m_num1&& pointFlag)
	{
		AfxMessageBox("��Ҫȡ�����������������²���");
		return;
	}
	if (p> 0&& (!pointFlag))
	{
		AfxMessageBox("��Ҫȡ�������㣬������ʼȡ������²���");
		return;
	}
	if (p>0)
		p--;
	n--;
	CPen newpen(PS_SOLID,1,RGB(0,255,0));
	CClientDC dc(this);
	dc.SelectObject(newpen);
	dc.SelectStockObject(NULL_BRUSH);
	dc.SetROP2( R2_NOT );
	dc.Rectangle(CYAxis[2*n].x,CYAxis[2*n].y,CYAxis[2*n+ 1].x,CYAxis[2*n+ 1].y);
	state= false;
	m_quote= true;
	Invalidate();
	CDialogEx::OnRButtonDown(nFlags, point);
}

/* ԭȡ��ģʽ����
void Ccaiyang::OnBnClickedButtonPoint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�߼��ж�
	if (n< m_classnum*m_num1&&(!pointFlag))
	{
		AfxMessageBox("��������������");
			return;
	}
	pointFlag=!pointFlag;
	if(pointFlag)
	{
		SetDlgItemText(IDC_BUTTON_POINT,"���ȡ��");
		n= m_classnum*m_num1;
		p= 0;
	}
	else
		SetDlgItemText(IDC_BUTTON_POINT,"��ʼȡ��");
	Invalidate();
}
*/

/* ԭ�˳���ť
void Ccaiyang::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
*/
//����Ӧ����
void Ccaiyang::InitChoiceMenu()
{
	CString pText;
	CMenu* Mtest = pMenu->GetSubMenu(0);
	LPSTR lpStr = pText.GetBuffer();
	pText = "1";
	Mtest->ModifyMenu(2, MF_STRING | MF_BYPOSITION, ID_CATEGORY + 1, lpStr);
	pText = "2";
	lpStr = pText.GetBuffer();
	Mtest->ModifyMenu(3, MF_STRING | MF_BYPOSITION, ID_CATEGORY + 2, lpStr);
	pText = "3";
	lpStr = pText.GetBuffer();
	Mtest->ModifyMenu(4, MF_STRING | MF_BYPOSITION, ID_CATEGORY + 3, lpStr);
	pText = "4";
	lpStr = pText.GetBuffer();
	Mtest->ModifyMenu(5, MF_STRING | MF_BYPOSITION, ID_CATEGORY + 4, lpStr);
	pText.ReleaseBuffer();
}