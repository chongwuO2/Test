// IteratRegion.cpp : 实现文件
//

#include "stdafx.h"
#include "RSsystem.h"
#include "IteratRegion.h"
#include "afxdialogex.h"
#include "Canshuguji.h"
#include "MainFrm.h"


// CIteratRegion 对话框

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


// CIteratRegion 消息处理程序


void CIteratRegion::OnCbnSelchangeClassf()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL CIteratRegion::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;

	//对三个下拉框进行初始化

	int i;
	CString str ;
	classnum=pMain->canshu.m_classnum;
	dimnum=pMain->canshu.dim_num;
	IteratRegion = new float[2*classnum*dimnum];//开辟数组存放迭代区间
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
			str.Format("类别%d—灰度",i+1);
			m_classname.AddString(str);
		}
	}
	if(pMain->feature_mark[1]==1)
	{
		for(i=0;i<classnum;i++)
		{
			str.Format("类别%d—能量",i+1);
			m_classname.AddString(str);
		}
	}
	if(pMain->feature_mark[2]==1)
	{
		for(i=0;i<classnum;i++)
		{
			str.Format("类别%d—对比度",i+1);
			m_classname.AddString(str);
		}
	}
	if(pMain->feature_mark[3]==1)
	{
		for(i=0;i<classnum;i++)
		{
			str.Format("类别%d—逆差矩",i+1);
			m_classname.AddString(str);
		}
	}
	if(pMain->feature_mark[4]==1)
	{
		for(i=0;i<classnum;i++)
		{
			str.Format("类别%d—熵",i+1);
			m_classname.AddString(str);
		}
	}
	if(pMain->feature_mark[5]==1)
	{
		for(i=0;i<classnum;i++)
		{
			str.Format("类别%d—相关性",i+1);
			m_classname.AddString(str);
		}
	}
	m_classname.SetCurSel(0);

	t=0;
	m_flag.SetCheck(TRUE);
	SetDlgItemInt(IDC_STEP,1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CIteratRegion::OnfinishPinput()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	int i;
	for(i=0;i<classnum*dimnum;i++)
	{
		if((IteratRegion[i]<-9)||(IteratRegion[i]>9))
		{
			MessageBox("区间输入有误或者未输入完全！");
			return;
		}

	}
	for(i=0;i<2*classnum*dimnum;i++)
	{
		pMain->IteratRegion[i]=IteratRegion[i];
	}
	pMain->step=GetDlgItemInt(IDC_STEP);//注：输入步长的限制是不大于迭代区间的最小值

	//创建参数估计窗口
	Canshuguji dlg;
	dlg.pro=&dlg;
	dlg.DoModal();
	CDialog::OnOK();
}


void CIteratRegion::OnBnClickedSavevalue()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	int n1,n2;
	GetDlgItemText(IDC_CLASSF,str);
	n1=GetDlgItemInt(IDC_REGION_START);
	n2=GetDlgItemInt(IDC_REGION_END);
	//输入迭代区间要求必须是n2>n1
	if(n2<n1)
	{
		MessageBox("迭代区间不符合条件，请重新选择！");
		return;
	}
	//按照输入类别顺序存储迭代区间
	if(str=="类别1—灰度")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别2—灰度")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别3—灰度")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别1—能量")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别2—能量")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别3—能量")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别1—对比度")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别2—对比度")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别3—对比度")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别1—逆差矩")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别2—逆差矩")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别3—逆差矩")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别1—熵")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别2—熵")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别3—熵")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别1—相关性")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别2—相关性")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}
	if(str=="类别3—相关性")
	{
		IteratRegion[2*t]=n1;
		IteratRegion[2*t+1]=n2;
		t++;
	}

	//设置下一个输入的类别特征
	CString str1;
	int n=m_classname.GetCurSel();
	if(n<classnum*dimnum)
	{
		m_classname.SetCurSel(n+1);
		if(m_flag.GetCheck()==TRUE)
		{
			str1.Format("还剩%d个区间",classnum*dimnum-n-1);
			MessageBox(str1);
		}
	}
	else
	{
		MessageBox("迭代区间输入完毕，如修改请点击修改区间，不修改请点击输入完成！");
	}
}


void CIteratRegion::OnBnClickedReselect()
{
	// TODO: 在此添加控件通知处理程序代码
	t=0;
	m_classname.SetCurSel(0);
}


void CIteratRegion::OnBnClickedFlag()
{
	// TODO: 在此添加控件通知处理程序代码

}

//修改步长函数
void CIteratRegion::OnEnChangeStep()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	
}
