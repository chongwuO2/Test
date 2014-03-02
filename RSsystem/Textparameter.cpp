// Textparameter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RSsystem.h"
#include "Textparameter.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CTextparameter �Ի���

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


// CTextparameter ��Ϣ�������

BOOL CTextparameter::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	SetDlgItemInt(IDC_D_VALUE,1);
	SetDlgItemInt(IDC_GRAY_SCALE,32);
	SetDlgItemInt(IDC_WSIZE,10);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CTextparameter::OnEnChangeWsize()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	int window_size=GetDlgItemInt(IDC_WSIZE);
	pMain->window_size=window_size;
}
