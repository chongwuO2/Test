
// RSsystemView.cpp : CRSsystemView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "RSsystem.h"
#endif

#include "RSsystemDoc.h"
#include "RSsystemView.h"
#include "Parallel.h"
#include "Ccaiyang.h"
#include "Canshuguji.h"
#include "Splash.h"
#include "gdal_priv.h"
#include "DisplaySpacesDlg.h"
#include "MainFrm.h"
#include "Classificationrule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRSsystemView

IMPLEMENT_DYNCREATE(CRSsystemView, CView)

BEGIN_MESSAGE_MAP(CRSsystemView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDM_SERIAL, &CRSsystemView::OnSerial)
	ON_COMMAND(IDM_PARALLEL, &CRSsystemView::OnParallel)
	ON_COMMAND(IDM_DIALOG3, &CRSsystemView::OnDialog3)
//	ON_WM_PAINT()
//	ON_COMMAND(IDD_CANSHUGUJI, &CRSsystemView::OnIddCanshuguji)
//	ON_UPDATE_COMMAND_UI(IDD_CANSHUGUJI, &CRSsystemView::OnUpdateIddCanshuguji)
//	ON_COMMAND(IDD_CANSHUGUJI, &CRSsystemView::OnIddCanshuguji)
ON_COMMAND(IDM_CANSHUGUJI, &CRSsystemView::OnCanshuguji)
ON_WM_CREATE()
//ON_COMMAND(IDM_OPENIMG, &CRSsystemView::OnOpenimg)
//ON_COMMAND(ID_OPENFILE, &CRSsystemView::OnOpenfile)
//ON_WM_CTLCOLOR()
//ON_COMMAND(IDM_CANSHU, &CRSsystemView::OnCanshu)
ON_COMMAND(ID_FILE_OPEN, &CRSsystemView::OnFileOpen)
ON_COMMAND(IDC_ASSESS, &CRSsystemView::OnAssess)
END_MESSAGE_MAP()

// CRSsystemView 构造/析构

CRSsystemView::CRSsystemView()
{
	// TODO: 在此处添加构造代码

}

CRSsystemView::~CRSsystemView()
{
	CPLFree(poBandBlock_Gray);
	CPLFree(poBandBlock_R);
	CPLFree(poBandBlock_G);
	CPLFree(poBandBlock_B);
}

BOOL CRSsystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}


void CRSsystemView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRSsystemView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRSsystemView 诊断

#ifdef _DEBUG
void CRSsystemView::AssertValid() const
{
	CView::AssertValid();
}

void CRSsystemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRSsystemDoc* CRSsystemView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRSsystemDoc)));
	return (CRSsystemDoc*)m_pDocument;
}
#endif //_DEBUG


// CRSsystemView 消息处理程序


void CRSsystemView::OnSerial()
{
	// TODO: 在此添加命令处理程序代码
	/*CSerial dlg;
	dlg.DoModal();*/
}


void CRSsystemView::OnParallel()
{
	// TODO: 在此添加命令处理程序代码
	CParallel dlg;
	dlg.DoModal();
}


void CRSsystemView::OnDialog3()
{
	// TODO: 在此添加命令处理程序代码
	if (OpenFlag==0)
	{
		MessageBox("请输入数据！");
		return;

	}
	else
	{
		Ccaiyang dlg;
		dlg.DoModal();    
    }
	
}

void CRSsystemView::OnCanshuguji()
{
	// TODO: 在此添加命令处理程序代码
	/*Canshuguji dlg;
	dlg.DoModal();*/
	
}


int CRSsystemView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CSplash wndSplash;                 //创建启动窗口类的实例  
    wndSplash.Create(IDB_SPLASH);  
    wndSplash.CenterWindow();  
    wndSplash.UpdateWindow();          //send WM_PAINT  
    //SetTimer(1,12000,NULL); 
    Sleep(1500); 
    wndSplash.DestroyWindow();//销毁初始画面窗

	return 0;
}





