// Ccaiyang.cpp : 实现文件

#include "stdafx.h"
#include "RSsystem.h"
#include "Ccaiyang.h"
#include "afxdialogex.h"
#include "Show.h"
#include "SkinPPWTL.h"
#include "MainFrm.h"
#include "gdal_priv.h"
#include "Image_kappa_evaluation.h"


// Ccaiyang 对话框

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

	//菜单响应部分
	ON_BN_CLICKED(IDM_SHOW, &Ccaiyang::OnClickedShow)
	ON_BN_CLICKED(IDC_RESAMPLE, &Ccaiyang::OnBnClickedResample)
	ON_BN_CLICKED(IDC_ENDSAMPLING, &Ccaiyang::OnEndsampling)
	ON_BN_CLICKED(ID_CATEGORY + 1, &Ccaiyang::OnChoiceCategory1)
	ON_BN_CLICKED(ID_CATEGORY + 2, &Ccaiyang::OnChoiceCategory2)
	ON_BN_CLICKED(ID_CATEGORY + 3, &Ccaiyang::OnChoiceCategory3)
	ON_BN_CLICKED(ID_CATEGORY + 4, &Ccaiyang::OnChoiceCategory4)
	//ON_EN_CHANGE(IDC_EDIT1, &Ccaiyang::OnEnChangeEdit1)
	//采样响应部分
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
	// TODO:  在此添加额外的初始化

	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	nWidth = pMain->nXsize;  
	nHeight = pMain->nYsize;
	SetWindowPos(&wndTopMost,0,0,nWidth+23,nHeight+78,SWP_NOMOVE|SWP_SHOWWINDOW);//23为客户区左右边框，78为客户区上下边框
	/*
	//设定图像显示区域
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
	//初始化显示字体
	//skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC1)->m_hWnd);//修改字体
	//skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC2)->m_hWnd);
	//skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC3)->m_hWnd);
	//skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC4)->m_hWnd);
	//skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC5)->m_hWnd);
	//m_font.CreatePointFont(150,_T("宋体"));//修改所有控件的字体
    //m_brush.CreateSolidBrush(RGB(213,222,243));//修改空间背景颜色
	
	
	
	//设定均值方差中的类
	//CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	//初始化
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
	//初始化菜单
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

		char* buf;	//类别数序号通道
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
		MessageBox("请输入类别数！");
		OnCancel();
	}

	//测试：类采样次数
	m_num1= 2;				
	if (CYArea== NULL)
	{
		CYArea= new Point[2*m_num1*m_classnum];
		CYAxis= new Point[2*(m_num1*m_classnum+ MAXNUM_POINT)];
	}
	

	pointFlag= 0;			//点采样
	n= 0;					//采样区域计数
	p= 0;					//采样点计数
	Init_Mu_real=NULL;
	Init_Sigma_real=NULL;
	k_feature=k_feature1=0;
	k_class=k_class1=0;
	return TRUE; 
	// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
	
}


void Ccaiyang::OnPaint()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CDC* pDC=GetDC();
	//绘制图像部分
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;   
	BandsType=pMain->BandsType;
	if(BandsType==1)
	{
		int i,j;  
		nWidth = pMain->nXsize;  
		nHeight = pMain->nYsize;
		poBandBlock_Gray = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nWidth*nHeight));//分配缓存空间
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

		//2.颜色表  
		for (i = 0;i < 256;i++)     
		{  
			pBmpInfo->bmiColors[i].rgbRed   = i;   
			pBmpInfo->bmiColors[i].rgbGreen = i;    
			pBmpInfo->bmiColors[i].rgbBlue  = i;      
			pBmpInfo->bmiColors[i].rgbReserved = 0;  

		} 

		//建立数据区     
		LONG LineBytes = (nWidth*8+31)/32*4;  
		LPBYTE pData = (LPBYTE)new char[LineBytes*nHeight];
		//为各像素赋值！！！！！！  
		for (i=0;i<nHeight;i++)  
		{  
			for (j=0;j<nWidth;j++)  
			{  
				pData[(nHeight-i-1)*LineBytes + j] = poBandBlock_Gray[i*nWidth+j];//此表达式有待研究  
			}  
		}	

		SetStretchBltMode(pDC->m_hDC,BLACKONWHITE|WHITEONBLACK); //设置指定设备环境中的位图拉伸模式 
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
		poBandBlock_R = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nWidth*nHeight));//分配缓存空间
		poBandBlock_G = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nWidth*nHeight));
		poBandBlock_B = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nWidth*nHeight));
		//将存储空间中的数据复制到当前显示的缓存区
		for (i=0;i<nHeight;i++)  
		{  
			for (j=0;j<nWidth;j++)  
			{ 
				poBandBlock_R[i*nWidth+j]= pMain->poBandBlock_R[i*nWidth+j];	//此表达式有待研究
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
		//2.颜色表  
		for (i = 0;i < 256;i++)  
		{  
			pBmpInfo->bmiColors[i].rgbRed   = i;  
			pBmpInfo->bmiColors[i].rgbGreen = i;  
			pBmpInfo->bmiColors[i].rgbBlue  = i;  
			pBmpInfo->bmiColors[i].rgbReserved = 0;  
		}  
		//建立数据区  
		LONG LineBytes = (nWidth*24+31)/32*4;  
		LPBYTE pData = (LPBYTE)new char[LineBytes*nHeight*3];  
		//为各像素赋值！！！！！！  
		for (i=0;i<nHeight;i++)  
		{  
			for (j=0,k=0;j<nWidth,k<3*nWidth;j++,k+=3)  
			{  
				//  pData[(nHeight-i-1)*LineBytes + j] = pDoc->poBandBlock_Gray[i*nWidth+j];//此表达式有待研究  
				pData[(nHeight-i-1)*LineBytes + k] = poBandBlock_B[i*nWidth + j];  
				pData[(nHeight-i-1)*LineBytes + k+1] = poBandBlock_G[i*nWidth + j];  
				pData[(nHeight-i-1)*LineBytes + k+2] = poBandBlock_R[i*nWidth + j];  
			}  
		}  

		SetStretchBltMode(pDC->m_hDC,BLACKONWHITE|WHITEONBLACK); //设置指定设备环境中的位图拉伸模式 
		StretchDIBits(pDC->m_hDC, 0, 0, width, height, 0, 0, nWidth, nHeight,   
			pData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);  		
		UpdateWindow();  
		delete pBmpInfo;  
		delete pData;  

	}
	//绘制采样矩形部分
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
	pText = "1 √";
	LPSTR lpStr = pText.GetBuffer();
	CMenu* Mtest = pMenu->GetSubMenu(0);
	Mtest->ModifyMenu(2, MF_STRING | MF_BYPOSITION, ID_CATEGORY + 1, lpStr);
	pText.ReleaseBuffer();
}
void Ccaiyang::OnChoiceCategory2()
{
	InitChoiceMenu();
	CString pText;
	pText = "2 √";
	LPSTR lpStr = pText.GetBuffer();
	CMenu* Mtest = pMenu->GetSubMenu(0);
	Mtest->ModifyMenu(3, MF_STRING | MF_BYPOSITION, ID_CATEGORY + 2, lpStr);
	pText.ReleaseBuffer();
}
void Ccaiyang::OnChoiceCategory3()
{
	InitChoiceMenu();
	CString pText;
	pText = "1 √";
	LPSTR lpStr = pText.GetBuffer();
	CMenu* Mtest = pMenu->GetSubMenu(0);
	Mtest->ModifyMenu(4, MF_STRING | MF_BYPOSITION, ID_CATEGORY + 3, lpStr);
	pText.ReleaseBuffer();
}
void Ccaiyang::OnChoiceCategory4()
{
	InitChoiceMenu();
	CString pText;
	pText = "1 √";
	LPSTR lpStr = pText.GetBuffer();
	CMenu* Mtest = pMenu->GetSubMenu(0);
	Mtest->ModifyMenu(5, MF_STRING | MF_BYPOSITION, ID_CATEGORY + 4, lpStr);
	pText.ReleaseBuffer();
}
void Ccaiyang::OnClickedShow()
{
	// TODO: 在此添加控件通知处理程序代码
	if (Init_Mu_real== NULL)
	{
		AfxMessageBox("请先进行采样");
		return;
	}
	CShow dlg;
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	//TEST : 传入数据
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
//	// TODO: 在此添加控件通知处理程序代码
//	CString str;
//	n=0;
//	if(GetDlgItemText(IDC_BUTTON7,str),str==_T("开始取点"))
//	{
//		SetDlgItemText(IDC_BUTTON7,_T("重新取点"));
//	}
//	else
//	{
//		SetDlgItemText(IDC_BUTTON7,_T("开始取点"));
//		
//	}
//	
//	
//}

/* 原类采样次数输入
void Ccaiyang::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
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
	// TODO:  在此添加控件通知处理程序代码
}
*/

