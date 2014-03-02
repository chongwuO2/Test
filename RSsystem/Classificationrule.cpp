// Classificationrule.cpp : 实现文件
//

#include "stdafx.h"
#include "RSsystem.h"
#include "Classificationrule.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Serial.h"
#include "IteratRegion.h"


// CClassificationrule 对话框

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


// CClassificationrule 消息处理程序


void CClassificationrule::OnBnClickedPararule()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
		MessageBox("请选择特征使用规则！");
		return;
	}
	if (m_SPflag)
	{
		CString filePath("");
		GetDlgItemText(IDC_PATH,filePath);
		//读入规则矩阵
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
			MessageBox("无法打开该文件！");
			return;
		}

	}
	

	CDialog::OnOK();
}


BOOL CClassificationrule::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString("最小距离");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString("最小错误率");
	m_classrule.SetCurSel(0);

	GetDlgItem(IDC_SELDAG)->ShowWindow(FALSE);
	GetDlgItem(IDC_SELBT)->ShowWindow(FALSE);
	GetDlgItem(IDC_RULE_FILE)->ShowWindow(FALSE);
	GetDlgItem(IDC_PATH)->ShowWindow(FALSE);
	GetDlgItem(IDC_OPEN_RULEFILE)->ShowWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//打开规则文件
void CClassificationrule::OnOpenRulefile()
{
	// TODO: 在此添加控件通知处理程序代码
	//打开规则文件
	char szFilter[]="(*.txt)|*.txt|All Files(*.*)|*.*||";
	CString filePath("");
	CFileDialog m_opendlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_EXPLORER |OFN_ALLOWMULTISELECT,szFilter);
	if(m_opendlg.DoModal()==IDOK)
	{
		VERIFY(filePath = m_opendlg.GetPathName());
	}
	SetDlgItemText(IDC_PATH,filePath);


}

//特征串行使用规则中选中DAG
void CClassificationrule::OnSeldag()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->D_B_flag=0;
}

//特征串行使用规则中选中二叉树
void CClassificationrule::OnSelbt()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->D_B_flag=1;
}
