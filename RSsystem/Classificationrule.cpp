// Classificationrule.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RSsystem.h"
#include "Classificationrule.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Serial.h"
#include "IteratRegion.h"


// CClassificationrule �Ի���

IMPLEMENT_DYNAMIC(CClassificationrule, CDialog)

CClassificationrule::CClassificationrule(CWnd* pParent /*=NULL*/)
	: CDialog(CClassificationrule::IDD, pParent)
{

}

CClassificationrule::~CClassificationrule()
{
}

void CClassificationrule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_classrule);
}


BEGIN_MESSAGE_MAP(CClassificationrule, CDialog)
	ON_BN_CLICKED(IDC_PARARULE, &CClassificationrule::OnBnClickedPararule)
	ON_BN_CLICKED(IDC_SERRULE, &CClassificationrule::OnBnClickedSerrule)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CClassificationrule::OnCbnSelchangerule)
	ON_BN_CLICKED(IDOK, &CClassificationrule::OnBnClickedOk)
	ON_BN_CLICKED(IDC_OPEN_RULEFILE, &CClassificationrule::OnOpenRulefile)
	ON_BN_CLICKED(IDC_SELDAG, &CClassificationrule::OnSeldag)
	ON_BN_CLICKED(IDC_SELBT, &CClassificationrule::OnSelbt)
END_MESSAGE_MAP()


// CClassificationrule ��Ϣ�������


void CClassificationrule::OnBnClickedPararule()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	m_SPflag=0;
	pMain->m_SPflag=0;
	GetDlgItem(IDC_SELDAG)->ShowWindow(FALSE);
	GetDlgItem(IDC_SELBT)->ShowWindow(FALSE);
	GetDlgItem(IDC_RULE_FILE)->ShowWindow(FALSE);
	GetDlgItem(IDC_PATH)->ShowWindow(FALSE);
	GetDlgItem(IDC_OPEN_RULEFILE)->ShowWindow(FALSE);
	((CButton*)(GetDlgItem(IDC_SERRULE)))->SetCheck(FALSE);
}


void CClassificationrule::OnBnClickedSerrule()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
    m_SPflag=1;	
	pMain->m_SPflag=1;
	
	GetDlgItem(IDC_SELDAG)->ShowWindow(TRUE);
	GetDlgItem(IDC_SELBT)->ShowWindow(TRUE);
	GetDlgItem(IDC_RULE_FILE)->ShowWindow(TRUE);
	GetDlgItem(IDC_PATH)->ShowWindow(TRUE);
	GetDlgItem(IDC_OPEN_RULEFILE)->ShowWindow(TRUE);
	((CButton*)(GetDlgItem(IDC_PARARULE)))->SetCheck(FALSE);
}

void CClassificationrule::OnCbnSelchangerule()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
		int rule;
		rule=m_classrule.GetCurSel();
	  if(rule==0)
		{
			pMain->Classifyruleflag=0;
		}
		else
		{
			pMain->Classifyruleflag=1;
		}
}


void CClassificationrule::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	int strategy_x= pMain->canshu.m_classnum*(pMain->canshu.m_classnum-1)/2;
	int strategy_y=pMain->canshu.dim_num+2;
	if((m_SPflag==1)||(m_SPflag==0))
	{
		CIteratRegion dlg;
		dlg.DoModal();
	}
	else
	{
		MessageBox("��ѡ������ʹ�ù���");
		return;
	}
	if (m_SPflag)
	{
		CString filePath("");
		GetDlgItemText(IDC_PATH,filePath);
		//����������
		FILE *fp =fopen(filePath,"r");
		if(fp!=NULL)
		{
			for(int index1=0;index1<strategy_x;index1++)
				for(int index2=0;index2<strategy_y;index2++)
				{

					if (index2==strategy_y-1)
						fscanf(fp,"%d\n ",&(pMain->Strategy[index1][index2]));
					else
						fscanf(fp,"%d ",&(pMain->Strategy[index1][index2]));
				}
				fclose(fp);
		}
		else
		{
			MessageBox("�޷��򿪸��ļ���");
			return;
		}

	}
	

	CDialog::OnOK();
}


BOOL CClassificationrule::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString("��С����");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString("��С������");
	m_classrule.SetCurSel(0);

	GetDlgItem(IDC_SELDAG)->ShowWindow(FALSE);
	GetDlgItem(IDC_SELBT)->ShowWindow(FALSE);
	GetDlgItem(IDC_RULE_FILE)->ShowWindow(FALSE);
	GetDlgItem(IDC_PATH)->ShowWindow(FALSE);
	GetDlgItem(IDC_OPEN_RULEFILE)->ShowWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//�򿪹����ļ�
void CClassificationrule::OnOpenRulefile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�򿪹����ļ�
	char szFilter[]="(*.txt)|*.txt|All Files(*.*)|*.*||";
	CString filePath("");
	CFileDialog m_opendlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_EXPLORER |OFN_ALLOWMULTISELECT,szFilter);
	if(m_opendlg.DoModal()==IDOK)
	{
		VERIFY(filePath = m_opendlg.GetPathName());
	}
	SetDlgItemText(IDC_PATH,filePath);


}

//��������ʹ�ù�����ѡ��DAG
void CClassificationrule::OnSeldag()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->D_B_flag=0;
}

//��������ʹ�ù�����ѡ�ж�����
void CClassificationrule::OnSelbt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->D_B_flag=1;
}
