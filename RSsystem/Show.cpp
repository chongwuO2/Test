// Show.cpp : 实现文件
//

#include "stdafx.h"
#include "RSsystem.h"
#include "Show.h"
#include "afxdialogex.h"
#include "SkinPPWTL.h"
#include "MainFrm.h"

// CShow 对话框

IMPLEMENT_DYNAMIC(CShow, CDialog)

CShow::CShow(CWnd* pParent /*=NULL*/)
	: CDialog(CShow::IDD, pParent)
{

}

CShow::~CShow()
{
}

void CShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_combo_category);
	DDX_Control(pDX, IDC_COMBO_FEATURE, m_combo_feature);
}


BEGIN_MESSAGE_MAP(CShow, CDialog)
	ON_WM_CTLCOLOR()
//	ON_BN_CLICKED(IDOK, &CShow::OnBnClickedOk)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_CATEGORY, &CShow::OnCbnSelchangeComboCategory)
	ON_CBN_SELCHANGE(IDC_COMBO_FEATURE, &CShow::OnCbnSelchangeComboFeature)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CShow 消息处理程序


BOOL CShow::OnInitDialog()
{
	CDialog::OnInitDialog();
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC_TITLE)->m_hWnd);
	// TODO:  在此添加额外的初始化
	//m_font.CreatePointFont(100,_T("宋体"));
    //m_brush.CreateSolidBrush(RGB(213,222,243));
	//初始化数值
	CString str1;
	CString str2;
	CMainFrame *pMain= (CMainFrame *)AfxGetApp()->m_pMainWnd;
	xaxis_interval= (xend- xstart)/ (5* xvalue_interval);
	count_num= (xend- xstart+ xvalue_interval)/ xvalue_interval;
	sortdata = NULL;
	if (!flag)
	{
		//初始化COMBO
		CString str;
		int flag = 0;
		feature_name = new int[m_classnum];
		if(m_classnum > 0)
		{
			for(int i = 0; i < m_classnum; i++)
			{
				str.Format("%d",i+1);
				m_combo_category.AddString(str); 
			}
		}
		if(feature_mark[0]==1)
		{
			m_combo_feature.AddString("灰度");
			feature_name[flag] = 0;
			if (flag== 0)
			{
				str1.Format("%f",pMain->canshu.Init_Mu[0][0]);
				str2.Format("%f",pMain->canshu.Init_Sigma[0][0]);
				SetDlgItemText(IDC_EDIT_MU,str1);
				SetDlgItemText(IDC_EDIT_SIGMA,str2);	
			}
			flag++;
		}
		if(feature_mark[1]==1)
		{
			m_combo_feature.AddString("能量");
			feature_name[flag] = 1;
			if (flag== 0)
			{
				str1.Format("%f",pMain->canshu.Init_Mu[1][0]);
				str2.Format("%f",pMain->canshu.Init_Sigma[1][0]);
				SetDlgItemText(IDC_EDIT_MU,str1);
				SetDlgItemText(IDC_EDIT_SIGMA,str2);
			}
			flag++;
		}
		if(feature_mark[2]==1)
		{
			m_combo_feature.AddString("对比度");
			feature_name[flag] = 2;
			if (flag== 0)
			{
				str1.Format("%f",pMain->canshu.Init_Mu[2][0]);
				str2.Format("%f",pMain->canshu.Init_Sigma[2][0]);
				SetDlgItemText(IDC_EDIT_MU,str1);
				SetDlgItemText(IDC_EDIT_SIGMA,str2);
			}
			flag++;
		}
		if(feature_mark[3]==1)
		{
			m_combo_feature.AddString("逆差矩");
			feature_name[flag] = 3;
			if (flag== 0)
			{
				str1.Format("%f",pMain->canshu.Init_Mu[3][0]);
				str2.Format("%f",pMain->canshu.Init_Sigma[3][0]);
				SetDlgItemText(IDC_EDIT_MU,str1);
				SetDlgItemText(IDC_EDIT_SIGMA,str2);
			}
			flag++;
		}
		if(feature_mark[4]==1)
		{
			m_combo_feature.AddString("熵");
			feature_name[flag] = 4;
			if (flag== 0)
			{
				str1.Format("%f",pMain->canshu.Init_Mu[4][0]);
				str2.Format("%f",pMain->canshu.Init_Sigma[4][0]);
				SetDlgItemText(IDC_EDIT_MU,str1);
				SetDlgItemText(IDC_EDIT_SIGMA,str2);
			}
			flag++;
		}
		if(feature_mark[5]==1)
		{
			m_combo_feature.AddString("相关性");
			feature_name[flag] = 5;
			if (flag== 0)
			{
				str1.Format("%f",pMain->canshu.Init_Mu[5][0]);
				str2.Format("%f",pMain->canshu.Init_Sigma[5][0]);
				SetDlgItemText(IDC_EDIT_MU,str1);
				SetDlgItemText(IDC_EDIT_SIGMA,str2);
			}
			flag++;
		}
		m_combo_category.SetCurSel(0);
		m_combo_feature.SetCurSel(0);

		//初始化sortdata
		curcategory_index = 0;
		curfeature_index = 0;
		SHOWCalc_sortdata();

		//初始化countPixel
		countPixel = new int[count_num];
		SHOWBucket_init();
		SHOWBucket_sort(sumPixel,count_num);//将当前类别的数据放入桶中

		//初始化maxPixel
		maxPixel = SHOWMaxBucket(count_num);
	} 
	else
	{
		//初始化控件
		m_combo_category.ShowWindow(FALSE);
		m_combo_feature.ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_FEATURE)-> ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_CATEGORY)-> ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TITLE)->SetWindowText("Kappa分布图");
		//初始化sortdata
		Image_kappa_evalu* MyImg= Image_kappa_evalu::getInstance();
		sumPixel= MyImg->SizeofAllKappa;
		sortdata= MyImg->AllKappa;

		//初始化countPixel
		countPixel = new int[count_num];
		SHOWBucket_init();
		SHOWBucket_sort(sumPixel,count_num);//将当前类别的数据放入桶中

		//初始化maxPixel
		maxPixel = SHOWMaxBucket(count_num);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CShow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE)
	{	
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(213,222,243));
		 pDC->SelectObject(&m_font);
		hbr = CreateSolidBrush(RGB(213,222,243));
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CShow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	//获取CDC对象
	CDC *pDC = GetDC();
	//初始化字体
	//m_font.CreateFont(14,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,   CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY,DEFAULT_PITCH   |   FF_SWISS,"Consolas");  
	//创建一个宽度为1的黑色钢笔
	//Pen curPen = new Pen(Brushes.Black, 1);
	int xoffset= 75;
	int pi= 2- (int)(!flag);
	//绘制并标识坐标刻度
	pDC -> MoveTo(50+ xaxis_interval*pi+ xoffset, 240);
	pDC -> LineTo(50+ xaxis_interval*pi+ xoffset, 242);
	pDC -> MoveTo(50+ xaxis_interval*2*pi+ xoffset, 240);
	pDC -> LineTo(50+ xaxis_interval*2*pi+ xoffset, 242);
	pDC -> MoveTo(50+ xaxis_interval*3*pi+ xoffset, 240);
	pDC -> LineTo(50+ xaxis_interval*3*pi+ xoffset, 242);
	pDC -> MoveTo(50+ xaxis_interval*4*pi+ xoffset, 240);
	pDC -> LineTo(50+ xaxis_interval*4*pi+ xoffset, 242);
	pDC -> MoveTo(50+ xaxis_interval*5*pi+ xoffset, 240);
	pDC -> LineTo(50+ xaxis_interval*5*pi+ xoffset, 242);
	pDC -> MoveTo(48+ xoffset, 40);
	pDC -> LineTo(50+ xoffset, 40);
	//绘制坐标轴上数字
	pDC -> SelectObject(&m_font);     
	m_font.DeleteObject();
	pDC -> SetBkMode(TRANSPARENT);
	pDC -> SetTextColor(RGB(0,0,0));
	CString str;
	if (flag)
	{		
		//绘制坐标轴
		pDC -> MoveTo( 50+ xoffset, 240);
		pDC -> LineTo(270+ xoffset, 240);
		pDC -> MoveTo( 50+ xoffset, 240);
		pDC -> LineTo( 50+ xoffset,  30);
		pDC -> TextOut( 285+ xoffset, 240,  "值");
		pDC -> TextOut(  10+ xoffset,  15, "概率");
		//
		str.Format("%.0f",xstart); 
		pDC -> TextOut( 46+ xoffset, 242, str);
		str.Format("%.2f",xstart+xaxis_interval*xvalue_interval);
		pDC -> TextOut( 79+ xoffset, 242, str);
		str.Format("%.2f",xstart+xaxis_interval*xvalue_interval*2);
		pDC -> TextOut(119+ xoffset, 242, str);
		str.Format("%.2f",xstart+xaxis_interval*xvalue_interval*3);
		pDC -> TextOut(159+ xoffset, 242, str);
		str.Format("%.2f",xstart+xaxis_interval*xvalue_interval*4);
		pDC -> TextOut(199+ xoffset, 242, str);
		str.Format("%.2f",xstart+xaxis_interval*xvalue_interval*5);
		pDC -> TextOut(239+ xoffset, 242, str);
		pDC -> TextOut( 34+ xoffset, 234,  "0");
		str.Format("%.2f",(float)(maxPixel)/sumPixel); 
		pDC -> TextOut( 10+ xoffset,  34, str);
	} 
	else
	{
		//绘制坐标轴
		pDC -> MoveTo( 50+ xoffset, 240);
		pDC -> LineTo(320+ xoffset, 240);
		pDC -> MoveTo( 50+ xoffset, 240);
		pDC -> LineTo( 50+ xoffset,  30);
		pDC -> TextOut( 335+ xoffset, 240,  "值");
		pDC -> TextOut(  10+ xoffset,  15, "概率");
		//
		str.Format("%.0f",xstart); 
		pDC -> TextOut( 46+ xoffset, 242, str);
		str.Format("%.0f",xstart+xaxis_interval);
		pDC -> TextOut( 92+ xoffset, 242, str);
		str.Format("%.0f",xstart+xaxis_interval*2);
		pDC -> TextOut(139+ xoffset, 242, str);
		str.Format("%.0f",xstart+xaxis_interval*3);
		pDC -> TextOut(189+ xoffset, 242, str);
		str.Format("%.0f",xstart+xaxis_interval*4);
		pDC -> TextOut(239+ xoffset, 242, str);
		str.Format("%.0f",xstart+xaxis_interval*5);
		pDC -> TextOut(289+ xoffset, 242, str);
		pDC -> TextOut( 34+ xoffset, 234,  "0");
		str.Format("%.2f",(float)(maxPixel)/sumPixel); 
		pDC -> TextOut( 10+ xoffset,  34, str);
	}
	
	//绘制直方图
	double temp = 0;
	for (int i = 0; i < count_num; i++)
	{
		//纵坐标长度
		temp = 200.0 * countPixel[i] / maxPixel;
		pDC -> MoveTo(50 + i*pi+ xoffset, 240);
		pDC -> LineTo(50 + i*pi+ xoffset, 240 - (int)temp);
	}
	//释放对象
	//curPen.Dispose();
}


