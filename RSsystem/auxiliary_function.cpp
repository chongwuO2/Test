#include "StdAfx.h"
#include <algorithm>
#include <iostream>
using namespace std;

//一维正态积分函数
//sigma为标准差
float normcdf(const float x, const float mu,const float sigma,const float interval=0.001) 
{
	float lower_boundary=mu-3.5*sigma;
	float up_boundary=mu+3.5*sigma;
	float sum=0;
	if (x<lower_boundary)
		return sum;
	else if (x>up_boundary)
	{
		sum=1;
		return sum;
	}
	else
		{
			for (float i=lower_boundary;i<x;i+=interval)
	          {
		          float a1=1.0/(sqrt(2.0*3.141593)*sigma);
		          float a2=2.0*pow(sigma,2);
		          float a3=i-mu;
		          float a4=-pow(a3,2)/a2;
		          float Gaussian_value=a1*exp(a4);
		          sum+=Gaussian_value*interval;
	          }
	       return sum;
	}
}


//多维正态概率密度函数
//X[dim_num]为输入的X向量，Mu[dim_num]为均值，Sigma[dim_num][dim_num]为协方差
//概率密度函数值>=0,且要求协方差为对称正定阵，如果不满足条件，函数返回值为-1，标记出错了
float mvnpdf(float* X,float* Mu,float ** Sigma,const int dim_num)
{
	float det(float* A,int m,int n);   /*求矩阵行列式*/ 
	void MatrixTranspose(float* A,float* B,int m,int n) ;//求矩阵转置
	void MatrixInverse(float* A,float* C,int m,int n) ;  /*矩阵求逆,B是A的逆*/ 
	float* sigma=new float[dim_num*dim_num];
	int count=0;
	for (int i=0;i<dim_num;i++)
	{
		for (int j=0;j<dim_num;j++)
		{
			sigma[i*dim_num+j]=Sigma[i][j];
			if (abs(Sigma[i][j]-Sigma[j][i])<1e-3)
			{
				count+=1;
			}
		}
		
	}
	float sigma_det=det(sigma,dim_num,dim_num);
	
	if (abs(sigma_det)<=1e-8 || count!= dim_num*dim_num)
	{
		return -1.0;   //Sigma必须为对称正定矩阵
	}
	float* ssigma_inv=new float[dim_num*dim_num];
	memset(ssigma_inv,0,dim_num*dim_num*sizeof(float));
	MatrixInverse(sigma,ssigma_inv,dim_num,dim_num);//sigma_inv为sigma的逆
	float** Sigma_inv=new float*[dim_num];
	for (int i=0;i<dim_num;i++)
	{
		Sigma_inv[i]=new float[dim_num];
	}
	for (int i=0;i<dim_num;i++)
	{
		for (int j=0;j<dim_num;j++)
		{
			Sigma_inv[i][j]=ssigma_inv[i*dim_num+j];
		}
	}
   float *x_u=new float[dim_num];
   for (int i=0;i<dim_num;i++)
   {
	   x_u[i]=X[i]-Mu[i];
   }

   float *x_u_Sigma_inv=new float[dim_num];
   memset(x_u_Sigma_inv,0,dim_num*sizeof(float));
   for (int i=0;i<dim_num;i++)
   {
	   for (int j=0;j<dim_num;j++)
	   {
		   x_u_Sigma_inv[i]+=x_u[j]*Sigma_inv[j][i];
	   }
   }
   float a1=0;
   for (int i=0;i<dim_num;i++)
   {
	   a1+=x_u_Sigma_inv[i]*x_u[i];
   }

   float a2=pow(2*3.1415926,dim_num);
   float a3=sqrt(a2*sigma_det);
   float Result=1/a3*exp(-0.5*a1);

   delete []x_u_Sigma_inv;
   delete [] x_u;
   delete [] ssigma_inv;
	for (int i=0;i<dim_num;i++)
	{
		delete[] Sigma_inv[i];
	}
	delete [] Sigma_inv;
	delete [] sigma;
	

	return Result;
}





