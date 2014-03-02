// Parallel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RSsystem.h"
#include "Parallel.h"
#include "afxdialogex.h"
#include "Mainfrm.h"
#include "Textwindow.h"
#include "Serial.h"

// CParallel �Ի���

IMPLEMENT_DYNAMIC(CParallel, CDialogEx)

CParallel::CParallel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParallel::IDD, pParent)
	, m_classnum(0)
{

}

CParallel::~CParallel()
{
}

void CParallel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBox1);
	DDX_Control(pDX, IDC_LIST2, m_listBox2);
	DDX_Control(pDX, IDC_EDIT1, m_leibie);
	//DDX_Control(pDX, IDC_COMBO2, m_classrule);
	DDX_Text(pDX, IDC_EDIT1, m_classnum);
}


BEGIN_MESSAGE_MAP(CParallel, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CParallel::OnBnClickedButton1)
	ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_SELSINGLE, &CParallel::OnBnClickedSelsingle)
ON_BN_CLICKED(IDC_SELALL, &CParallel::OnBnClickedSelall)
ON_BN_CLICKED(IDC_DELSINGLE, &CParallel::OnBnClickedDelsingle)
ON_BN_CLICKED(IDC_DELALL, &CParallel::OnBnClickedDelall)
ON_EN_CHANGE(IDC_EDIT1, &CParallel::OnEnChangeEdit1)
ON_BN_CLICKED(IDC_BUTTON2, &CParallel::OnBnClickedButton2)
//ON_BN_CLICKED(IDC_SERRULE, &CParallel::OnBnClickedSerrule)
//ON_BN_CLICKED(IDC_PARARULE, &CParallel::OnBnClickedPararule)
//ON_CBN_SELCHANGE(IDC_COMBO2, &CParallel::OnCbnSelchangerule)
END_MESSAGE_MAP()


// CParallel ��Ϣ�������


void CParallel::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//MessageBox(_T("����������ϣ��뷵�ؿ�ʼ������"));
	
		CDialog::OnOK();
}


BOOL CParallel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ�����ڵĲ���ѡ��ֵ
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	SetDlgItemInt(IDC_EDIT1,pMain->canshu.m_classnum) ;

	//�޸Ĵ�������
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC1)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC2)->m_hWnd);
	//skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC3)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC4)->m_hWnd);
	//m_font.CreatePointFont(100,_T("����"));
    //m_brush.CreateSolidBrush(RGB(213,222,243));

	//��ʼ��Listbox��Combobox
	 ((CListBox*)GetDlgItem(IDC_LIST1))->AddString("�Ҷ�");//��Ͽ��Ա
	 ((CListBox*)GetDlgItem(IDC_LIST1))->AddString("����");
	 ((CListBox*)GetDlgItem(IDC_LIST1))->AddString("�Աȶ�");
	 ((CListBox*)GetDlgItem(IDC_LIST1))->AddString("����");
	 ((CListBox*)GetDlgItem(IDC_LIST1))->AddString("��");
	 ((CListBox*)GetDlgItem(IDC_LIST1))->AddString("�����");


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//�޸ĸ����ؼ������弰��ɫ
HBRUSH CParallel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(213,222,243));
		hbr = CreateSolidBrush(RGB(213,222,243));
		 pDC->SelectObject(&m_font);
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC1)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(213,222,243));
		hbr = CreateSolidBrush(RGB(213,222,243));
		 pDC->SelectObject(&m_font);
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC2)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(213,222,243));
		hbr = CreateSolidBrush(RGB(213,222,243));
		 pDC->SelectObject(&m_font);
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC4)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(213,222,243));
		hbr = CreateSolidBrush(RGB(213,222,243));
		 pDC->SelectObject(&m_font);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}





void CParallel::OnBnClickedSelsingle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString item;
	int index = m_listBox1.GetCurSel();
	m_listBox1.GetText(index,item);
	m_listBox1.DeleteString(index);
	m_listBox2.AddString(item);
}


void CParallel::OnBnClickedSelall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString item;
     while(m_listBox1.GetCount() >0){
        m_listBox1.GetText(0, item);
        m_listBox1.DeleteString(0);
        m_listBox2.AddString(item);   
    };
}


void CParallel::OnBnClickedDelsingle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString item;
	int index = m_listBox2.GetCurSel();
	m_listBox2.GetText(index,item);
	m_listBox2.DeleteString(index);
	m_listBox1.AddString(item);
}


void CParallel::OnBnClickedDelall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 CString item;
     while(m_listBox2.GetCount() >0){
        m_listBox2.GetText(0, item);
        m_listBox2.DeleteString(0);
        m_listBox1.AddString(item);   
    };
}


void CParallel::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
}

//�������
void CParallel::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	m_classnum=GetDlgItemInt(IDC_EDIT1);
	pMain->canshu.m_classnum=m_classnum;
	pMain->canshu.dim_num=m_listBox2.GetListBoxInfo();
	if((m_listBox2.GetListBoxInfo()==0)||(m_classnum==0))
	{
		MessageBox("��ѡ��������");
		return;
	}

	LPCTSTR str[6]={"�Ҷ�","����","�Աȶ�","����","��","�����"};
	int i;
	for(i=0;i<6;i++)
	{
		if(m_listBox2.FindString(0,str[i])!=LB_ERR)
		{
			pMain->feature_mark[i]=1;
		}
		else
		{
			pMain->feature_mark[i]=0;
		}
	}
	if((pMain->feature_mark[0]==1)&&(m_listBox2.GetCount()==1))
	{
		return;
	}
	else
	{
		CTextwindow dlg;
		dlg.DoModal();
	}


	//��ʱΪStrategy���ٿռ䱸��
	int strategy_x=m_classnum*(m_classnum-1)/2;
	int strategy_y=pMain->canshu.dim_num+2;
	pMain->Strategy = new int *[strategy_x];
	for(i=0;i<strategy_x;i++)
	{
		pMain->Strategy[i]=new int [strategy_y];
	}
}





/*void CParallel::OnEnChangeWinsize()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
*/


/*
void CParallel::OnBnClickedSerrule()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->m_SPflag=1;
	CSerial dlg;
	dlg.DoModal();
}

void CParallel::OnBnClickedPararule()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->m_SPflag=0;
}
*/





//void CParallel::OnCbnSelchangerule()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
//	int rule;
//	rule=m_classrule.GetCurSel();
//	if(rule==0)
//	{
//		pMain->Classifyruleflag=0;
//	}
//	else
//	{
//		pMain->Classifyruleflag=1;
//	}
//}