void Ccaiyang::OnBnClickedResample()
{
	// TODO: 在此添加控件通知处理程序代码
	n= 0;
	p= 0;
	pointFlag= 0;
	m_quote= true;
	state= false;
	//((CEdit*)(GetDlgItem(IDC_EDIT1)))->SetReadOnly(FALSE);
	//SetDlgItemText(IDC_BUTTON_POINT,"开始取点");
	Invalidate();
}


void Ccaiyang::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//不执行采样
	if ((n== m_classnum*m_num1||p> 0)&& (!pointFlag))
		return;
	//执行采样
	if (point.y<= height)	//是否为图像区域
	{	
		bool existTexture = (point.x>= windowsize*xratio)& (point.x<= width- windowsize*xratio)
			& (point.y>= windowsize*yratio)& (point.y<= height- windowsize*yratio);
		if ((!textureFlag)|| existTexture)		//是否为纹理可提取区域
		{
			m_quote= 1;
			if (pointFlag)
			//点模式
			{
				m_start= point;
				state= false;
			} 
			//区域模式
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
			AfxMessageBox("该区域不可提取纹理");
	} 
	else
		AfxMessageBox("请在图像区域采样");


	CDialogEx::OnLButtonDown(nFlags, point);
}


void Ccaiyang::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//不执行采样
	if ((n== m_classnum*m_num1||p> 0)&& (!pointFlag))	//区域采样或点采样完毕
		return;
	
	//执行采样
	if (point.y<= height)	//是否为图像区域
	{	
		bool existTexture = (point.x>= windowsize*xratio)& (point.x<= width- windowsize*xratio)
			& (point.y>= windowsize*yratio)& (point.y<= height- windowsize*yratio);
		if ((!textureFlag)|| existTexture)	//是否为纹理可提取区域
		{
			if (m_start== point)
				m_quote= 0;
			//区域模式
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
				//dc.Rectangle(m_start.x,m_start.y,m_old.x,m_old.y);//上次鼠标移动到的位置之间的临时线 
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
				if(n>=m_num1*m_classnum)//当超过输入的个数，则停止画矩形
				{
					state=false;
					m_quote=0;
					//ReleaseCapture();
					Invalidate();
				}
			}
			//取点模式
			else if (m_start== point&& pointFlag)
			{				
				if(p>=50)//当超过输入的个数，则停止画点
				{
					AfxMessageBox("已达到最大取点数目(50),请点击完成取点");
					return;
				}
				CPen newpen(PS_SOLID,1,RGB(255,0,0));
				CClientDC dc(this);
				//CPen *pOldPen = dc.SelectObject(&newpen);
				//dc.SetROP2( R2_NOT );
				dc.SelectStockObject(NULL_BRUSH);
				//dc.Rectangle(m_start.x,m_start.y,m_old.x,m_old.y);//上次鼠标移动到的位置之间的临时线 
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
			AfxMessageBox("该区域不可提取纹理");
	} 
	else
		AfxMessageBox("请在图像区域采样");
	
	CDialogEx::OnLButtonUp(nFlags, point);
}