void sort_mu(float * mu,float* weight,float * sigma,const int class_num)//排序，根据mu的大小weight,sigma相应调整顺序
{ 
	int* suffix=new int[class_num];
	float * mu_sort=new float[class_num];
	float * weight_sort=new float[class_num];
	float * sigma_sort=new float[class_num];
    for (int i=0;i<class_num;i++)
    {
		mu_sort[i]=mu[i];
		weight_sort[i]=weight[i];
		sigma_sort[i]=sigma[i];
    }
	sort(mu,mu+class_num);
	for (int i=0;i<class_num;i++)
	{
		float temp_mu=mu[i];
		for (int j=0;j<class_num;j++)
		{
			if (temp_mu==mu_sort[j])
			{
				suffix[i]=j;
				break;
			}
		}
	}
	for (int i=0;i<class_num;i++)
	{   
		weight[i]=weight_sort[suffix[i]];
		sigma[i]=sigma_sort[suffix[i]];
	}
	delete[] suffix;
	delete[] mu_sort;
	delete[] sigma_sort;
	delete[] weight_sort;

}


float det(float* A,int m,int n)   /*求矩阵行列式*/ 
{         
	int   i,j,k,p,r; 
	float   X,temp=1,temp1=1,s=0,s1=0; 

	if(n==2) 
	{ 
		for(i=0;i <m;i++) 
			for(j=0;j <n;j++) 
				if((i+j)%2)   temp1*=A[i*n+j]; 
				else   temp*=A[i*n+j]; 
				X=temp-temp1; 
	} 
	else 
	{ 
		for(k=0;k <n;k++) 
		{ 
			for(i=0,j=k;i <m,j <n;i++,j++) 
				temp*=A[i*n+j]; 
			if(m-i) 
			{ 
				for(p=m-i,r=m-1;p> 0;p--,r--) 
					temp*=A[r*n+p-1]; 
			} 
			s+=temp; 
			temp=1; 
		} 

		for(k=n-1;k>=0;k--) 
		{ 
			for(i=0,j=k;i <m,j>=0;i++,j--) 
				temp1*=A[i*n+j]; 
			if(m-i) 
			{for(p=m-1,r=i;r <m;p--,r++) 
			temp1*=A[r*n+p];} 
			s1+=temp1; 
			temp1=1; 
		}                 
		X=s-s1; 
	} 
	return   X; 
}   


void MatrixTranspose(float* A,float* B,int m,int n)   //矩阵转置，B是A的转置
{
	int   i,j; 
	for(i=0;i <n;i++) 
	{
		for(j=0;j <m;j++) 
		{B[i*m+j]=A[j*n+i];} 
	}
}


void MatrixInverse(float* A,float* C,int m,int n)   /*矩阵求逆,C是A的逆*/ 
{
	int   i,j,x,y,k; 
	float* SP=new float[m*n];
    float* AB=new float[m*n];
	float* B=new float[m*n];
	float X=det(A,m,n); 
	X=1/X; 
	for(i=0;i <m;i++) 
		for(j=0;j <n;j++) 
		{ 
			for(k=0;k <m*n;k++) 
				B[k]=A[k]; 
			{ 
				for(x=0;x <n;x++) 
					B[i*n+x]=0; 
				for(y=0;y <m;y++) 
					B[m*y+j]=0; 
				B[i*n+j]=1; 
				SP[i*n+j]=det(B,m,n); 
				AB[i*n+j]=X*SP[i*n+j]; 
			} 
		} 
		MatrixTranspose(AB,C,m,n);
		delete [] SP;
		delete [] AB;
		delete [] B;
}

void normlize(float * Array,const int array_size,const float range_min,const float range_max)
{
	float min=pow(2.0,31);
	float max=-pow(2.0,31);
	for (int i=0;i<array_size;i++)
	{
		if (Array[i]-max>1e-5)
		{
			max=Array[i];
		}
		if (min-Array[i]>1e-5)
		{
			min=Array[i];
		}
	}
	for (int i=0;i<array_size;i++)
	{
		Array[i]=range_min+(Array[i]-min)/(max-min)*(range_max-range_min);
		float a1=floor(Array[i]);
		float a2=ceil(Array[i]);
		float a3=(a1+a2)/2;
		if (Array[i]-a3>1e-5)
		{
			Array[i]=a2;
		}
		else
			Array[i]=a1;
	}
}