void CRSsystemView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	CRSsystemDoc* pDoc = GetDocument();
	char szFilter[]="(*.jpg;*.jpeg;*.gif;*.bmp;*.tif;*.mat)|*.jpg;*.jpeg;*.gif;*.bmp;*.tif;*.mat|All Files(*.*)|*.*||";
	CString filePath("");
	CFileDialog m_opendlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_EXPLORER |OFN_ALLOWMULTISELECT,szFilter);
	if(m_opendlg.DoModal()==IDOK)
	{
		VERIFY(filePath = m_opendlg.GetPathName());
	}
	CString strFilePath(filePath);
	GDALDataset * DataSet;          // 在这里数据集即为理解为图像文件  
     GDALAllRegister();              //注册驱动，这项万不可少，必要步骤。 
	 CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");//设置可以读入带有中文的路径
     DataSet = (GDALDataset *)GDALOpen(strFilePath,GA_ReadOnly);//文件的打开使用的是GDALOpen函数
	 pMain->canshu.str=strFilePath;

	if (DataSet == NULL)  
    {  
        AfxMessageBox("无法打开遥感图像"); 
		OpenFlag=0;
        return ;  
    } 
	
	GDALRasterBand **pBand;        //数据集下最重要的成分波段。  
    m_Bands = DataSet->GetRasterCount();  
    pBand = new GDALRasterBand * [m_Bands]; //新建波段  
	if (pBand == NULL)  
	{  
        AfxMessageBox("创建数据集波段失败");  
        return ;  
    }  
    for (int i =0;i<m_Bands;i++)  
    {  
        pBand[i] = DataSet->GetRasterBand(i+1);//预读取遥感的第一个波段,因该是这个作用吧！  
        if (pBand[i] == NULL)  
        {  
            AfxMessageBox("创建i波段数据集失败！");  
            return;  
        }  
    }  

	 CDisplaySpacesDlg dlg;  
     dlg.bandCount = m_Bands;  
	 

	pMain->BandsType=dlg.BandsType;//数据块的xy方向像素尺寸 
		if (dlg.DoModal()==IDOK) 
		{
	if (dlg.BandsType == 1)//打开灰色图像  
	{  
      BandsType = dlg.BandsType;  
      BdCGray = dlg.m_dispara.grayBand;  
    if (pBand[BdCGray] == NULL)  
    {  
        return ;  
	}  
	
	nXsize = pBand[BdCGray-1]->GetXSize();  
	nYsize = pBand[BdCGray-1]->GetYSize(); 
	pMain->nXsize=nXsize;
	pMain->nYsize=nYsize;
	 pMain->BandsType=BandsType;
     poBandBlock_Gray = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nXsize*nYsize));//分配缓冲区空间  
    //RasterIO函数可以将图像的某一个子块读入或写入  
    pBand[BdCGray-1]->RasterIO(GF_Read, 0, 0, nXsize,  
                nYsize, poBandBlock_Gray, nXsize, nYsize, pBand[BdCGray-1]->GetRasterDataType(), 0, 0);
	}
	else if(dlg.BandsType == 0)  //打开彩色图像
	{  
		BandsType = dlg.BandsType;  
		BdCR=dlg.m_dispara.RBand;
		BdCG=dlg.m_dispara.GBand;
		BdCB=dlg.m_dispara.BBand;

		
		int nXsizeR,nXsizeG,nXsizeB;  
		int nYsizeR,nYsizeG,nYsizeB;  
		
		nXsizeR = pBand[BdCR-1]->GetXSize();  
		nYsizeR = pBand[BdCR-1]->GetYSize();  
		nXsizeG = pBand[BdCG-1]->GetXSize();  
		nYsizeG = pBand[BdCG-1]->GetYSize();  
		nXsizeB = pBand[BdCB-1]->GetXSize();  
		nYsizeB = pBand[BdCB-1]->GetYSize();  
		
		nXsize = nXsizeR;  
		nYsize = nYsizeR;  
		pMain->nXsize=nXsize;
		pMain->nYsize=nYsize;
		pMain->BandsType=BandsType;
		
		poBandBlock_R = (BYTE*)CPLMalloc(sizeof(BYTE)*(nXsizeR*nYsizeR));  
		poBandBlock_G = (BYTE*)CPLMalloc(sizeof(BYTE)*(nXsizeG*nYsizeG));  
		poBandBlock_B = (BYTE*)CPLMalloc(sizeof(BYTE)*(nXsizeB*nYsizeB));  
  
        pBand[BdCR-1]->RasterIO(GF_Read,0,0,nXsizeR,nYsizeR,poBandBlock_R,nXsizeR,nYsizeR,pBand[BdCR-1]->GetRasterDataType(),0,0);  
		pBand[BdCG-1]->RasterIO(GF_Read,0,0,nXsizeG,nYsizeG,poBandBlock_G,nXsizeG,nYsizeG,pBand[BdCG-1]->GetRasterDataType(),0,0);  
		pBand[BdCB-1]->RasterIO(GF_Read,0,0,nXsizeB,nYsizeB,poBandBlock_B,nXsizeB,nYsizeB,pBand[BdCB-1]->GetRasterDataType(),0,0);  
  
}  
	pDoc->UpdateAllViews(NULL);  
    delete DataSet; //释放资源   
	Invalidate();
		}
	
}


