// Canshuguji.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RSsystem.h"
#include "Canshuguji.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "kappa_evaluation.h"
#include "Image_kappa_evaluation.h"
#include "Show.h"

// Canshuguji �Ի���

IMPLEMENT_DYNAMIC(Canshuguji, CDialog)

Canshuguji::Canshuguji(CWnd* pParent /*=NULL*/)
	: CDialog(Canshuguji::IDD, pParent)
{

}

Canshuguji::~Canshuguji()
{
}

void Canshuguji::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KAPPA, m_Kapppa);
	DDX_Control(pDX, IDC_EST_PROGRESS, m_ProcessControl);
}


BEGIN_MESSAGE_MAP(Canshuguji, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDCANCEL, &Canshuguji::OnBnClickedCancel)
	ON_COMMAND(IDM_CANSHUGUJI, &Canshuguji::OnPara_estimation)
	ON_BN_CLICKED(IDC_TEST, &Canshuguji::OnBnClickedTest)
	ON_BN_CLICKED(IDC_BUTTON_KAPPA, &Canshuguji::OnBnClickedButtonKappa)
END_MESSAGE_MAP()


// Canshuguji ��Ϣ�������


BOOL Canshuguji::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC)->m_hWnd);
	//m_font.CreatePointFont(100,_T("����"));
    //m_brush.CreateSolidBrush(RGB(213,222,243));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH Canshuguji::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
	{	
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(213,222,243));
		 pDC->SelectObject(&m_font);
		hbr = CreateSolidBrush(RGB(213,222,243));
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void Canshuguji::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}

void Canshuguji::OnPara_estimation()
{
	// TODO: �ڴ���������������
	/*CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	Image_kappa_evalu* MyImg = Image_kappa_evalu::getInstance();
	int i=0;
	double itol=0.00001;
	int maxiter =10000;
	int class_num = MyImg->class_numb;
	int dim_num=MyImg->dim_numb;
	float * Init_Mu =new float[class_num*dim_num];
	float *Init_Sigma=new float[class_num*dim_num];
	float * Init_Weight =new float[class_num*dim_num];


	Init_Mu = pMain->Init_Mu_real;
	Init_Sigma = pMain->Init_Sigma_real;
	for(i=0;i<class_num*dim_num;i++)
	{
		Init_Weight[i]=1/float(class_num*dim_num);
	}


	if(dim_num==1)
	{
		MyImg->GMM_singl(MyImg->Image_Gray,MyImg->imgsize,itol,maxiter,Init_Weight,Init_Mu,Init_Sigma);	//��άGMM����
		MyImg->kappa_evalu_singl();//Kappa����
		
	}

	//���kappaֵ������
	CString str ;
	str.Format("%f",MyImg->kappa);
	SetDlgItemText(IDC_KAPPA,str);

	delete [] Init_Mu;
	delete []Init_Sigma;
	delete [] Init_Weight;*/
}

