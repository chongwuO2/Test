// Textwindow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RSsystem.h"
#include "Textwindow.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CTextwindow �Ի���

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


// CTextwindow ��Ϣ�������


void CTextwindow::OnEnChangeWindow()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->window_size=GetDlgItemInt(IDC_WINDOW);
}


BOOL CTextwindow::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetDlgItemInt(IDC_WINDOW,10);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