void CRSsystemView::OnDraw(CDC* pDC)
{
	// TODO: 在此添加专用代码和/或调用基类
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	  CRSsystemDoc* pDoc = GetDocument();
     ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
	 if (BandsType == 1)//GDI绘图法  
    {  
        //数据头文件  
        //1.图像数据块头  
        int i,j;  
        int  nWidth = nXsize;  
        int nHeight = nYsize; 
		bands=m_Bands;
		pMain->poBandBlock_Gray = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nXsize*nYsize));//分配缓冲区空间
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
				pMain->poBandBlock_Gray[i*nWidth+j]= poBandBlock_Gray[i*nWidth+j];
            }  
        }  
	
        SetStretchBltMode(pDC->m_hDC,BLACKONWHITE|WHITEONBLACK);  
        StretchDIBits(pDC->m_hDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight,   
            pData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);  
          OpenFlag=1;
        UpdateWindow();  
        delete pBmpInfo;  
        delete pData;  
  
    } 
	else if (BandsType == 0)//GDI绘图法  
	 {  
		 //数据头文件  
		 //1.图像数据块头  
		 int i,j,k;  
		 int  nWidth = nXsize;  
		 int nHeight = nYsize;  
		 pMain->poBandBlock_R = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nXsize*nYsize));//分配缓冲区空间
		 pMain->poBandBlock_G = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nXsize*nYsize));//分配缓冲区空间
		 pMain->poBandBlock_B = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nXsize*nYsize));//分配缓冲区空间
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
				 pMain->poBandBlock_R[i*nWidth+j]= poBandBlock_R[i*nWidth+j];
				 pMain->poBandBlock_G[i*nWidth+j]= poBandBlock_G[i*nWidth+j];
				 pMain->poBandBlock_B[i*nWidth+j]= poBandBlock_B[i*nWidth+j];
			 }  
		 }  
      
		 SetStretchBltMode(pDC->m_hDC,BLACKONWHITE|WHITEONBLACK);  
		 StretchDIBits(pDC->m_hDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight, 
			 pData, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);  
		 
		 OpenFlag=1;
		 UpdateWindow();  
		 delete pBmpInfo;  
		 delete pData;  
	 }  
	else
	{
		 OpenFlag=0;
	}
}


void CRSsystemView::OnAssess()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	if((pMain->Init_Mu_real!=NULL)&&(pMain->Init_Sigma_real!=NULL))
	{
		CClassificationrule dlg;
		dlg.DoModal();
	}
	else
	{
		MessageBox("请先完成采样！");
		return;
	}
	
}