void CShow::OnCbnSelchangeComboCategory()
{
	// TODO: 在此添加控件通知处理程序代码

	// 直方图刷新部分
	curcategory_index = m_combo_category.GetCurSel();
	m_combo_category.SetCurSel(curcategory_index);
	// 重新生成sortdata
	SHOWCalc_sortdata();

	// 重新生成countPixel
	delete countPixel;
	countPixel = new int[count_num];
	SHOWBucket_init();
	SHOWBucket_sort(sumPixel,count_num);//将当前类别的数据放入桶中

	//重新赋值maxPixel
	maxPixel = SHOWMaxBucket(count_num);
	RedrawWindow();

	//编辑框刷新部分
	CString str;
	CMainFrame *pMain= (CMainFrame *)AfxGetApp()->m_pMainWnd;
	int class_num= pMain->canshu.m_classnum;//获得类别数
	int k_class= m_combo_category.GetCurSel();
	int k_feature= m_combo_feature.GetCurSel();
	str.Format("%f",pMain->Init_Mu_real[k_feature*class_num+k_class]); 
	SetDlgItemText(IDC_EDIT_MU,str);
	str.Format("%f",pMain->Init_Sigma_real[k_feature*class_num+k_class]); 
	SetDlgItemText(IDC_EDIT_SIGMA,str);
}


