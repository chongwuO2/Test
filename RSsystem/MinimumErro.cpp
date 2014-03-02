#include "StdAfx.h"
#include "Image_kappa_evaluation.h"
#include <iostream>
using namespace std;

//利用最小错误率时Kappa计算中调用函数
//一维的情况
//Data[Data_size]为输入的数据，class_num为类别数
//weight[class_num]为各类的权重。Center[class_num]为各类的中心
//sigma[class_num]为各类特征的标准差
//最后依据最小错误率准则判断各data属于的类别号放在ResultofLabel中
//标号为1~class_num,按照各参数给定的顺序依次为1~class_num类
void Image_kappa_evalu::MinimumErroClassify_singl(float* Data,const int Data_size,const int class_num,float *weight,float* Center,float *sigma,int* ResultofLabel)
{
	float** Gaussian_value=new float*[class_num];
	for (int i=0;i<class_num;i++)
	{
		Gaussian_value[i]=new float[Data_size];
		memset(Gaussian_value[i],0,sizeof(float)*Data_size);
	}

	for (int i=0;i<class_num;i++)
	{
		for (int j=0;j<Data_size;j++)
		{   
			float a1=1.0/(sqrt(2.0*3.141593)*sigma[i]);
			float a2=2.0*pow(sigma[i],2);
			float a3=Data[j]-Center[i];
			float a4=-pow(a3,2)/a2;
			Gaussian_value[i][j]=weight[i]*a1*exp(a4);
		}
	}

	for (int i=0;i<Data_size;i++)
	{   
		float max=-1e10;
		int flag=-1;
		for (int j=0;j<class_num;j++)
		{
			if (Gaussian_value[j][i]-max>1e-10)
			{
				max=Gaussian_value[j][i];
				flag=j;
			}
		}
		ResultofLabel[i]=flag+1;
	}
	
	for (int i=0;i<class_num;i++)
	{
		delete [] Gaussian_value[i];
	}
	delete [] Gaussian_value;
}




//利用最小错误率时Kappa计算中调用函数
//多维的情况
//Data[Data_size]为输入的数据，dim_size为维数，class_num为类别数
//weight[class_num]为各类的权重。Center[dim_size][class_num]为各类各维的中心
//sigma[class_num][dim_size][dim_size]为各类特征的协方差
//最后依据最小错误率准则判断各data属于的类别号放在ResultofLabel中
void Image_kappa_evalu::MinimumErroClassify_multi(float** Data,const int Data_size,const int dim_size,const int class_num,float *weight,float** Center,float ***sigma,int* ResultofLabel)
{
	float** Gaussian_value=new float*[class_num];
	for (int i=0;i<class_num;i++)
	{
		Gaussian_value[i]=new float[Data_size];
		memset(Gaussian_value[i],0,sizeof(float)*Data_size);
	}
	float* X_temp=new float[dim_size];
	float* Mu_temp=new float[dim_size];
	float**Sigma_temp=new float*[dim_size];
	for (int i=0;i<dim_size;i++)
	{
		Sigma_temp[i]=new float[dim_size];
	}

	for (int i=0;i<class_num;i++)
	{
		for (int j=0;j<Data_size;j++)
		{   
			for (int k=0;k<dim_size;k++)
			{
				X_temp[k]=Data[k][j];
				Mu_temp[k]=Center[k][i];
				for (int q=0;q<dim_size;q++)
				{
					Sigma_temp[k][q]=sigma[i][k][q];
				}
			}
			Gaussian_value[i][j]=weight[i]*mvnpdf(X_temp,Mu_temp,Sigma_temp,dim_size);		
		}
	}

	for (int i=0;i<Data_size;i++)
	{   
		float max=-1e10;
	    int flag=-1;
		for (int j=0;j<class_num;j++)
		{
			if (Gaussian_value[j][i]-max>1e-10)
			{
				max=Gaussian_value[j][i];
				flag=j;
			}
		}
	    ResultofLabel[i]=flag+1;
	}
	for (int i=0;i<dim_size;i++)
	{
		delete [] Sigma_temp[i];
	}
	delete [] Sigma_temp;
	delete [] Mu_temp;
	delete [] X_temp;
	for (int i=0;i<class_num;i++)
	{
		delete [] Gaussian_value[i];
	}
	delete [] Gaussian_value;
}



