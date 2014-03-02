#include "StdAfx.h"
#include "Image_kappa_evaluation.h"
#include <iostream>
using namespace std;

//����kappa��ȡֵ��ΧΪ��-1,1������Ĭ����ֵ����СֵΪ-1�����ֵΪ+1��
//ͳ��kappa�ֲ�ֱ��ͼ�ļ����ò�Ҫ����0.1����interval<=0.1
float Image_kappa_evalu::HighestFrequenceInArray(float * Array,const long int Array_size,const float interval)
{
	int NumofIterval=floor(2.0/interval)+1;
	int* CountofEachIterval=new int[NumofIterval];
	memset(CountofEachIterval,0,sizeof(int)*NumofIterval);

	for (int i=0;i<Array_size;i++)
	{
		int sub=(int)floor((Array[i]+1.0)/interval);
		CountofEachIterval[sub]++;
	}

	int max=-1;
	int subofmax=-1;
	for (int i=0;i<NumofIterval;i++)
	{
		if (CountofEachIterval[i]>max)
		{
			max=CountofEachIterval[i];
			subofmax=i;
		}
	}
	float HighestFrequenceValue=-1.0+subofmax*interval;
	return HighestFrequenceValue;
}



//һά�������й̶���ֵ�ڱ�������������GMM���Ƹ�������Լ���ǰ�����¼����kappa
//IteratRegionΪ�������䣬��ʽΪ2*class_num,������ʱ����������Ϊ��-3,3��-4,4,-5��5�����ֱ��ʾ(Init_Mu[0]-3,Init_Mu[0]+3),(Init_Mu[1]-4,Init_Mu[1]+4),(Init_Mu[2]-5,Init_Mu[2]+5)
//stepΪ��ֵ�����ڵı�������
//ClassifyRuleflag=0��ʾ��С���룬ClassifyRuleflag=1��ʾ��С������
//���������ʵ�kappaֵ������Image_kappa_evalu���е�kappa
void Image_kappa_evalu::FindHighestFrequencyKappa_singl(const int ClassifyRuleflag,float* Data,const int Data_size,float * Init_Weight, float *Init_Mu,float* Init_Sigma,float *IteratRegion,const float step)
{   
	const double itol=0.00001;
	const int maxiter=10000;
	const int class_num=class_numb;//��ĸ���ΪInit_parameter��ȷ���������class_numb
	
	int *iterat_num=new int[class_num];
	SizeofAllKappa=1;//(R)
	for (int i=0;i<class_num;i++)
	{
		iterat_num[i]=floor((IteratRegion[i*2+1]-IteratRegion[i*2])/step)+1;
		SizeofAllKappa*=iterat_num[i];//�ܹ���Ҫ�����Ĵ���
	}
	AllKappa=new float[SizeofAllKappa];//(R)
	memset(AllKappa,-2,sizeof(float)*SizeofAllKappa);//KAPPA��ȡֵΪ��-1,1������ʼ��Ϊ����kappa��Χ���ֵ������ֵ�������õ�
	float*Mu_temp=new float[class_num];
	int n;
	if (class_num==2)
	{
		n=0;
		float Mutempofclass1=Init_Mu[0]+IteratRegion[0];
		float Mutempofclass2=Init_Mu[1]+IteratRegion[2];
		for (int i=0;i<iterat_num[0];i++)
		{
			for (int j=0;j<iterat_num[1];j++)
			{
				Mu_temp[0]=Mutempofclass1;
				Mu_temp[1]=Mutempofclass2;
				GMM_singl(Data,Data_size,class_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
				if (ClassifyRuleflag==0)
				{
					kappa_evalu_singl();
				}
				if (ClassifyRuleflag==1)
				{
					kappa_evalu_singl_minimumerro();
				}
				
				AllKappa[i*iterat_num[1]+j]=kappa;
				Mutempofclass2+=step;
				//����������
				n++;
				if(n==int(SizeofAllKappa*0.2))
				{
					pro->SetprogressPos(20);
				}
				if(n==int(SizeofAllKappa*0.5))
				{
					pro->SetprogressPos(50);
				}
				if(n==int(SizeofAllKappa*0.7))
				{
					pro->SetprogressPos(70);
				}
				if(n==int(SizeofAllKappa*0.9))
				{
					pro->SetprogressPos(90);
				}
			}
			Mutempofclass1+=step;
		}	
	}
	else if (class_num==3)
	{
		n=0;
		float Mutempofclass1=Init_Mu[0]+IteratRegion[0];
		float Mutempofclass2=Init_Mu[1]+IteratRegion[2];
		float Mutempofclass3=Init_Mu[2]+IteratRegion[4];
		for (int i=0;i<iterat_num[0];i++)
		{
			for (int j=0;j<iterat_num[1];j++)
			{
				for (int k=0;k<iterat_num[2];k++)
				{
					Mu_temp[0]=Mutempofclass1;
					Mu_temp[1]=Mutempofclass2;
					Mu_temp[2]=Mutempofclass3;
					GMM_singl(Data,Data_size,class_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
					if (ClassifyRuleflag==0)
					{
						kappa_evalu_singl();
					}
					if (ClassifyRuleflag==1)
					{
						kappa_evalu_singl_minimumerro();
					}
					AllKappa[i*iterat_num[1]*iterat_num[2]+j*iterat_num[2]+k]=kappa;
					Mutempofclass3+=step;
					//����������
					n++;
					if(n==int(SizeofAllKappa*0.2))
					{
						pro->SetprogressPos(20);
					}
					if(n==int(SizeofAllKappa*0.5))
					{
						pro->SetprogressPos(50);
					}
					if(n==int(SizeofAllKappa*0.7))
					{
						pro->SetprogressPos(70);
					}
					if(n==int(SizeofAllKappa*0.9))
					{
						pro->SetprogressPos(90);
					}
				}
				Mutempofclass2+=step;
			}
			Mutempofclass1+=step;
		}

	}
	else if (class_num==4)
	{
		n=0;
		float Mutempofclass1=Init_Mu[0]+IteratRegion[0];
		float Mutempofclass2=Init_Mu[1]+IteratRegion[2];
		float Mutempofclass3=Init_Mu[2]+IteratRegion[4];
		float Mutempofclass4=Init_Mu[3]+IteratRegion[6];
		for (int i=0;i<iterat_num[0];i++)
		{
			for (int j=0;j<iterat_num[1];j++)
			{
				for (int k=0;k<iterat_num[2];k++)
				{
					for (int q=0;q<iterat_num[3];q++)
					{
						Mu_temp[0]=Mutempofclass1;
						Mu_temp[1]=Mutempofclass2;
						Mu_temp[2]=Mutempofclass3;
						Mu_temp[3]=Mutempofclass4;
						GMM_singl(Data,Data_size,class_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
						if (ClassifyRuleflag==0)
						{
							kappa_evalu_singl();
						}
						if (ClassifyRuleflag==1)
						{
							kappa_evalu_singl_minimumerro();
						}
						AllKappa[i*iterat_num[1]*iterat_num[2]*iterat_num[3]+j*iterat_num[2]*iterat_num[3]+k*iterat_num[3]+q]=kappa;
						Mutempofclass4+=step;
						//����������
						n++;
						if(n==int(SizeofAllKappa*0.2))
						{
							pro->SetprogressPos(20);
						}
						if(n==int(SizeofAllKappa*0.5))
						{
							pro->SetprogressPos(50);
						}
						if(n==int(SizeofAllKappa*0.7))
						{
							pro->SetprogressPos(70);
						}
						if(n==int(SizeofAllKappa*0.9))
						{
							pro->SetprogressPos(90);
						}
					}
					Mutempofclass3+=step;	
				}
				Mutempofclass2+=step;
			}
			Mutempofclass1+=step;
		}

	}
	else  //����������ʱ������
	{

	}
	kappa=HighestFrequenceInArray(AllKappa,SizeofAllKappa);//Ѱ�����Ƶ�ʵ�kappa
	//delete [] AllKappa;	(R)
	delete [] Mu_temp;
	delete [] iterat_num;
}


//�����ά����ʱ�̶���ֵ���������Ƶ��
//Data[dim_num][Data_size]Ϊ�����������Data_size���ݱ���Ϊʵ�ʳ��ȣ�����������Խ��Σ��
//Init_WeightΪ��ʼ�����Ȩ��  Weight[class_numb]
//Init_MuΪ��ʼ����ľ�ֵ  Mu[class_numb*dim_numb] Mu[11 21 31 12 22 32]
//Init_SigmaΪ��ʼ����ı�׼�� Sigma[class_numb*dim_numb*dim_numb] Sigma[111 112 121 122   211 212 221 222 311 312 321 322 ]
//IteratRegionΪ�������䣬��СΪ2*dim_numb*class_num,
//��������ά����ʱ����������Ϊ��-1,1��-2,2,-3,3 -4,4, -5,5, -6,6��,�ֱ��ʾ
//[(mu11-1,mu11+1) (mu12-2,mu12+2) (mu13-3,mu13+3)
// (mu21-4,mu21+4) (mu22-5,mu22+5) (mu23-6,mu23+6)];
//stepΪ��ֵ�����Ĳ�����kappaevalu_stepΪ����kappaʱ������͵Ĳ���
//���������ʵ�kappaֵ������Image_kappa_evalu���е�kappa
void Image_kappa_evalu::FindHighestFrequencyKappa_multi(const int ClassifyRuleflag,float** Data,const int Data_size,float * Init_Weight, float *Init_Mu,float* Init_Sigma,float *IteratRegion,const float step,const float kappaevalu_step)
{
	const double itol=0.00001;
	const int maxiter=10000;
	const int class_num=class_numb;//��ĸ���ΪInit_parameter��ȷ���������class_numb
	const int dim_num=dim_numb;//ά��ΪInit_parameter��ȷ����ά�Ȳ���dim_numb;

	int *iterat_num=new int[dim_num*class_num];
	SizeofAllKappa=1;//(R)
	for (int i=0;i<dim_num*class_num;i++)
	{
		iterat_num[i]=floor((IteratRegion[i*2+1]-IteratRegion[i*2])/step)+1;
		//SizeofAllKappa*=iterat_num[i];//�ܹ���Ҫ�����Ĵ���
	}
	for (int i=0;i<dim_num*class_num;i++ )
	{
		SizeofAllKappa*=iterat_num[i];
	}

	AllKappa=new float[SizeofAllKappa];//(R)
	memset(AllKappa,-2,sizeof(float)*SizeofAllKappa);//KAPPA��ȡֵΪ��-1,1������ʼ��Ϊ����kappa��Χ���ֵ������ֵ�������õ�
	float*Mu_temp=new float[dim_num*class_num];
	if (dim_num==2 && class_num==2)
	{
		float Mutempofclass11=Init_Mu[0]+IteratRegion[0];//��һ���һά����
		float Mutempofclass21=Init_Mu[1]+IteratRegion[2];//�ڶ����һά����
		float Mutempofclass12=Init_Mu[2]+IteratRegion[4];//��һ��ڶ�ά����
		float Mutempofclass22=Init_Mu[3]+IteratRegion[6];//�ڶ���ڶ�ά����
		int sub=0;
		for (int i=0;i<iterat_num[0];i++)
		{
			for (int j=0;j<iterat_num[1];j++)
			{
				for (int k=0;k<iterat_num[2];k++)
				{
					for (int q=0;q<iterat_num[3];q++)
					{
						Mu_temp[0]=Mutempofclass11;
						Mu_temp[1]=Mutempofclass21;
						Mu_temp[2]=Mutempofclass12;
						Mu_temp[3]=Mutempofclass22;
						GMM_multi(Data,Data_size,class_num,dim_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
						kappa_evalu_multi(ClassifyRuleflag,kappaevalu_step);
						AllKappa[sub]=kappa;
						sub++;
						Mutempofclass22+=step;
						if(sub==int(SizeofAllKappa*0.1))
						{
							pro->SetprogressPos(10);
						}
						if(sub==int(SizeofAllKappa*0.2))
						{
							pro->SetprogressPos(20);
						}
						if(sub==int(SizeofAllKappa*0.3))
						{
							pro->SetprogressPos(30);
						}
						if(sub==int(SizeofAllKappa*0.4))
						{
							pro->SetprogressPos(40);
						}
						if(sub==int(SizeofAllKappa*0.5))
						{
							pro->SetprogressPos(50);
						}
						if(sub==int(SizeofAllKappa*0.6))
						{
							pro->SetprogressPos(60);
						}
						if(sub==int(SizeofAllKappa*0.7))
						{
							pro->SetprogressPos(70);
						}
						if(sub==int(SizeofAllKappa*0.8))
						{
							pro->SetprogressPos(80);
						}
						if(sub==int(SizeofAllKappa*0.90))
						{
							pro->SetprogressPos(90);
						}
					}
					Mutempofclass12+=step;	
				}
				Mutempofclass21+=step;
			}
			Mutempofclass11+=step;
		}
	}
	else if (dim_num==2 && class_num==3)
	{
		float Mutempofclass11=Init_Mu[0]+IteratRegion[0];//��һ���һά����
		float Mutempofclass21=Init_Mu[1]+IteratRegion[2];//�ڶ����һά����
		float Mutempofclass31=Init_Mu[2]+IteratRegion[4];//�������һά����
		float Mutempofclass12=Init_Mu[3]+IteratRegion[6];//��һ��ڶ�ά����
		float Mutempofclass22=Init_Mu[4]+IteratRegion[8];//�ڶ���ڶ�ά����
		float Mutempofclass32=Init_Mu[5]+IteratRegion[10];//������ڶ�ά����
		int sub=0;
		for (int m11=0;m11<iterat_num[0];m11++)
		{
			for (int m21=0;m21<iterat_num[1];m21++)
			{
				for (int m31=0;m31<iterat_num[2];m31++)
				{
					for (int m12=0;m12<iterat_num[3];m12++)
					{
						for (int m22=0;m22<iterat_num[4];m22++)
						{
							for (int m32=0;m32<iterat_num[5];m32++)
							{
								Mu_temp[0]=Mutempofclass11;
								Mu_temp[1]=Mutempofclass21;
								Mu_temp[2]=Mutempofclass31;
								Mu_temp[3]=Mutempofclass12;
								Mu_temp[4]=Mutempofclass22;
								Mu_temp[5]=Mutempofclass32;
								GMM_multi(Data,Data_size,class_num,dim_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
								kappa_evalu_multi(ClassifyRuleflag,kappaevalu_step);
								AllKappa[sub]=kappa;
								sub++;
								Mutempofclass32+=step;
								if(sub==int(SizeofAllKappa*0.1))
								{
									pro->SetprogressPos(10);
								}
								if(sub==int(SizeofAllKappa*0.2))
								{
									pro->SetprogressPos(20);
								}
								if(sub==int(SizeofAllKappa*0.3))
								{
									pro->SetprogressPos(30);
								}
								if(sub==int(SizeofAllKappa*0.4))
								{
									pro->SetprogressPos(40);
								}
								if(sub==int(SizeofAllKappa*0.5))
								{
									pro->SetprogressPos(50);
								}
								if(sub==int(SizeofAllKappa*0.6))
								{
									pro->SetprogressPos(60);
								}
								if(sub==int(SizeofAllKappa*0.7))
								{
									pro->SetprogressPos(70);
								}
								if(sub==int(SizeofAllKappa*0.8))
								{
									pro->SetprogressPos(80);
								}
								if(sub==int(SizeofAllKappa*0.90))
								{
									pro->SetprogressPos(90);
								}
							}
							Mutempofclass22+=step;
						}
						Mutempofclass12+=step;
					}
					Mutempofclass31+=step;
				}
				Mutempofclass21+=step;
			}
			Mutempofclass11+=step;
		}
    }

	else if (dim_num==2 && class_num==4)
	{
		float Mutempofclass11=Init_Mu[0]+IteratRegion[0];//��һ���һά����
		float Mutempofclass21=Init_Mu[1]+IteratRegion[2];//�ڶ����һά����
		float Mutempofclass31=Init_Mu[2]+IteratRegion[4];//�������һά����
		float Mutempofclass41=Init_Mu[3]+IteratRegion[6];//�������һά����
		float Mutempofclass12=Init_Mu[4]+IteratRegion[8];//��һ��ڶ�ά����
		float Mutempofclass22=Init_Mu[5]+IteratRegion[10];//�ڶ���ڶ�ά����
		float Mutempofclass32=Init_Mu[6]+IteratRegion[12];//������ڶ�ά����
		float Mutempofclass42=Init_Mu[7]+IteratRegion[14];//������ڶ�ά����
		int sub=0;
		for (int m11=0;m11<iterat_num[0];m11++)
		{
			for (int m21=0;m21<iterat_num[1];m21++)
			{
				for (int m31=0;m31<iterat_num[2];m31++)
				{
					for (int m41=0;m41<iterat_num[3];m41++)
					{
						for (int m12=0;m12<iterat_num[4];m12++)
						{
							for (int m22=0;m22<iterat_num[5];m22++)
							{
								for (int m32=0;m32<iterat_num[6];m32++)
								{
									for (int m42=0;m42<iterat_num[7];m42++)
									{
										Mu_temp[0]=Mutempofclass11;
										Mu_temp[1]=Mutempofclass21;
										Mu_temp[2]=Mutempofclass31;
										Mu_temp[3]=Mutempofclass41;
										Mu_temp[4]=Mutempofclass12;
										Mu_temp[5]=Mutempofclass22;
										Mu_temp[6]=Mutempofclass32;
										Mu_temp[7]=Mutempofclass42;
										GMM_multi(Data,Data_size,class_num,dim_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
										kappa_evalu_multi(ClassifyRuleflag,kappaevalu_step);
										AllKappa[sub]=kappa;
										sub++;
										Mutempofclass42+=step;
										if(sub==int(SizeofAllKappa*0.1))
										{
											pro->SetprogressPos(10);
										}
										if(sub==int(SizeofAllKappa*0.2))
										{
											pro->SetprogressPos(20);
										}
										if(sub==int(SizeofAllKappa*0.3))
										{
											pro->SetprogressPos(30);
										}
										if(sub==int(SizeofAllKappa*0.4))
										{
											pro->SetprogressPos(40);
										}
										if(sub==int(SizeofAllKappa*0.5))
										{
											pro->SetprogressPos(50);
										}
										if(sub==int(SizeofAllKappa*0.6))
										{
											pro->SetprogressPos(60);
										}
										if(sub==int(SizeofAllKappa*0.7))
										{
											pro->SetprogressPos(70);
										}
										if(sub==int(SizeofAllKappa*0.8))
										{
											pro->SetprogressPos(80);
										}
										if(sub==int(SizeofAllKappa*0.90))
										{
											pro->SetprogressPos(90);
										}
									}
									Mutempofclass32+=step;
								}
								Mutempofclass22+=step;
							}
							Mutempofclass12+=step;
						}
						Mutempofclass41+=step;
					}
					Mutempofclass31+=step;
				}
				Mutempofclass21+=step;
			}
			Mutempofclass11+=step;
		}
	}

	else if (dim_num==3 && class_num==2)
	{
		float Mutempofclass11=Init_Mu[0]+IteratRegion[0];//��һ���һά����
		float Mutempofclass21=Init_Mu[1]+IteratRegion[2];//�ڶ����һά����
		float Mutempofclass12=Init_Mu[2]+IteratRegion[4];//��һ��ڶ�ά����
		float Mutempofclass22=Init_Mu[3]+IteratRegion[6];//�ڶ���ڶ�ά����
		float Mutempofclass13=Init_Mu[4]+IteratRegion[8];//�������һά����
		float Mutempofclass23=Init_Mu[5]+IteratRegion[10];//������ڶ�ά����
		int sub=0;
		for (int m11=0;m11<iterat_num[0];m11++)
		{
			for (int m21=0;m21<iterat_num[1];m21++)
			{
				for (int m12=0;m12<iterat_num[2];m12++)
				{
					for (int m22=0;m22<iterat_num[3];m22++)
					{
						for (int m13=0;m13<iterat_num[4];m13++)
						{
							for (int m23=0;m23<iterat_num[5];m23++)
							{
								Mu_temp[0]=Mutempofclass11;
								Mu_temp[1]=Mutempofclass21;
								Mu_temp[2]=Mutempofclass12;
								Mu_temp[3]=Mutempofclass22;
								Mu_temp[4]=Mutempofclass13;
								Mu_temp[5]=Mutempofclass23;
								GMM_multi(Data,Data_size,class_num,dim_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
								kappa_evalu_multi(ClassifyRuleflag,kappaevalu_step);
								AllKappa[sub]=kappa;
								sub++;
								Mutempofclass23+=step;
								if(sub==int(SizeofAllKappa*0.1))
								{
									pro->SetprogressPos(10);
								}
								if(sub==int(SizeofAllKappa*0.2))
								{
									pro->SetprogressPos(20);
								}
								if(sub==int(SizeofAllKappa*0.3))
								{
									pro->SetprogressPos(30);
								}
								if(sub==int(SizeofAllKappa*0.4))
								{
									pro->SetprogressPos(40);
								}
								if(sub==int(SizeofAllKappa*0.5))
								{
									pro->SetprogressPos(50);
								}
								if(sub==int(SizeofAllKappa*0.6))
								{
									pro->SetprogressPos(60);
								}
								if(sub==int(SizeofAllKappa*0.7))
								{
									pro->SetprogressPos(70);
								}
								if(sub==int(SizeofAllKappa*0.8))
								{
									pro->SetprogressPos(80);
								}
								if(sub==int(SizeofAllKappa*0.90))
								{
									pro->SetprogressPos(90);
								}
							}
							Mutempofclass13+=step;
						}
						Mutempofclass22+=step;
					}
					Mutempofclass12+=step;
				}
				Mutempofclass21+=step;
			}
			Mutempofclass11+=step;
		}
	}

	else if (dim_num==3 && class_num==3)
	{
		float Mutempofclass11=Init_Mu[0]+IteratRegion[0];//��һ���һά����
		float Mutempofclass21=Init_Mu[1]+IteratRegion[2];//�ڶ����һά����
		float Mutempofclass31=Init_Mu[2]+IteratRegion[4];//�������һά����
		float Mutempofclass12=Init_Mu[3]+IteratRegion[6];//��һ��ڶ�ά����
		float Mutempofclass22=Init_Mu[4]+IteratRegion[8];//�ڶ���ڶ�ά����
		float Mutempofclass32=Init_Mu[5]+IteratRegion[10];//������ڶ�ά����
		float Mutempofclass13=Init_Mu[6]+IteratRegion[12];//��һ�����ά����
		float Mutempofclass23=Init_Mu[7]+IteratRegion[14];//�ڶ������ά����
		float Mutempofclass33=Init_Mu[8]+IteratRegion[16];//���������ά����
		int sub=0;
		for (int m11=0;m11<iterat_num[0];m11++)
		{
			for (int m21=0;m21<iterat_num[1];m21++)
			{
				for (int m31=0;m31<iterat_num[2];m31++)
				{
					for (int m12=0;m12<iterat_num[3];m12++)
					{
						for (int m22=0;m22<iterat_num[4];m22++)
						{
							for (int m32=0;m32<iterat_num[5];m32++)
							{
								for (int m13=0;m13<iterat_num[6];m13++)
								{
									for (int m23=0;m23<iterat_num[7];m23++)
									{
										for (int m33=0;m33<iterat_num[8];m33++)
										{
											Mu_temp[0]=Mutempofclass11;
											Mu_temp[1]=Mutempofclass21;
											Mu_temp[2]=Mutempofclass31;
											Mu_temp[3]=Mutempofclass12;
											Mu_temp[4]=Mutempofclass22;
											Mu_temp[5]=Mutempofclass32;
											Mu_temp[6]=Mutempofclass13;
											Mu_temp[7]=Mutempofclass23;
											Mu_temp[8]=Mutempofclass33;
											GMM_multi(Data,Data_size,class_num,dim_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
											kappa_evalu_multi(ClassifyRuleflag,kappaevalu_step);
											AllKappa[sub]=kappa;
											sub++;
											Mutempofclass33+=step;
											if(sub==int(SizeofAllKappa*0.1))
											{
												pro->SetprogressPos(10);
											}
											if(sub==int(SizeofAllKappa*0.2))
											{
												pro->SetprogressPos(20);
											}
											if(sub==int(SizeofAllKappa*0.3))
											{
												pro->SetprogressPos(30);
											}
											if(sub==int(SizeofAllKappa*0.4))
											{
												pro->SetprogressPos(40);
											}
											if(sub==int(SizeofAllKappa*0.5))
											{
												pro->SetprogressPos(50);
											}
											if(sub==int(SizeofAllKappa*0.6))
											{
												pro->SetprogressPos(60);
											}
											if(sub==int(SizeofAllKappa*0.7))
											{
												pro->SetprogressPos(70);
											}
											if(sub==int(SizeofAllKappa*0.8))
											{
												pro->SetprogressPos(80);
											}
											if(sub==int(SizeofAllKappa*0.90))
											{
												pro->SetprogressPos(90);
											}
										}
										Mutempofclass23+=step;
									}
									Mutempofclass13+=step;
								}
								Mutempofclass32+=step;
							}
							Mutempofclass22+=step;
						}
						Mutempofclass12+=step;
					}
					Mutempofclass31+=step;
				}
				Mutempofclass21+=step;
			}
			Mutempofclass11+=step;
		}
	}

	else if (dim_num==3 && class_num==4)
	{
		float Mutempofclass11=Init_Mu[0]+IteratRegion[0];//��һ���һά����
		float Mutempofclass21=Init_Mu[1]+IteratRegion[2];//�ڶ����һά����
		float Mutempofclass31=Init_Mu[2]+IteratRegion[4];//�������һά����
		float Mutempofclass41=Init_Mu[3]+IteratRegion[6];//�������һά����
		float Mutempofclass12=Init_Mu[4]+IteratRegion[8];//��һ��ڶ�ά����
		float Mutempofclass22=Init_Mu[5]+IteratRegion[10];//�ڶ���ڶ�ά����
		float Mutempofclass32=Init_Mu[6]+IteratRegion[12];//������ڶ�ά����
		float Mutempofclass42=Init_Mu[7]+IteratRegion[14];//������ڶ�ά����
		float Mutempofclass13=Init_Mu[8]+IteratRegion[16];//��һ�����ά����
		float Mutempofclass23=Init_Mu[9]+IteratRegion[18];//�ڶ������ά����
		float Mutempofclass33=Init_Mu[10]+IteratRegion[20];//���������ά����
		float Mutempofclass43=Init_Mu[11]+IteratRegion[22];//���������ά����
		int sub=0;
		for (int m11=0;m11<iterat_num[0];m11++)
		{
			for (int m21=0;m21<iterat_num[1];m21++)
			{
				for (int m31=0;m31<iterat_num[2];m31++)
				{
					for (int m41=0;m41<iterat_num[3];m41++)
					{
					for (int m12=0;m12<iterat_num[4];m12++)
					{
						for (int m22=0;m22<iterat_num[5];m22++)
						{
							for (int m32=0;m32<iterat_num[6];m32++)
							{
								for (int m42=0;m42<iterat_num[7];m42++)
								{
								for (int m13=0;m13<iterat_num[8];m13++)
								{
									for (int m23=0;m23<iterat_num[9];m23++)
									{
										for (int m33=0;m33<iterat_num[10];m33++)
										{
											for (int m43=0;m43<iterat_num[11];m43++)
											{
												Mu_temp[0]=Mutempofclass11;
												Mu_temp[1]=Mutempofclass21;
												Mu_temp[2]=Mutempofclass31;
												Mu_temp[3]=Mutempofclass41;
												Mu_temp[4]=Mutempofclass12;
												Mu_temp[5]=Mutempofclass22;
												Mu_temp[6]=Mutempofclass32;
												Mu_temp[7]=Mutempofclass42;
												Mu_temp[8]=Mutempofclass13;
												Mu_temp[9]=Mutempofclass23;
												Mu_temp[10]=Mutempofclass33;
												Mu_temp[11]=Mutempofclass43;
												GMM_multi(Data,Data_size,class_num,dim_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
												kappa_evalu_multi(ClassifyRuleflag,kappaevalu_step);
												AllKappa[sub]=kappa;
												sub++;
												Mutempofclass43+=step;
												if(sub==int(SizeofAllKappa*0.1))
												{
													pro->SetprogressPos(10);
												}
												if(sub==int(SizeofAllKappa*0.2))
												{
													pro->SetprogressPos(20);
												}
												if(sub==int(SizeofAllKappa*0.3))
												{
													pro->SetprogressPos(30);
												}
												if(sub==int(SizeofAllKappa*0.4))
												{
													pro->SetprogressPos(40);
												}
												if(sub==int(SizeofAllKappa*0.5))
												{
													pro->SetprogressPos(50);
												}
												if(sub==int(SizeofAllKappa*0.6))
												{
													pro->SetprogressPos(60);
												}
												if(sub==int(SizeofAllKappa*0.7))
												{
													pro->SetprogressPos(70);
												}
												if(sub==int(SizeofAllKappa*0.8))
												{
													pro->SetprogressPos(80);
												}
												if(sub==int(SizeofAllKappa*0.90))
												{
													pro->SetprogressPos(90);
												}
											}
											Mutempofclass33+=step;
										}
										Mutempofclass23+=step;
									}
									Mutempofclass13+=step;
								}
								Mutempofclass42+=step;
							    }
							    Mutempofclass32+=step;
						     }
						     Mutempofclass22+=step;
					     }
					     Mutempofclass12+=step;
				      }
				      Mutempofclass41+=step;
			       }
			       Mutempofclass31+=step;
		         }
		        Mutempofclass21+=step;
			}
			Mutempofclass11+=step;
		}
	}
	else//��ά�����ݲ�����
	{
	}
	kappa=HighestFrequenceInArray(AllKappa,SizeofAllKappa);//Ѱ�����Ƶ�ʵ�kappa
	//delete [] AllKappa;(R)
	delete [] Mu_temp;
	delete [] iterat_num;
}




//DAG��BinaryTree��ʽ��strategyΪ�������flagΪ0ΪDGA��flagΪ1Ϊ������, Classifyruleflag=0ΪDAG��Classifyruleflag=1Ϊ������
//�����ά����ʱ�̶���ֵ���������Ƶ��
//Data[dim_num][Data_size]Ϊ�����������Data_size���ݱ���Ϊʵ�ʳ��ȣ�����������Խ��Σ��
// Init_WeightΪ��ʼ�����Ȩ��  Weight[class_numb]
//Init_MuΪ��ʼ����ľ�ֵ  Mu[class_numb*dim_numb] Mu[11 21 31 12 22 32]
//Init_SigmaΪ��ʼ����ı�׼�� Sigma[class_numb*dim_numb*dim_numb] Sigma[111 112 121 122   211 212 221 222 311 312 321 322 ]
//IteratRegionΪ�������䣬��СΪ2*dim_numb*class_num,
//��������ά����ʱ����������Ϊ��-1,1��-2,2,-3,3 -4,4, -5,5, -6,6��,�ֱ��ʾ
//[(mu11-1,mu11+1) (mu12-2,mu12+2) (mu13-3,mu13+3)
// (mu21-4,mu21+4) (mu22-5,mu22+5) (mu23-6,mu23+6)];
//stepΪ��ֵ�����Ĳ�����kappaevalu_stepΪ����kappaʱ������͵Ĳ���
//���������ʵ�kappaֵ������Image_kappa_evalu���е�kappa
void Image_kappa_evalu::FindHighestFrequencyKappa_DAGOrBiTree(const int Classifyruleflag,int** Strategy,const int flag,float** Data,const int Data_size,float * Init_Weight, float *Init_Mu,float* Init_Sigma,float *IteratRegion,const float step,const float kappaevalu_step)
{
	const double itol=0.00001;
	const int maxiter=10000;
	const int class_num=class_numb;//��ĸ���ΪInit_parameter��ȷ���������class_numb
	const int dim_num=dim_numb;//ά��ΪInit_parameter��ȷ����ά�Ȳ���dim_numb;

	int *iterat_num=new int[dim_num*class_num];
	long int SizeofAllKappa=1;
	for (int i=0;i<dim_num*class_num;i++)
	{
		iterat_num[i]=floor((IteratRegion[i*2+1]-IteratRegion[i*2])/step)+1;
		SizeofAllKappa*=iterat_num[i];//�ܹ���Ҫ�����Ĵ���
	}
	float* AllKappa=new float[SizeofAllKappa];
	memset(AllKappa,-2,sizeof(float)*SizeofAllKappa);//KAPPA��ȡֵΪ��-1,1������ʼ��Ϊ����kappa��Χ���ֵ������ֵ�������õ�
	float*Mu_temp=new float[dim_num*class_num];
	if (dim_num==2 && class_num==2)
	{
		float Mutempofclass11=Init_Mu[0]+IteratRegion[0];//��һ���һά����
		float Mutempofclass21=Init_Mu[1]+IteratRegion[2];//�ڶ����һά����
		float Mutempofclass12=Init_Mu[2]+IteratRegion[4];//��һ��ڶ�ά����
		float Mutempofclass22=Init_Mu[3]+IteratRegion[6];//�ڶ���ڶ�ά����
		int sub=0;
		for (int i=0;i<iterat_num[0];i++)
		{
			for (int j=0;j<iterat_num[1];j++)
			{
				for (int k=0;k<iterat_num[2];k++)
				{
					for (int q=0;q<iterat_num[3];q++)
					{
						Mu_temp[0]=Mutempofclass11;
						Mu_temp[1]=Mutempofclass21;
						Mu_temp[2]=Mutempofclass12;
						Mu_temp[3]=Mutempofclass22;
						GMM_multi(Data,Data_size,class_num,dim_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
						kappa_evalu_multi_DAGOrBiTree(Classifyruleflag,Strategy,flag,kappaevalu_step);
						AllKappa[sub]=kappa;
						sub++;
						Mutempofclass22+=step;
						if(sub==int(SizeofAllKappa*0.1))
						{
							pro->SetprogressPos(10);
						}
						if(sub==int(SizeofAllKappa*0.2))
						{
							pro->SetprogressPos(20);
						}
						if(sub==int(SizeofAllKappa*0.3))
						{
							pro->SetprogressPos(30);
						}
						if(sub==int(SizeofAllKappa*0.4))
						{
							pro->SetprogressPos(40);
						}
						if(sub==int(SizeofAllKappa*0.5))
						{
							pro->SetprogressPos(50);
						}
						if(sub==int(SizeofAllKappa*0.6))
						{
							pro->SetprogressPos(60);
						}
						if(sub==int(SizeofAllKappa*0.7))
						{
							pro->SetprogressPos(70);
						}
						if(sub==int(SizeofAllKappa*0.8))
						{
							pro->SetprogressPos(80);
						}
						if(sub==int(SizeofAllKappa*0.90))
						{
							pro->SetprogressPos(90);
						}
					}
					Mutempofclass12+=step;	
				}
				Mutempofclass21+=step;
			}
			Mutempofclass11+=step;
		}
	}
	else if (dim_num==2 && class_num==3)
	{
		float Mutempofclass11=Init_Mu[0]+IteratRegion[0];//��һ���һά����
		float Mutempofclass21=Init_Mu[1]+IteratRegion[2];//�ڶ����һά����
		float Mutempofclass31=Init_Mu[2]+IteratRegion[4];//�������һά����
		float Mutempofclass12=Init_Mu[3]+IteratRegion[6];//��һ��ڶ�ά����
		float Mutempofclass22=Init_Mu[4]+IteratRegion[8];//�ڶ���ڶ�ά����
		float Mutempofclass32=Init_Mu[5]+IteratRegion[10];//������ڶ�ά����
		int sub=0;
		for (int m11=0;m11<iterat_num[0];m11++)
		{
			for (int m21=0;m21<iterat_num[1];m21++)
			{
				for (int m31=0;m31<iterat_num[2];m31++)
				{
					for (int m12=0;m12<iterat_num[3];m12++)
					{
						for (int m22=0;m22<iterat_num[4];m22++)
						{
							for (int m32=0;m32<iterat_num[5];m32++)
							{
								Mu_temp[0]=Mutempofclass11;
								Mu_temp[1]=Mutempofclass21;
								Mu_temp[2]=Mutempofclass31;
								Mu_temp[3]=Mutempofclass12;
								Mu_temp[4]=Mutempofclass22;
								Mu_temp[5]=Mutempofclass32;
								GMM_multi(Data,Data_size,class_num,dim_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
								kappa_evalu_multi_DAGOrBiTree(Classifyruleflag,Strategy,flag,kappaevalu_step);
								AllKappa[sub]=kappa;
								sub++;
								Mutempofclass32+=step;
								if(sub==int(SizeofAllKappa*0.1))
								{
									pro->SetprogressPos(10);
								}
								if(sub==int(SizeofAllKappa*0.2))
								{
									pro->SetprogressPos(20);
								}
								if(sub==int(SizeofAllKappa*0.3))
								{
									pro->SetprogressPos(30);
								}
								if(sub==int(SizeofAllKappa*0.4))
								{
									pro->SetprogressPos(40);
								}
								if(sub==int(SizeofAllKappa*0.5))
								{
									pro->SetprogressPos(50);
								}
								if(sub==int(SizeofAllKappa*0.6))
								{
									pro->SetprogressPos(60);
								}
								if(sub==int(SizeofAllKappa*0.7))
								{
									pro->SetprogressPos(70);
								}
								if(sub==int(SizeofAllKappa*0.8))
								{
									pro->SetprogressPos(80);
								}
								if(sub==int(SizeofAllKappa*0.90))
								{
									pro->SetprogressPos(90);
								}
							}
							Mutempofclass22+=step;
						}
						Mutempofclass12+=step;
					}
					Mutempofclass31+=step;
				}
				Mutempofclass21+=step;
			}
			Mutempofclass11+=step;
		}
	}

	else if (dim_num==2 && class_num==4)
	{
		float Mutempofclass11=Init_Mu[0]+IteratRegion[0];//��һ���һά����
		float Mutempofclass21=Init_Mu[1]+IteratRegion[2];//�ڶ����һά����
		float Mutempofclass31=Init_Mu[2]+IteratRegion[4];//�������һά����
		float Mutempofclass41=Init_Mu[3]+IteratRegion[6];//�������һά����
		float Mutempofclass12=Init_Mu[4]+IteratRegion[8];//��һ��ڶ�ά����
		float Mutempofclass22=Init_Mu[5]+IteratRegion[10];//�ڶ���ڶ�ά����
		float Mutempofclass32=Init_Mu[6]+IteratRegion[12];//������ڶ�ά����
		float Mutempofclass42=Init_Mu[7]+IteratRegion[14];//������ڶ�ά����
		int sub=0;
		for (int m11=0;m11<iterat_num[0];m11++)
		{
			for (int m21=0;m21<iterat_num[1];m21++)
			{
				for (int m31=0;m31<iterat_num[2];m31++)
				{
					for (int m41=0;m41<iterat_num[3];m41++)
					{
						for (int m12=0;m12<iterat_num[4];m12++)
						{
							for (int m22=0;m22<iterat_num[5];m22++)
							{
								for (int m32=0;m32<iterat_num[6];m32++)
								{
									for (int m42=0;m42<iterat_num[7];m42++)
									{
										Mu_temp[0]=Mutempofclass11;
										Mu_temp[1]=Mutempofclass21;
										Mu_temp[2]=Mutempofclass31;
										Mu_temp[3]=Mutempofclass41;
										Mu_temp[4]=Mutempofclass12;
										Mu_temp[5]=Mutempofclass22;
										Mu_temp[6]=Mutempofclass32;
										Mu_temp[7]=Mutempofclass42;
										GMM_multi(Data,Data_size,class_num,dim_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
										kappa_evalu_multi_DAGOrBiTree(Classifyruleflag,Strategy,flag,kappaevalu_step);
										AllKappa[sub]=kappa;
										sub++;
										Mutempofclass42+=step;
										if(sub==int(SizeofAllKappa*0.1))
										{
											pro->SetprogressPos(10);
										}
										if(sub==int(SizeofAllKappa*0.2))
										{
											pro->SetprogressPos(20);
										}
										if(sub==int(SizeofAllKappa*0.3))
										{
											pro->SetprogressPos(30);
										}
										if(sub==int(SizeofAllKappa*0.4))
										{
											pro->SetprogressPos(40);
										}
										if(sub==int(SizeofAllKappa*0.5))
										{
											pro->SetprogressPos(50);
										}
										if(sub==int(SizeofAllKappa*0.6))
										{
											pro->SetprogressPos(60);
										}
										if(sub==int(SizeofAllKappa*0.7))
										{
											pro->SetprogressPos(70);
										}
										if(sub==int(SizeofAllKappa*0.8))
										{
											pro->SetprogressPos(80);
										}
										if(sub==int(SizeofAllKappa*0.90))
										{
											pro->SetprogressPos(90);
										}
									}
									Mutempofclass32+=step;
								}
								Mutempofclass22+=step;
							}
							Mutempofclass12+=step;
						}
						Mutempofclass41+=step;
					}
					Mutempofclass31+=step;
				}
				Mutempofclass21+=step;
			}
			Mutempofclass11+=step;
		}
	}

	else if (dim_num==3 && class_num==2)
	{
		float Mutempofclass11=Init_Mu[0]+IteratRegion[0];//��һ���һά����
		float Mutempofclass21=Init_Mu[1]+IteratRegion[2];//�ڶ����һά����
		float Mutempofclass12=Init_Mu[2]+IteratRegion[4];//��һ��ڶ�ά����
		float Mutempofclass22=Init_Mu[3]+IteratRegion[6];//�ڶ���ڶ�ά����
		float Mutempofclass13=Init_Mu[4]+IteratRegion[8];//�������һά����
		float Mutempofclass23=Init_Mu[5]+IteratRegion[10];//������ڶ�ά����
		int sub=0;
		for (int m11=0;m11<iterat_num[0];m11++)
		{
			for (int m21=0;m21<iterat_num[1];m21++)
			{
				for (int m12=0;m12<iterat_num[2];m12++)
				{
					for (int m22=0;m22<iterat_num[3];m22++)
					{
						for (int m13=0;m13<iterat_num[4];m13++)
						{
							for (int m23=0;m23<iterat_num[5];m23++)
							{
								Mu_temp[0]=Mutempofclass11;
								Mu_temp[1]=Mutempofclass21;
								Mu_temp[2]=Mutempofclass12;
								Mu_temp[3]=Mutempofclass22;
								Mu_temp[4]=Mutempofclass13;
								Mu_temp[5]=Mutempofclass23;
								GMM_multi(Data,Data_size,class_num,dim_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
								kappa_evalu_multi_DAGOrBiTree(Classifyruleflag,Strategy,flag,kappaevalu_step);
								AllKappa[sub]=kappa;
								sub++;
								Mutempofclass23+=step;
								if(sub==int(SizeofAllKappa*0.1))
								{
									pro->SetprogressPos(10);
								}
								if(sub==int(SizeofAllKappa*0.2))
								{
									pro->SetprogressPos(20);
								}
								if(sub==int(SizeofAllKappa*0.3))
								{
									pro->SetprogressPos(30);
								}
								if(sub==int(SizeofAllKappa*0.4))
								{
									pro->SetprogressPos(40);
								}
								if(sub==int(SizeofAllKappa*0.5))
								{
									pro->SetprogressPos(50);
								}
								if(sub==int(SizeofAllKappa*0.6))
								{
									pro->SetprogressPos(60);
								}
								if(sub==int(SizeofAllKappa*0.7))
								{
									pro->SetprogressPos(70);
								}
								if(sub==int(SizeofAllKappa*0.8))
								{
									pro->SetprogressPos(80);
								}
								if(sub==int(SizeofAllKappa*0.90))
								{
									pro->SetprogressPos(90);
								}
							}
							Mutempofclass13+=step;
						}
						Mutempofclass22+=step;
					}
					Mutempofclass12+=step;
				}
				Mutempofclass21+=step;
			}
			Mutempofclass11+=step;
		}
	}

	else if (dim_num==3 && class_num==3)
	{
		float Mutempofclass11=Init_Mu[0]+IteratRegion[0];//��һ���һά����
		float Mutempofclass21=Init_Mu[1]+IteratRegion[2];//�ڶ����һά����
		float Mutempofclass31=Init_Mu[2]+IteratRegion[4];//�������һά����
		float Mutempofclass12=Init_Mu[3]+IteratRegion[6];//��һ��ڶ�ά����
		float Mutempofclass22=Init_Mu[4]+IteratRegion[8];//�ڶ���ڶ�ά����
		float Mutempofclass32=Init_Mu[5]+IteratRegion[10];//������ڶ�ά����
		float Mutempofclass13=Init_Mu[6]+IteratRegion[12];//��һ�����ά����
		float Mutempofclass23=Init_Mu[7]+IteratRegion[14];//�ڶ������ά����
		float Mutempofclass33=Init_Mu[8]+IteratRegion[16];//���������ά����
		int sub=0;
		for (int m11=0;m11<iterat_num[0];m11++)
		{
			for (int m21=0;m21<iterat_num[1];m21++)
			{
				for (int m31=0;m31<iterat_num[2];m31++)
				{
					for (int m12=0;m12<iterat_num[3];m12++)
					{
						for (int m22=0;m22<iterat_num[4];m22++)
						{
							for (int m32=0;m32<iterat_num[5];m32++)
							{
								for (int m13=0;m13<iterat_num[6];m13++)
								{
									for (int m23=0;m23<iterat_num[7];m23++)
									{
										for (int m33=0;m33<iterat_num[8];m33++)
										{
											Mu_temp[0]=Mutempofclass11;
											Mu_temp[1]=Mutempofclass21;
											Mu_temp[2]=Mutempofclass31;
											Mu_temp[3]=Mutempofclass12;
											Mu_temp[4]=Mutempofclass22;
											Mu_temp[5]=Mutempofclass32;
											Mu_temp[6]=Mutempofclass13;
											Mu_temp[7]=Mutempofclass23;
											Mu_temp[8]=Mutempofclass33;
											GMM_multi(Data,Data_size,class_num,dim_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
											kappa_evalu_multi_DAGOrBiTree(Classifyruleflag,Strategy,flag,kappaevalu_step);
											AllKappa[sub]=kappa;
											sub++;
											Mutempofclass33+=step;
											if(sub==int(SizeofAllKappa*0.1))
											{
												pro->SetprogressPos(10);
											}
											if(sub==int(SizeofAllKappa*0.2))
											{
												pro->SetprogressPos(20);
											}
											if(sub==int(SizeofAllKappa*0.3))
											{
												pro->SetprogressPos(30);
											}
											if(sub==int(SizeofAllKappa*0.4))
											{
												pro->SetprogressPos(40);
											}
											if(sub==int(SizeofAllKappa*0.5))
											{
												pro->SetprogressPos(50);
											}
											if(sub==int(SizeofAllKappa*0.6))
											{
												pro->SetprogressPos(60);
											}
											if(sub==int(SizeofAllKappa*0.7))
											{
												pro->SetprogressPos(70);
											}
											if(sub==int(SizeofAllKappa*0.8))
											{
												pro->SetprogressPos(80);
											}
											if(sub==int(SizeofAllKappa*0.90))
											{
												pro->SetprogressPos(90);
											}
										}
										Mutempofclass23+=step;
									}
									Mutempofclass13+=step;
								}
								Mutempofclass32+=step;
							}
							Mutempofclass22+=step;
						}
						Mutempofclass12+=step;
					}
					Mutempofclass31+=step;
				}
				Mutempofclass21+=step;
			}
			Mutempofclass11+=step;
		}
	}

	else if (dim_num==3 && class_num==4)
	{
		float Mutempofclass11=Init_Mu[0]+IteratRegion[0];//��һ���һά����
		float Mutempofclass21=Init_Mu[1]+IteratRegion[2];//�ڶ����һά����
		float Mutempofclass31=Init_Mu[2]+IteratRegion[4];//�������һά����
		float Mutempofclass41=Init_Mu[3]+IteratRegion[6];//�������һά����
		float Mutempofclass12=Init_Mu[4]+IteratRegion[8];//��һ��ڶ�ά����
		float Mutempofclass22=Init_Mu[5]+IteratRegion[10];//�ڶ���ڶ�ά����
		float Mutempofclass32=Init_Mu[6]+IteratRegion[12];//������ڶ�ά����
		float Mutempofclass42=Init_Mu[7]+IteratRegion[14];//������ڶ�ά����
		float Mutempofclass13=Init_Mu[8]+IteratRegion[16];//��һ�����ά����
		float Mutempofclass23=Init_Mu[9]+IteratRegion[18];//�ڶ������ά����
		float Mutempofclass33=Init_Mu[10]+IteratRegion[20];//���������ά����
		float Mutempofclass43=Init_Mu[11]+IteratRegion[22];//���������ά����
		int sub=0;
		for (int m11=0;m11<iterat_num[0];m11++)
		{
			for (int m21=0;m21<iterat_num[1];m21++)
			{
				for (int m31=0;m31<iterat_num[2];m31++)
				{
					for (int m41=0;m41<iterat_num[3];m41++)
					{
						for (int m12=0;m12<iterat_num[4];m12++)
						{
							for (int m22=0;m22<iterat_num[5];m22++)
							{
								for (int m32=0;m32<iterat_num[6];m32++)
								{
									for (int m42=0;m42<iterat_num[7];m42++)
									{
										for (int m13=0;m13<iterat_num[8];m13++)
										{
											for (int m23=0;m23<iterat_num[9];m23++)
											{
												for (int m33=0;m33<iterat_num[10];m33++)
												{
													for (int m43=0;m43<iterat_num[11];m43++)
													{
														Mu_temp[0]=Mutempofclass11;
														Mu_temp[1]=Mutempofclass21;
														Mu_temp[2]=Mutempofclass31;
														Mu_temp[3]=Mutempofclass41;
														Mu_temp[4]=Mutempofclass12;
														Mu_temp[5]=Mutempofclass22;
														Mu_temp[6]=Mutempofclass32;
														Mu_temp[7]=Mutempofclass42;
														Mu_temp[8]=Mutempofclass13;
														Mu_temp[9]=Mutempofclass23;
														Mu_temp[10]=Mutempofclass33;
														Mu_temp[11]=Mutempofclass43;
														GMM_multi(Data,Data_size,class_num,dim_num,itol,maxiter,Init_Weight,Mu_temp,Init_Sigma);//GMM����
														kappa_evalu_multi_DAGOrBiTree(Classifyruleflag,Strategy,flag,kappaevalu_step);
														AllKappa[sub]=kappa;
														sub++;
														Mutempofclass43+=step;
														if(sub==int(SizeofAllKappa*0.1))
														{
															pro->SetprogressPos(10);
														}
														if(sub==int(SizeofAllKappa*0.2))
														{
															pro->SetprogressPos(20);
														}
														if(sub==int(SizeofAllKappa*0.3))
														{
															pro->SetprogressPos(30);
														}
														if(sub==int(SizeofAllKappa*0.4))
														{
															pro->SetprogressPos(40);
														}
														if(sub==int(SizeofAllKappa*0.5))
														{
															pro->SetprogressPos(50);
														}
														if(sub==int(SizeofAllKappa*0.6))
														{
															pro->SetprogressPos(60);
														}
														if(sub==int(SizeofAllKappa*0.7))
														{
															pro->SetprogressPos(70);
														}
														if(sub==int(SizeofAllKappa*0.8))
														{
															pro->SetprogressPos(80);
														}
														if(sub==int(SizeofAllKappa*0.90))
														{
															pro->SetprogressPos(90);
														}
													}
													Mutempofclass33+=step;
												}
												Mutempofclass23+=step;
											}
											Mutempofclass13+=step;
										}
										Mutempofclass42+=step;
									}
									Mutempofclass32+=step;
								}
								Mutempofclass22+=step;
							}
							Mutempofclass12+=step;
						}
						Mutempofclass41+=step;
					}
					Mutempofclass31+=step;
				}
				Mutempofclass21+=step;
			}
			Mutempofclass11+=step;
		}
	}
	else//��ά�����ݲ�����
	{
	}
	kappa=HighestFrequenceInArray(AllKappa,SizeofAllKappa);//Ѱ�����Ƶ�ʵ�kappa
	delete [] AllKappa;
	delete [] Mu_temp;
	delete [] iterat_num;
} 