void CShow::OnCbnSelchangeComboFeature()
{
	// TODO: 在此添加控件通知处理程序代码

	// 直方图刷新部分
	curfeature_index = m_combo_feature.GetCurSel();
	m_combo_feature.SetCurSel(curfeature_index);
	// 重新生成sortdata
	SHOWCalc_sortdata();

	// 重新生成countPixel
	delete countPixel;
	countPixel = new int[count_num];
	SHOWBucket_init();
	SHOWBucket_sort(sumPixel,count_num);//将当前类别的数据放入桶中

	//重新赋值maxPixel
	maxPixel = SHOWMaxBucket(count_num);
	RedrawWindow();

	//编辑框刷新部分
	CString str;
	CMainFrame *pMain= (CMainFrame *)AfxGetApp()->m_pMainWnd;
	int class_num= pMain->canshu.m_classnum;//获得类别数
	int k_class= m_combo_category.GetCurSel();
	int k_feature= m_combo_feature.GetCurSel();
	str.Format("%f",pMain->Init_Mu_real[k_feature*class_num+k_class]); 
	SetDlgItemText(IDC_EDIT_MU,str);
	str.Format("%f",pMain->Init_Sigma_real[k_feature*class_num+k_class]); 
	SetDlgItemText(IDC_EDIT_SIGMA,str);

}



