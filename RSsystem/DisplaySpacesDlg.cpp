// DisplaySpacesDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RSsystem.h"
#include "DisplaySpacesDlg.h"
#include "afxdialogex.h"
#include "gdal_priv.h"
#include "MainFrm.h"


// CDisplaySpacesDlg 对话框

IMPLEMENT_DYNAMIC(CDisplaySpacesDlg, CDialogEx)

CDisplaySpacesDlg::CDisplaySpacesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDisplaySpacesDlg::IDD, pParent)
	, m_static_Gray(_T(""))
	, m_static_R(_T(""))
	, m_static_G(_T(""))
	, m_static_B(_T(""))
{

}

CDisplaySpacesDlg::~CDisplaySpacesDlg()
{
}

void CDisplaySpacesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BANDLIST, m_BandList);
	DDX_Text(pDX, IDC_STATIC_GRAY, m_static_Gray);
	DDX_Text(pDX, IDC_STATIC_R, m_static_R);
	DDX_Text(pDX, IDC_STATIC_G, m_static_G);
	DDX_Text(pDX, IDC_STATIC_B, m_static_B);
}


BEGIN_MESSAGE_MAP(CDisplaySpacesDlg, CDialogEx)
	ON_LBN_SELCANCEL(IDC_BANDLIST, &CDisplaySpacesDlg::OnSelcancelBandlist)
	ON_LBN_DBLCLK(IDC_BANDLIST, &CDisplaySpacesDlg::OnLbnDblclkBandlist)
	ON_BN_CLICKED(IDC_GRAY, &CDisplaySpacesDlg::OnGray)
	ON_BN_CLICKED(IDC_RGB, &CDisplaySpacesDlg::OnRgb)
	ON_BN_CLICKED(IDOK, &CDisplaySpacesDlg::OnOk)
	ON_LBN_SELCHANGE(IDC_BANDLIST, &CDisplaySpacesDlg::OnSelchangeBandlist)
END_MESSAGE_MAP()


// CDisplaySpacesDlg 消息处理程序


BOOL CDisplaySpacesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CheckRadioButton(IDC_GRAY,IDC_RGB,IDC_GRAY);//设置单选按钮的初始状态
    CheckRadioButton(IDC_R,IDC_B,IDC_R);
	CheckRadioButton(IDC_GRAYBAND,IDC_GRAYBAND,IDC_GRAYBAND);
	m_dispara.m_showGray=1;
	BandsType=m_dispara.m_showGray;
	GetDlgItem(IDC_R)->EnableWindow(FALSE);
	GetDlgItem(IDC_G)->EnableWindow(FALSE);
	GetDlgItem(IDC_B)->EnableWindow(FALSE);
	
	CString str;
	for(int i = 0; i < bandCount; i++)
	{
		str.Format("波段%d",i+1);
		m_BandList.AddString(str); 
	}
	
	m_BandList.GetText(0,str);
	str.TrimLeft();
	m_static_Gray=str;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDisplaySpacesDlg::OnSelcancelBandlist()
{
	// TODO: 在此添加控件通知处理程序代码
	
}


void CDisplaySpacesDlg::OnLbnDblclkBandlist()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void CDisplaySpacesDlg::OnGray()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dispara.m_showGray=1;
	GetDlgItem(IDC_GRAYBAND)->EnableWindow(TRUE);
	GetDlgItem(IDC_R)->EnableWindow(FALSE);
	GetDlgItem(IDC_G)->EnableWindow(FALSE);
	GetDlgItem(IDC_B)->EnableWindow(FALSE);
	BandsType=m_dispara.m_showGray;
}


void CDisplaySpacesDlg::OnRgb()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dispara.m_showGray=0;
	GetDlgItem(IDC_R)->EnableWindow(TRUE);
	GetDlgItem(IDC_G)->EnableWindow(TRUE);
	GetDlgItem(IDC_B)->EnableWindow(TRUE);
	GetDlgItem(IDC_GRAYBAND)->EnableWindow(FALSE);
	BandsType=m_dispara.m_showGray;
}


void CDisplaySpacesDlg::OnOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_dispara.m_showGray)
	{
		if(m_dispara.grayBand<0)
			return;
	}
	else
	{
		if(m_dispara.RBand<0||m_dispara.GBand<0||m_dispara.BBand<0)
		{
			AfxMessageBox("请选择波段");
			return;
		}
	}

	CDialogEx::OnOK();
}


void CDisplaySpacesDlg::OnSelchangeBandlist()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	UpdateData();
	int nIndex=m_BandList.GetCurSel();
	CString str;
	if(!m_dispara.m_showGray)
	{
		int result=GetCheckedRadioButton(IDC_R,IDC_B);
		if(result==IDC_R)
		{
			m_dispara.RBand=nIndex+1;
			CheckRadioButton(IDC_R,IDC_B,IDC_G);
			m_BandList.GetText(nIndex,str);
			str.TrimLeft();
			m_static_R=str;
		}
		
		if(result==IDC_G)
		{
			m_dispara.GBand=nIndex+1;
			CheckRadioButton(IDC_R,IDC_B,IDC_B);
			m_BandList.GetText(nIndex,str);
			str.TrimLeft();
			m_static_G=str;
		}	
		
		if(result==IDC_B)
		{
			m_dispara.BBand=nIndex+1;
			CheckRadioButton(IDC_R,IDC_B,IDC_R);
			m_BandList.GetText(nIndex,str);
			str.TrimLeft();
			m_static_B=str;
		}
	}
	else
	{
		CheckRadioButton(IDC_GRAYBAND,IDC_GRAYBAND,IDC_GRAYBAND);
		m_dispara.grayBand=nIndex+1;
		m_BandList.GetText(nIndex,str);
		str.TrimLeft();
		m_static_Gray=str;
		pMain->canshu.bands_sec =m_dispara.grayBand ;
	}
	
    UpdateData(FALSE);
}
