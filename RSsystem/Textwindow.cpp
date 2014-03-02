// Textwindow.cpp : 实现文件
//

#include "stdafx.h"
#include "RSsystem.h"
#include "Textwindow.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CTextwindow 对话框

IMPLEMENT_DYNAMIC(CTextwindow, CDialog)

CTextwindow::CTextwindow(CWnd* pParent /*=NULL*/)
	: CDialog(CTextwindow::IDD, pParent)
{

}

CTextwindow::~CTextwindow()
{
}

void CTextwindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTextwindow, CDialog)
	ON_EN_CHANGE(IDC_WINDOW, &CTextwindow::OnEnChangeWindow)
END_MESSAGE_MAP()


// CTextwindow 消息处理程序


void CTextwindow::OnEnChangeWindow()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->window_size=GetDlgItemInt(IDC_WINDOW);
}


BOOL CTextwindow::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemInt(IDC_WINDOW,10);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
