// Serial.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RSsystem.h"
#include "Serial.h"
#include "afxdialogex.h"
#include"SkinPPWTL.h"
#include "MainFrm.h"


// CSerial �Ի���

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


// CSerial ��Ϣ�������


void CSerial::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//MessageBox(_T("����������ϣ��뷵�ؿ�ʼ������"));
	CDialog::OnOK();
}
BOOL CSerial::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
    skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC1)->m_hWnd);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//m_font.CreatePointFont(100,_T("����"));
    //m_brush.CreateSolidBrush(RGB(213,222,243));

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

HBRUSH CSerial::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_STATIC1)
	{	
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(213,222,243));
		 pDC->SelectObject(&m_font);
		hbr = CreateSolidBrush(RGB(213,222,243));
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}





void CSerial::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	if(SrcPath=="") //�ж��ļ�·���Ƿ�Ϊ��
	{
		MessageBox(_T("������ѡ���ļ�!"));
		return;
	}
	
}
