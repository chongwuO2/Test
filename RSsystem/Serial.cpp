// Serial.cpp : 实现文件
//

#include "stdafx.h"
#include "RSsystem.h"
#include "Serial.h"
#include "afxdialogex.h"
#include"SkinPPWTL.h"
#include "MainFrm.h"


// CSerial 对话框

IMPLEMENT_DYNAMIC(CSerial, CDialogEx)

CSerial::CSerial(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSerial::IDD, pParent)
{

}

CSerial::~CSerial()
{
}

void CSerial::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSerial, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CSerial::OnBnClickedButton2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CSerial::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSerial 消息处理程序


void CSerial::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//MessageBox(_T("参数输入完毕！请返回开始采样！"));
	CDialog::OnOK();
}
BOOL CSerial::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
    skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC1)->m_hWnd);
	// TODO:  在此添加额外的初始化
	//m_font.CreatePointFont(100,_T("宋体"));
    //m_brush.CreateSolidBrush(RGB(213,222,243));

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH CSerial::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC1)
	{	
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(213,222,243));
		 pDC->SelectObject(&m_font);
		hbr = CreateSolidBrush(RGB(213,222,243));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}





void CSerial::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter = _T("Files(*.text;*.doc)|*.text;*.doc|All files(*.*)|*.*||");
	CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST, strFilter, NULL);
	if (dlg.DoModal() == IDOK)                      
	{
		SrcPath = dlg.GetPathName();
		//m_ImgFileName = dlg.GetFileTitle();
		//m_ImgExtName = dlg.GetFileExt();
		//CString fileName = dlg.GetFileTitle();
		//	SetWindowText(fileName);
	}	
	else
	{
		return;
	}
	if(SrcPath=="") //判断文件路径是否为空
	{
		MessageBox(_T("请重新选择文件!"));
		return;
	}
	
}
