// Textparameter.cpp : 实现文件
//

#include "stdafx.h"
#include "RSsystem.h"
#include "Textparameter.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CTextparameter 对话框

IMPLEMENT_DYNAMIC(CTextparameter, CDialog)

CTextparameter::CTextparameter(CWnd* pParent /*=NULL*/)
	: CDialog(CTextparameter::IDD, pParent)
{

}

CTextparameter::~CTextparameter()
{
}

void CTextparameter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WSIZE, m_Windowsize);
}


BEGIN_MESSAGE_MAP(CTextparameter, CDialog)
	ON_EN_CHANGE(IDC_WSIZE, &CTextparameter::OnEnChangeWsize)
END_MESSAGE_MAP()


// CTextparameter 消息处理程序

BOOL CTextparameter::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	SetDlgItemInt(IDC_D_VALUE,1);
	SetDlgItemInt(IDC_GRAY_SCALE,32);
	SetDlgItemInt(IDC_WSIZE,10);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CTextparameter::OnEnChangeWsize()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	int window_size=GetDlgItemInt(IDC_WSIZE);
	pMain->window_size=window_size;
}
