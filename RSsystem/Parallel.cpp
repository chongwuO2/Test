// Parallel.cpp : 实现文件
//

#include "stdafx.h"
#include "RSsystem.h"
#include "Parallel.h"
#include "afxdialogex.h"
#include "Mainfrm.h"
#include "Textwindow.h"
#include "Serial.h"

// CParallel 对话框

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


// CParallel 消息处理程序


void CParallel::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//MessageBox(_T("参数输入完毕！请返回开始采样！"));
	
		CDialog::OnOK();
}


BOOL CParallel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化窗口的参数选择值
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	SetDlgItemInt(IDC_EDIT1,pMain->canshu.m_classnum) ;

	//修改窗口字体
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC1)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC2)->m_hWnd);
	//skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC3)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC4)->m_hWnd);
	//m_font.CreatePointFont(100,_T("宋体"));
    //m_brush.CreateSolidBrush(RGB(213,222,243));

	//初始化Listbox和Combobox
	 ((CListBox*)GetDlgItem(IDC_LIST1))->AddString("灰度");//组合框成员
	 ((CListBox*)GetDlgItem(IDC_LIST1))->AddString("能量");
	 ((CListBox*)GetDlgItem(IDC_LIST1))->AddString("对比度");
	 ((CListBox*)GetDlgItem(IDC_LIST1))->AddString("逆差矩");
	 ((CListBox*)GetDlgItem(IDC_LIST1))->AddString("熵");
	 ((CListBox*)GetDlgItem(IDC_LIST1))->AddString("相关性");


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//修改各个控件的字体及颜色
HBRUSH CParallel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
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
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}





void CParallel::OnBnClickedSelsingle()
{
	// TODO: 在此添加控件通知处理程序代码
	CString item;
	int index = m_listBox1.GetCurSel();
	m_listBox1.GetText(index,item);
	m_listBox1.DeleteString(index);
	m_listBox2.AddString(item);
}


void CParallel::OnBnClickedSelall()
{
	// TODO: 在此添加控件通知处理程序代码
	CString item;
     while(m_listBox1.GetCount() >0){
        m_listBox1.GetText(0, item);
        m_listBox1.DeleteString(0);
        m_listBox2.AddString(item);   
    };
}


void CParallel::OnBnClickedDelsingle()
{
	// TODO: 在此添加控件通知处理程序代码
	CString item;
	int index = m_listBox2.GetCurSel();
	m_listBox2.GetText(index,item);
	m_listBox2.DeleteString(index);
	m_listBox1.AddString(item);
}


void CParallel::OnBnClickedDelall()
{
	// TODO: 在此添加控件通知处理程序代码
	 CString item;
     while(m_listBox2.GetCount() >0){
        m_listBox2.GetText(0, item);
        m_listBox2.DeleteString(0);
        m_listBox1.AddString(item);   
    };
}


void CParallel::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	
}

//保存参数
void CParallel::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	m_classnum=GetDlgItemInt(IDC_EDIT1);
	pMain->canshu.m_classnum=m_classnum;
	pMain->canshu.dim_num=m_listBox2.GetListBoxInfo();
	if((m_listBox2.GetListBoxInfo()==0)||(m_classnum==0))
	{
		MessageBox("请选择特征！");
		return;
	}

	LPCTSTR str[6]={"灰度","能量","对比度","逆差矩","熵","相关性"};
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


	//此时为Strategy开辟空间备用
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
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
*/


/*
void CParallel::OnBnClickedSerrule()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->m_SPflag=1;
	CSerial dlg;
	dlg.DoModal();
}

void CParallel::OnBnClickedPararule()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->m_SPflag=0;
}
*/





//void CParallel::OnCbnSelchangerule()
//{
//	// TODO: 在此添加控件通知处理程序代码
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