//���԰�ť
void Canshuguji::OnBnClickedTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	Image_kappa_evalu* MyImg = Image_kappa_evalu::getInstance();
	int i,j;

	int Data_size;
	int windowsize=pMain->window_size;// ������ȡ������С
	int realimgsize=(MyImg->xlen-2*windowsize)*(MyImg->ylen-2*windowsize);
	int class_num = MyImg->class_numb;
	int dim_num=MyImg->dim_numb;
	float step=pMain->step;
	MyImg->pro=pro;//�����ڵ�ַ��MyImg��

	int classifyruleflag=pMain->Classifyruleflag;//ѡ����С�������С�����ʷ������
	float * Init_Mu =new float[class_num*dim_num];
	float *Init_Sigma=new float[class_num*dim_num];
	float * Init_Weight =new float[class_num*dim_num];
	float * IteratRegion= new float[2*class_num*dim_num];//���û��Լ�����
	IteratRegion =pMain->IteratRegion;

	Init_Mu = pMain->Init_Mu_real;
	Init_Sigma = pMain->Init_Sigma_real;
	for(i=0;i<class_num*dim_num;i++)
	{
		Init_Weight[i]=1/float(class_num*dim_num);
	}

		//��data��ֵ����Ϊ��ά�Ͷ�ά
	float * Data;
	float **Data_mul;
	if(dim_num==1)
	{
		if(pMain->feature_mark[0]==1)
		{ 
			Data_size=MyImg->imgsize;
			Data=new float[Data_size];
			Data=MyImg->Image_Gray;
		}
		else
		{
			Data_size=realimgsize;
			Data=new float[Data_size];
			for(i=1;i<6;i++)
			{
				if(pMain->feature_mark[i]==1)
				{
					Data=MyImg->TextureFeatures[i-1];
					break;
				}

			}
		}

	}
	else if (dim_num>1)
	{
		
		Data_size=realimgsize;
		Data_mul=new float*[dim_num];
		for(i=0;i<dim_num;i++)
		{
			Data_mul[i]=new float [Data_size];
			memset(Data_mul[i],0,sizeof(float)*Data_size);
		}
		int k;
		//�����ά���������Ҷȣ����ȰѻҶ���Ϣ����Ϊrealimgsize
		if(pMain->feature_mark[0]==1)
		{
			int t=0;
			for(i=windowsize;i<MyImg->ylen-windowsize;i++)
				for(j=windowsize;j<MyImg->xlen-windowsize;j++)
				{
					Data_mul[0][t]=MyImg->Image_Gray[i*MyImg->xlen+j];
					t++;
				}
				//��ѡ��������������Ϣ���Data_mul
				k=1;
				for(i=1;i<6;i++)
				{
					if(pMain->feature_mark[i]==1)
					{
						Data_mul[k]=MyImg->TextureFeatures[i-1];
						k++;
					}
				}
		}
		else
		{
			k=0;
			for(i=1;i<6;i++)
			{
				if(pMain->feature_mark[i]==1)
				{
					Data_mul[k]=MyImg->TextureFeatures[i-1];
					k++;
				}
			}

		}
	}
	//��������ʹ��
	if(pMain->m_SPflag==0)
	{
		if (dim_num==1)
		{
			SetprogressPos(5);
			MyImg->FindHighestFrequencyKappa_singl(classifyruleflag,Data,Data_size,Init_Weight,Init_Mu,Init_Sigma,IteratRegion,step);

		}
		
		else if (dim_num>1)
		{
			
			SetprogressPos(5);
			MyImg->FindHighestFrequencyKappa_multi(classifyruleflag,Data_mul,Data_size,Init_Weight,Init_Mu,Init_Sigma,IteratRegion);


			for(i=0;i<dim_num;i++)
			{
				delete [] Data_mul[i];
			}

		}
	}
	//��������ʹ��
	else if (pMain->m_SPflag==1)
	{
		if (dim_num>1)
		{
			//��������ʹ�ù������kappaֵ
			int flag =pMain->D_B_flag;
			//���Դ���ʹ��
			int **Strategy=new int *[3];
			for (int i=0;i<3;i++)
			{
				Strategy[i]=new int[4];
			}
			Strategy=pMain->Strategy;

			MyImg->FindHighestFrequencyKappa_DAGOrBiTree(classifyruleflag,Strategy,flag,Data_mul,Data_size,Init_Weight,Init_Mu,Init_Sigma,IteratRegion,step);

			for(i=0;i<dim_num;i++)
			{
				delete [] Data_mul[i];
			}
		}
		else
		{
			MessageBox("��ά�����޷�����ʹ�ã�������ѡ��������");
			return;
		}
	}

	//���kappaֵ������
	SetprogressPos(100);
	MessageBox("kappa������ɣ�");
	CString str ;
	str.Format("%f",MyImg->kappa);
	SetDlgItemText(IDC_KAPPA,str);


	delete [] Init_Mu;
	delete []Init_Sigma;
	delete [] Init_Weight;
	

}
void  Canshuguji::SetprogressPos(int nPos)
{
	m_ProcessControl.SetPos(nPos);
}

void Canshuguji::OnBnClickedButtonKappa()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItem(IDC_KAPPA)->GetWindowText(str);
	if(str=="")
	{
		AfxMessageBox("û������");
		return;
	}	
	CShow dlg;
	//TEST : ��������
	dlg.maxPixel = 0;
	dlg.xstart = 0;
	dlg.xend = 1;
	dlg.xvalue_interval= 0.01;
	dlg.flag= 1;
	dlg.DoModal();
}