//一维特征基于最小错误率的kappa计算
//类别数为Image_kappa_evalu类中的成员变量class_numb
//各类的权重、均值、标准差都保存在Image_kappa_evalu类中的Mu，Sigma,Weight
void Image_kappa_evalu::kappa_evalu_singl_minimumerro()
{
	const int class_num=class_numb;
	float* mu=new float[class_num];
	float* sigma=new float[class_num];
	float* weight=new float[class_num];
	for (int i=0;i<class_num;i++)
	{
		mu[i]=Mu[i];
		sigma[i]=Sigma[i];
		weight[i]=Weight[i];
	}
	const float range_min=-50.0;
	const float range_max=300.0;
	const float step=0.001;
	const int NumofPoint=floor((range_max-range_min)/step)+1;
	float* Data=new float[NumofPoint];
	int * resultoflabel=new int[NumofPoint];
	memset(resultoflabel,0,sizeof(int)*NumofPoint);
	float temp1=range_min;
	for (int i=0;i<NumofPoint;i++)
	{
		Data[i]=temp1;
		temp1+=step;
	}
	MinimumErroClassify_singl(Data,NumofPoint,class_num,weight,mu,sigma,resultoflabel);
	float ** Resultofnormpdf=new float* [class_num];
	float ** p=new float*[class_num];
	for (int i=0;i<class_num;i++)
	{
		Resultofnormpdf[i]=new float[NumofPoint];
		p[i]=new float[class_num];
		memset(p[i],0,sizeof(float)*class_num);
	}
	for (int i=0;i<class_num;i++)
	{
		for (int j=0;j<NumofPoint;j++)
		{   
			float a1=1.0/(sqrt(2.0*3.141593)*sigma[i]);
			float a2=2.0*pow(sigma[i],2);
			float a3=Data[j]-mu[i];
			float a4=-pow(a3,2)/a2;
			Resultofnormpdf[i][j]=step*a1*exp(a4);
			int flag=resultoflabel[j];
			p[i][flag-1]+=Resultofnormpdf[i][j];
		}
	}

	if (class_num==2)//两类两维
	{
		float p11=weight[0]*p[0][0];
		float p12=weight[0]*p[0][1];
		float p21=weight[1]*p[1][0];
		float p22=weight[1]*p[1][1];
		kappa=1-(p12+p21)/(1-weight[0]*(p11+p21)-weight[1]*(p22+p12));
	}
	else if (class_num==3) //三类两维
	{
		float p11=weight[0]*p[0][0];
		float p12=weight[0]*p[0][1];
		float p13=weight[0]*p[0][2];
		float p21=weight[1]*p[1][0];
		float p22=weight[1]*p[1][1];
		float p23=weight[1]*p[1][2];
		float p31=weight[2]*p[2][0];
		float p32=weight[2]*p[2][1];
		float p33=weight[2]*p[2][2];
		float p1_other=p21+p31;
		float p2_other=p12+p32;
		float p3_other=p13+p23;
		kappa=1-(p1_other+p2_other+p3_other)/(1-weight[0]*(p11+p1_other)-weight[1]*(p22+p2_other)-weight[2]*(p33+p3_other));
	}
	else if (class_num==4) //四类两维
	{
		float p11=weight[0]*p[0][0];
		float p12=weight[0]*p[0][1];
		float p13=weight[0]*p[0][2];
		float p14=weight[0]*p[0][3];
		float p21=weight[1]*p[1][0];
		float p22=weight[1]*p[1][1];
		float p23=weight[1]*p[1][2];
		float p24=weight[1]*p[1][3];
		float p31=weight[2]*p[2][0];
		float p32=weight[2]*p[2][1];
		float p33=weight[2]*p[2][2];
		float p34=weight[2]*p[2][3];
		float p41=weight[3]*p[3][0];
		float p42=weight[3]*p[3][1];
		float p43=weight[3]*p[3][2];
		float p44=weight[3]*p[3][3];
		float p1_other=p21+p31+p41;
		float p2_other=p12+p32+p42;
		float p3_other=p13+p23+p43;
		float p4_other=p14+p24+p34;
		kappa=1-(p1_other+p2_other+p3_other+p4_other)/(1-weight[0]*(p11+p1_other)-weight[1]*(p22+p2_other)-weight[2]*(p33+p3_other)-weight[3]*(p44+p4_other));

	}
	else
		kappa=-2;//多于四类暂时不考虑

	delete [] weight;
	delete [] mu;
	delete [] sigma;
	delete [] Data;
	delete [] resultoflabel;
	for (int i=0;i<class_num;i++)
	{
		delete [] p[i];
		delete [] Resultofnormpdf[i];
	}
	delete [] p;
	delete [] Resultofnormpdf;
		
}