void Ccaiyang::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//区域采样画矩形框
	if( state&&m_quote )
	{
		CPen newpen(PS_SOLID,1,RGB(255,0,0));//选择红色画笔
		CClientDC dc(this);
		CPen *pOldPen = dc.SelectObject(&newpen);
		dc.SetROP2( R2_NOT );//绘制模式
		dc.SelectStockObject(NULL_BRUSH);
		dc.Rectangle(m_start.x,m_start.y,m_old.x,m_old.y);
		//dc.MoveTo( m_start );	//这两行代码从起点到鼠标当前位置画线
		dc.Rectangle(m_start.x,m_start.y,point.x,point.y);
		dc.SelectObject(pOldPen); 
		m_old = point;	//鼠标当前位置在下一次鼠标移动事件看来就是"旧位置"
	}
			CDialogEx::OnMouseMove(nFlags, point);
	
}


HBRUSH Ccaiyang::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	//改变窗口中字体的大小
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
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


//采样结束,计算各类均值,方差
void Ccaiyang::OnEndsampling()
{
	// TODO: 在此添加控件通知处理程序代码
	if (n< m_num1*m_classnum||pointFlag)
	{
		AfxMessageBox("采样未完成");
		return;
	}
	int i, j, k;
	int t=0;
	CString str1;
	CString str2;
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	Image_kappa_evalu * MyImg = Image_kappa_evalu::getInstance();
	CString str =pMain->canshu.str;
	int bands_sec=pMain->canshu.bands_sec; //获得波段数
	int class_num=pMain->canshu.m_classnum;//获得类别数
	int dim_num=pMain->canshu.dim_num;     //获得维数
	int window_size=pMain->window_size;//注:默认值为10
	

	MyImg->init_GrayImg(str,bands_sec);//初始化灰度图
	MyImg->init_Parameter(class_num,dim_num);

	
	  //为纹理的均值和方差开辟二维空间
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

	float  *sum=new float[m_classnum]; //记录所有灰度值的和
	float  *sum1=new float[m_classnum];//记录所有灰度值的平方和

	//灰度特征的样本均值和方差
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
		

	//纹理特征(能量，对比度，逆差矩，熵，相关性)的样本均值和方差
		int realheight = (MyImg->ylen)-2*window_size;//开窗口的真实大小
		int realwidth=(MyImg->xlen)-2*window_size;
		int real_size=realwidth*realheight;

		if((feature_mark[1]==1)||(feature_mark[2]==1)||(feature_mark[3]==1)||(feature_mark[4]==1)||(feature_mark[5]==1))
		{
			//MyImg->GetImgTextureFeatures(window_size);//得到纹理特征
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

	//将所有计算出的初值存进MainFrm中保存
	for(i=0;i<6;i++)
		for(j=0;j<class_num;j++)
		{
			pMain->canshu.Init_Mu[i][j]=Init_Mu[i][j];
			pMain->canshu.Init_Sigma[i][j]=Init_Sigma[i][j];
		}

	/* 功能转移到CSHOW类
	switch(feature1)
	{
	case 0:
			m_Mu_feature.SelectString(-1,"灰度");
			m_Sigma_feature.SelectString(-1,"灰度");
			str1.Format("%f",Init_Mu[0][0]);
			str2.Format("%f",Init_Sigma[0][0]);
			SetDlgItemText(IDC_MU,str1);
			SetDlgItemText(IDC_SIGMA,str2);		
		break;
	case 1:
		m_Mu_feature.SelectString(-1,"能量");
		m_Sigma_feature.SelectString(-1,"能量");
		str1.Format("%f",Init_Mu[1][0]);
		str2.Format("%f",Init_Sigma[1][0]);
		SetDlgItemText(IDC_MU,str1);
		SetDlgItemText(IDC_SIGMA,str2);
		break;
	case 2:
		m_Mu_feature.SelectString(-1,"对比度");
		m_Sigma_feature.SelectString(-1,"对比度");
		str1.Format("%f",Init_Mu[2][0]);
		str2.Format("%f",Init_Sigma[2][0]);
		SetDlgItemText(IDC_MU,str1);
		SetDlgItemText(IDC_SIGMA,str2);
		break;
	case 3:
		m_Mu_feature.SelectString(-1,"逆差矩");
		m_Sigma_feature.SelectString(-1,"逆差矩");
		str1.Format("%f",Init_Mu[3][0]);
		str2.Format("%f",Init_Sigma[3][0]);
		SetDlgItemText(IDC_MU,str1);
		SetDlgItemText(IDC_SIGMA,str2);
		break;
	case 4:
		m_Mu_feature.SelectString(-1,"熵");
		m_Sigma_feature.SelectString(-1,"熵");
		str1.Format("%f",Init_Mu[4][0]);
		str2.Format("%f",Init_Sigma[4][0]);
		SetDlgItemText(IDC_MU,str1);
		SetDlgItemText(IDC_SIGMA,str2);
		break;
	case 5:
		m_Mu_feature.SelectString(-1,"相关性");
		m_Sigma_feature.SelectString(-1,"相关性");
		str1.Format("%f",Init_Mu[5][0]);
		str2.Format("%f",Init_Sigma[5][0]);
		SetDlgItemText(IDC_MU,str1);
		SetDlgItemText(IDC_SIGMA,str2);
		break;
	}
	*/

	//取出所需要的特征的样本初值，并存入Init_Mu_real
	Init_Mu_real =new float[class_num*dim_num];
	Init_Sigma_real = new float[class_num*dim_num];
	pMain->Init_Mu_real=new float[class_num*dim_num];//存储一份在GMM估计时使用
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

/*功能转移到CSHOW类
//改变显示项后在编辑框中显示出均值
void Ccaiyang::OnCbnSelchangeJunzhi()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	int class_num=pMain->canshu.m_classnum;//获得类别数
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
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	int class_num=pMain->canshu.m_classnum;//获得类别数
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
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	int class_num=pMain->canshu.m_classnum;//获得类别数
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
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	int class_num=pMain->canshu.m_classnum;//获得类别数
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (n== m_classnum*m_num1&& pointFlag)
	{
		AfxMessageBox("若要取消采样区域，请点击重新采样");
		return;
	}
	if (p> 0&& (!pointFlag))
	{
		AfxMessageBox("若要取消采样点，请点击开始取点或重新采样");
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

/* 原取点模式输入
void Ccaiyang::OnBnClickedButtonPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	//逻辑判定
	if (n< m_classnum*m_num1&&(!pointFlag))
	{
		AfxMessageBox("请先完成区域采样");
			return;
	}
	pointFlag=!pointFlag;
	if(pointFlag)
	{
		SetDlgItemText(IDC_BUTTON_POINT,"完成取点");
		n= m_classnum*m_num1;
		p= 0;
	}
	else
		SetDlgItemText(IDC_BUTTON_POINT,"开始取点");
	Invalidate();
}
*/

/* 原退出按钮
void Ccaiyang::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
*/
//非响应函数
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