// CShow 数据处理函数

void CShow::SHOWCalc_sortdata()
{
	if (sortdata != NULL)
		delete sortdata;
	//纹理文件读取
	Image_kappa_evalu* MyImg = Image_kappa_evalu::getInstance();
	int realheight = (MyImg->ylen)-2*window_size;//开窗口的真实大小
	int realwidth=(MyImg->xlen)-2*window_size;
	int real_size=realwidth*realheight;
	//灰度
	int* data_num = new int[sample_num];	//记录每次采样的像素数目
	int data_offset;
	int index_offset;
	int f_offset;
	if (feature_name[curfeature_index]== 0)
	{
		f_offset= curcategory_index*sample_num;
		sumPixel = 0;                           //
		for(int i= 0; i < sample_num;i++)
		{
			data_num[i] = (CYArea[(i+ f_offset)*2+ 1].y- CYArea[(i+ f_offset)*2].y+ 1)*(CYArea[(i+ f_offset)*2+ 1].x- CYArea[(i+ f_offset)*2].x+ 1);
			sumPixel = sumPixel + data_num[i];
		}
		sortdata = new float[sumPixel];
		for (int i= 0;i< sample_num;i++)
		{	
			data_offset= CYArea[(i+ f_offset)*2].y * nWidth + CYArea[(i+ f_offset)*2].x;
			if (i!= 0)
				index_offset+= data_num[i-1];
			else
				index_offset= 0;
			for(int j= 1;j<= data_num[i];j++)
			{	
				if (j% (CYArea[(i+ f_offset)*2+1].x- CYArea[(i+ f_offset)*2].x+ 1)== 0)
					data_offset+= nWidth- (CYArea[i*2+ 1].x- CYArea[i*2].x+ 1);	
				sortdata[index_offset+ j- 1]= MyImg-> Image_Gray[data_offset+ j];		
			}
		}
	}

	else //纹理
	{

		f_offset= curcategory_index*sample_num;
		sumPixel = 0;                           //
		for(int i= 0; i< sample_num;i++)
		{
			data_num[i]= (CYArea[(i+ f_offset)*2+ 1].y- CYArea[(i+ f_offset)*2].y+ 1)*(CYArea[(i+ f_offset)*2+ 1].x- CYArea[(i+ f_offset)*2].x+ 1);
			sumPixel= sumPixel + data_num[i];
		}
		sortdata = new float[sumPixel];
		for (int i= 0;i< sample_num;i++)
		{	
			data_offset= (CYArea[(i+ f_offset)*2].y- 10) * realwidth + (CYArea[(i+ f_offset)*2].x- 10);
			if (i!= 0)
				index_offset+= data_num[i-1];
			else
				index_offset= 0;
			for(int j= 1;j<= data_num[i];j++)
			{	
				if (j% (CYArea[(i+ f_offset)*2+1].x- CYArea[(i+ f_offset)*2].x+ 1)== 0)
					data_offset+= realwidth- (CYArea[i*2+ 1].x- CYArea[i*2].x+ 1);
				sortdata[index_offset+ j- 1]= MyImg-> TextureFeatures[feature_name[curfeature_index]- 1][data_offset+ j];		
			}
		}

	}

	delete data_num;
}

void CShow::SHOWBucket_sort(int length, int max_x)
{
	for (int i = 0; i < length; i++ )
	{
		float temp = sortdata[i];
		if (temp>= 0)
			countPixel[(int)(max_x * (float)(sortdata[i]/(xend- xstart+ xvalue_interval)) )]++;
		else
			countPixel[0]++;
	}
}


void CShow::SHOWBucket_init()
{
	for (int i= 0;i< count_num;i++)
	{
		countPixel[i] = 0;
	}
}

int CShow::SHOWMaxBucket(int max_x)
{
	int maxpixel = 0;
	for (int i = 0; i< max_x; i++ )
	{
		if (countPixel[i]>maxpixel)
			maxpixel = countPixel[i];	
	}
	return maxpixel;
}


BOOL CShow::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialog::OnEraseBkgnd(pDC);
}