//DAG下kappa计算调用函数,基于最小错误率
void Image_kappa_evalu::MinimumErroClassify_DAG(float** Data,const int Data_size,const int dim_size,const int class_num,float *weight,float** Center,float ***sigma,int** Stratg,int* ResultofLabel)
{
	const int numofStratgline=class_num*(class_num-1)/2;
	const int numofStratgcolumn=2+dim_size;

	int** ResultofAllClassifiers=new int*[numofStratgline];
	for (int i=0;i<numofStratgline;i++)
	{
		ResultofAllClassifiers[i]=new int[Data_size];
		memset(ResultofAllClassifiers[i],0,sizeof(int)*Data_size);
	}

	for (int i=0;i<numofStratgline;i++)
	{
		//统计每个分类器所使用的特征个数
		int NumofFeatures=0;
		for (int j=2;j<numofStratgcolumn;j++)
		{
			if (Stratg[i][j]==1)
			{
				NumofFeatures++;
			}
		}
		//记录使用特征的标号
		int * indexofFeature=new int[NumofFeatures];
		int sub=0;
		for (int k=2;k<numofStratgcolumn;k++)
		{
			if (Stratg[i][k]==1)
			{
				indexofFeature[sub]=k-2;
				sub++;
			}
		}
		float** Data_temp=new float*[NumofFeatures];
		for (int j=0;j<NumofFeatures;j++)
		{
			Data_temp[j]=new float[Data_size];
			memset(Data_temp[j],0,sizeof(float)*Data_size);
		}
		for (int j=0;j<NumofFeatures;j++)
		{
			for (int k=0;k<Data_size;k++)
			{
				int numofF=indexofFeature[j];
				Data_temp[j][k]=Data[numofF][k];
			}

		}

		float** Mu_temp=new float*[NumofFeatures];
		float* Weight_temp=new float[2];
		float*** Sigma_temp=new float**[2];
		for (int j=0;j<2;j++)
		{
			Sigma_temp[j]=new float*[NumofFeatures];
		}
		for (int j=0;j<2;j++)
		{
			for (int k=0;k<NumofFeatures;k++)
			{
				Sigma_temp[j][k]=new float[NumofFeatures];
			}
		}
		for (int j=0;j<NumofFeatures;j++)
		{
			Mu_temp[j]=new float[2];
		}

		for (int j=0;j<2;j++)
		{
			int indexofclassifier=Stratg[i][j]-1;
			Weight_temp[j]=weight[indexofclassifier];
			for (int k=0;k<NumofFeatures;k++)
			{
				int numofF1=indexofFeature[k];
				Mu_temp[k][j]=Center[numofF1][indexofclassifier];
				for (int q=0;q<NumofFeatures;q++)
				{
					int numofF2=indexofFeature[q];
					Sigma_temp[j][k][q]=sigma[indexofclassifier][numofF1][numofF2];
				}
			}
		}
		MinimumErroClassify_multi(Data_temp,Data_size,NumofFeatures,2,Weight_temp,Mu_temp,Sigma_temp,ResultofAllClassifiers[i]);
		delete [] indexofFeature;

		for (int j=0;j<NumofFeatures;j++)
		{
			delete [] Mu_temp[j];
			delete [] Data_temp[j];
		}
		delete [] Mu_temp;
		delete [] Weight_temp;
		delete [] Data_temp;
		for (int j=0;j<2;j++)
		{
			for (int k=0;k<NumofFeatures;k++)
			{
				delete [] Sigma_temp[j][k];
			}
		}
		for (int j=0;j<2;j++)
		{
			delete [] Sigma_temp[j];
		}
		delete [] Sigma_temp;
	}

	if (class_num==3)
	{
		for (int j=0;j<Data_size;j++)
		{
			if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==1)
			{
				ResultofLabel[j]=Stratg[1][0];
			}
			else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==2)
			{
				ResultofLabel[j]=Stratg[1][1];
			}
			else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==1)
			{
				ResultofLabel[j]=Stratg[2][0];
			}
			else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==2)
			{
				ResultofLabel[j]=Stratg[2][1];
			}
			else
			{

			}

		}
	}

	else if (class_num==4)
	{
		for (int j=0;j<Data_size;j++)
		{
			if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==1  && ResultofAllClassifiers[3][j]==1)
			{
				ResultofLabel[j]=Stratg[3][0];
			}
			else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==1  && ResultofAllClassifiers[3][j]==2)
			{
				ResultofLabel[j]=Stratg[3][1];
			}
			else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==2  && ResultofAllClassifiers[4][j]==1)
			{
				ResultofLabel[j]=Stratg[4][0];
			}
			else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==2  && ResultofAllClassifiers[4][j]==2)
			{
				ResultofLabel[j]=Stratg[4][1];
			}
			else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==1  && ResultofAllClassifiers[4][j]==1)
			{
				ResultofLabel[j]=Stratg[4][0];
			}
			else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==1  && ResultofAllClassifiers[4][j]==2)
			{
				ResultofLabel[j]=Stratg[4][1];
			}
			else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==2  && ResultofAllClassifiers[5][j]==1)
			{
				ResultofLabel[j]=Stratg[5][0];
			}
			else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==2  && ResultofAllClassifiers[5][j]==2)
			{
				ResultofLabel[j]=Stratg[5][1];
			}
			else
			{

			}

		}	

	}
	else if(class_num==5)
	{

		for (int j=0;j<Data_size;j++)
		{
			if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==1  && ResultofAllClassifiers[3][j]==1 && ResultofAllClassifiers[6][j]==1 )
			{
				ResultofLabel[j]=Stratg[6][0];
			}
			else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==1  && ResultofAllClassifiers[3][j]==1 && ResultofAllClassifiers[6][j]==2 )
			{
				ResultofLabel[j]=Stratg[6][1];
			}
			else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==1  && ResultofAllClassifiers[3][j]==2 && ResultofAllClassifiers[7][j]==1)
			{
				ResultofLabel[j]=Stratg[7][0];
			}
			else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==1  && ResultofAllClassifiers[3][j]==2 && ResultofAllClassifiers[7][j]==2)
			{
				ResultofLabel[j]=Stratg[7][1];
			}
			else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==2  && ResultofAllClassifiers[4][j]==1 && ResultofAllClassifiers[7][j]==1)
			{
				ResultofLabel[j]=Stratg[7][0];
			}
			else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==2  && ResultofAllClassifiers[4][j]==1 && ResultofAllClassifiers[7][j]==2)
			{
				ResultofLabel[j]=Stratg[7][1];
			}
			else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==1  && ResultofAllClassifiers[4][j]==1 && ResultofAllClassifiers[7][j]==1)
			{
				ResultofLabel[j]=Stratg[7][0];
			}
			else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==1  && ResultofAllClassifiers[4][j]==1 && ResultofAllClassifiers[7][j]==2)
			{
				ResultofLabel[j]=Stratg[7][1];
			}
			else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==2  && ResultofAllClassifiers[4][j]==2 && ResultofAllClassifiers[8][j]==1 )
			{
				ResultofLabel[j]=Stratg[8][0];
			}
			else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==2  && ResultofAllClassifiers[4][j]==2 && ResultofAllClassifiers[8][j]==2)
			{
				ResultofLabel[j]=Stratg[8][1];
			}
			else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==1  && ResultofAllClassifiers[4][j]==2 && ResultofAllClassifiers[8][j]==1)
			{
				ResultofLabel[j]=Stratg[8][0];
			}
			else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==1  && ResultofAllClassifiers[4][j]==2 && ResultofAllClassifiers[8][j]==2)
			{
				ResultofLabel[j]=Stratg[8][1];
			}
			else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==2  && ResultofAllClassifiers[5][j]==1 && ResultofAllClassifiers[8][j]==1)
			{
				ResultofLabel[j]=Stratg[8][0];
			}
			else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==2  && ResultofAllClassifiers[5][j]==1 && ResultofAllClassifiers[8][j]==2)
			{
				ResultofLabel[j]=Stratg[8][1];
			}
			else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==2  && ResultofAllClassifiers[5][j]==2 && ResultofAllClassifiers[9][j]==1)
			{
				ResultofLabel[j]=Stratg[9][0];
			}
			else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==2  && ResultofAllClassifiers[5][j]==2 && ResultofAllClassifiers[9][j]==2)
			{
				ResultofLabel[j]=Stratg[9][1];
			}
			else
			{
				ResultofLabel[j]=0;
			}

		}	

	}
	else
	{

	}

	for (int i=0;i<numofStratgline;i++)
	{
		delete [] ResultofAllClassifiers[i]; 
	}
	delete [] ResultofAllClassifiers;

}