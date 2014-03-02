
// RSsystemView.cpp : CRSsystemView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CRSsystemView ����/����

CRSsystemView::CRSsystemView()
{
	// TODO: �ڴ˴���ӹ������

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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

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


// CRSsystemView ���

#ifdef _DEBUG
void CRSsystemView::AssertValid() const
{
	CView::AssertValid();
}

void CRSsystemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRSsystemDoc* CRSsystemView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRSsystemDoc)));
	return (CRSsystemDoc*)m_pDocument;
}
#endif //_DEBUG


// CRSsystemView ��Ϣ�������


void CRSsystemView::OnSerial()
{
	// TODO: �ڴ���������������
	/*CSerial dlg;
	dlg.DoModal();*/
}


void CRSsystemView::OnParallel()
{
	// TODO: �ڴ���������������
	CParallel dlg;
	dlg.DoModal();
}


void CRSsystemView::OnDialog3()
{
	// TODO: �ڴ���������������
	if (OpenFlag==0)
	{
		MessageBox("���������ݣ�");
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
	// TODO: �ڴ���������������
	/*Canshuguji dlg;
	dlg.DoModal();*/
	
}


int CRSsystemView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CSplash wndSplash;                 //���������������ʵ��  
    wndSplash.Create(IDB_SPLASH);  
    wndSplash.CenterWindow();  
    wndSplash.UpdateWindow();          //send WM_PAINT  
    //SetTimer(1,12000,NULL); 
    Sleep(1500); 
    wndSplash.DestroyWindow();//���ٳ�ʼ���洰

	return 0;
}





void CRSsystemView::OnFileOpen()
{
	// TODO: �ڴ���������������
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
	GDALDataset * DataSet;          // ���������ݼ���Ϊ���Ϊͼ���ļ�  
     GDALAllRegister();              //ע�������������򲻿��٣���Ҫ���衣 
	 CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");//���ÿ��Զ���������ĵ�·��
     DataSet = (GDALDataset *)GDALOpen(strFilePath,GA_ReadOnly);//�ļ��Ĵ�ʹ�õ���GDALOpen����
	 pMain->canshu.str=strFilePath;

	if (DataSet == NULL)  
    {  
        AfxMessageBox("�޷���ң��ͼ��"); 
		OpenFlag=0;
        return ;  
    } 
	
	GDALRasterBand **pBand;        //���ݼ�������Ҫ�ĳɷֲ��Ρ�  
    m_Bands = DataSet->GetRasterCount();  
    pBand = new GDALRasterBand * [m_Bands]; //�½�����  
	if (pBand == NULL)  
	{  
        AfxMessageBox("�������ݼ�����ʧ��");  
        return ;  
    }  
    for (int i =0;i<m_Bands;i++)  
    {  
        pBand[i] = DataSet->GetRasterBand(i+1);//Ԥ��ȡң�еĵ�һ������,�����������ðɣ�  
        if (pBand[i] == NULL)  
        {  
            AfxMessageBox("����i�������ݼ�ʧ�ܣ�");  
            return;  
        }  
    }  

	 CDisplaySpacesDlg dlg;  
     dlg.bandCount = m_Bands;  
	 

	pMain->BandsType=dlg.BandsType;//���ݿ��xy�������سߴ� 
		if (dlg.DoModal()==IDOK) 
		{
	if (dlg.BandsType == 1)//�򿪻�ɫͼ��  
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
     poBandBlock_Gray = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nXsize*nYsize));//���仺�����ռ�  
    //RasterIO�������Խ�ͼ���ĳһ���ӿ�����д��  
    pBand[BdCGray-1]->RasterIO(GF_Read, 0, 0, nXsize,  
                nYsize, poBandBlock_Gray, nXsize, nYsize, pBand[BdCGray-1]->GetRasterDataType(), 0, 0);
	}
	else if(dlg.BandsType == 0)  //�򿪲�ɫͼ��
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
    delete DataSet; //�ͷ���Դ   
	Invalidate();
		}
	
}


void CRSsystemView::OnDraw(CDC* pDC)
{
	// TODO: �ڴ����ר�ô����/����û���
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	  CRSsystemDoc* pDoc = GetDocument();
     ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
	 if (BandsType == 1)//GDI��ͼ��  
    {  
        //����ͷ�ļ�  
        //1.ͼ�����ݿ�ͷ  
        int i,j;  
        int  nWidth = nXsize;  
        int nHeight = nYsize; 
		bands=m_Bands;
		pMain->poBandBlock_Gray = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nXsize*nYsize));//���仺�����ռ�
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
	else if (BandsType == 0)//GDI��ͼ��  
	 {  
		 //����ͷ�ļ�  
		 //1.ͼ�����ݿ�ͷ  
		 int i,j,k;  
		 int  nWidth = nXsize;  
		 int nHeight = nYsize;  
		 pMain->poBandBlock_R = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nXsize*nYsize));//���仺�����ռ�
		 pMain->poBandBlock_G = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nXsize*nYsize));//���仺�����ռ�
		 pMain->poBandBlock_B = (unsigned char*)CPLMalloc(sizeof(unsigned char)*(nXsize*nYsize));//���仺�����ռ�
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
	// TODO: �ڴ���������������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	if((pMain->Init_Mu_real!=NULL)&&(pMain->Init_Sigma_real!=NULL))
	{
		CClassificationrule dlg;
		dlg.DoModal();
	}
	else
	{
		MessageBox("������ɲ�����");
		return;
	}
	
}
