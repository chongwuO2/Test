// IteratRegion.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RSsystem.h"
#include "IteratRegion.h"
#include "afxdialogex.h"
#include "Canshuguji.h"
#include "MainFrm.h"


// CIteratRegion �Ի���

IMPLEMENT_DYNAMIC(CIteratRegion, CDialog)

CIteratRegion::CIteratRegion(CWnd* pParent /*=NULL*/)
	: CDialog(CIteratRegion::IDD, pParent)
{

}

CIteratRegion::~CIteratRegion()
{
	delete [] IteratRegion;
}

void CIteratRegion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLASSF, m_classname);
	DDX_Control(pDX, IDC_REGION_START, m_startnum);
	DDX_Control(pDX, IDC_REGION_END, m_endnum);
	DDX_Control(pDX, IDC_FLAG, m_flag);
}


BEGIN_MESSAGE_MAP(CIteratRegion, CDialog)
	ON_CBN_SELCHANGE(IDC_CLASSF, &CIteratRegion::OnCbnSelchangeClassf)
	ON_BN_CLICKED(IDOK, &CIteratRegion::OnfinishPinput)
	ON_BN_CLICKED(IDC_SAVEVALUE, &CIteratRegion::OnBnClickedSavevalue)
	ON_BN_CLICKED(IDC_RESELECT, &CIteratRegion::OnBnClickedReselect)
	ON_BN_CLICKED(IDC_FLAG, &CIteratRegion::OnBnClickedFlag)
	ON_EN_CHANGE(IDC_STEP, &CIteratRegion::OnEnChangeStep)
END_MESSAGE_MAP()


// CIteratRegion ��Ϣ�������


void CIteratRegion::OnCbnSelchangeClassf()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


BOOL CIteratRegion::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;

	//��������������г�ʼ��

	int i;
	CString str ;
	classnum=pMain->canshu.m_classnum;
	dimnum=pMain->canshu.dim_num;
	IteratRegion = new float[2*classnum*dimnum];//���������ŵ�������
	pMain->IteratRegion=new float[2*classnum*dimnum];
	for(i=-9;i<10;i++)
	{
		str.Format("%d",i);
		m_startnum.AddString(str);
		m_endnum.AddString(str);
	}
	
	if(pMain->feature_mark[0]==1)
	{
		for(i=0;i<classnum;i++)
		{
			str.Format("���%d���Ҷ�",i+1);
			m_classname.AddString(str);
		}
	}
	if(pMain->feature_mark[1]==1)
	{
		for(i=0;i<classnum;i++)
		{
			str.Format("���%d������",i+1);
			m_classname.AddString(str);
		}
	}
	if(pMain->feature_mark[2]==1)
	{
		for(i=0;i<classnum;i++)
		{
			str.Format("���%d���Աȶ�",i+1);
			m_classname.AddString(str);
		}
	}
	if(pMain->feature_mark[3]==1)
	{
		for(i=0;i<classnum;i++)
		{
			str.Format("���%d������",i+1);
			m_classname.AddString(str);
		}
	}
	if(pMain->feature_mark[4]==1)
	{
		for(i=0;i<classnum;i++)
		{
			str.Format("���%d����",i+1);
			m_classname.AddString(str);
		}
	}
	if(pMain->feature_mark[5]==1)
	{
		for(i=0;i<classnum;i++)
		{
			str.Format("���%d�������",i+1);
			m_classname.AddString(str);
		}
	}
	m_classname.SetCurSel(0);

	t=0;
	m_flag.SetCheck(TRUE);
	SetDlgItemInt(IDC_STEP,1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CIteratRegion::OnfinishPinput()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	int i;
	for(i=0;i<classnum*dimnum;i++)
	{
		if((IteratRegion[i]<-9)||(IteratRegion[i]>9))
		{
			MessageBox("���������������δ������ȫ��");
			return;
		}

	}
	for(i=0;i<2*classnum*dimnum;i++)
	{
		pMain->IteratRegion[i]=IteratRegion[i];
	}
	pMain->step=GetDlgItemInt(IDC_STEP);//ע�����벽���������ǲ����ڵ����������Сֵ

	//�����������ƴ���
	Canshuguji dlg;
	dlg.pro=&dlg;
	dlg.DoModal();
	CDialog::OnOK();
}


void CIteratRegion::OnBnClickedSavevalue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	int n1,n2;
	GetDlgItemText(IDC_CLASSF,str);
	n1=GetDlgItemInt(IDC_REGION_START);
	n2=GetDlgItemInt(IDC_REGION_END);
	//�����������Ҫ�������n2>n1
	if(n2<n1)
	{
		MessageBox("�������䲻����������������ѡ��");
		return;
	}
	//�����������˳��洢��������
	if(str=="���1���Ҷ�")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���2���Ҷ�")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���3���Ҷ�")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���1������")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���2������")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���3������")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���1���Աȶ�")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���2���Աȶ�")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���3���Աȶ�")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���1������")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���2������")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���3������")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���1����")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���2����")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���3����")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���1�������")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���2�������")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="���3�������")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}

	//������һ��������������
	CString str1;
	int n=m_classname.GetCurSel();
	if(n<classnum*dimnum)
	{
		m_classname.SetCurSel(n+1);
		if(m_flag.GetCheck()==TRUE)
		{
			str1.Format("��ʣ%d������",classnum*dimnum-n-1);
			MessageBox(str1);
		}
	}
	else
	{
		MessageBox("��������������ϣ����޸������޸����䣬���޸�����������ɣ�");
	}
}


void CIteratRegion::OnBnClickedReselect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	t=0;
	m_classname.SetCurSel(0);
}


void CIteratRegion::OnBnClickedFlag()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}

//�޸Ĳ�������
void CIteratRegion::OnEnChangeStep()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
}
