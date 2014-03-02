#include "StdAfx.h"
#include "Image_kappa_evaluation.h"
#include <iostream>
using namespace std;

Image_kappa_evalu::Image_kappa_evalu()
{
	this->xlen = 0;
	this->ylen = 0;
	this->zlen = 0;
	this->imgsize=0;
	this->band_selec=0;
	this->class_numb=0;
	this->dim_numb=0;
	this->kappa=0;
	this->Image_Gray= NULL;	
	this->TextureFeatures=NULL;
	this->Weight=NULL;
	this->Mu=NULL;
	this->Sigma=NULL;

	//参考图部分
    ptoImage_Ref=NULL;

	//最小距离分类部分
	this->labelofClassification=NULL;
	KappaofReal=0;

}

Image_kappa_evalu::~Image_kappa_evalu()
{
	CPLFree(Image_Gray);
	delete Weight;
	delete Mu;
	delete Sigma;

	//最小距离分类部分
	delete labelofClassification;

	//提取纹理部分
	for (int i=0;i<5;i++)
	{
		delete [] TextureFeatures[i];
	}
	delete [] TextureFeatures;
	
};

void Image_kappa_evalu::init_GrayImg(CString str,int band_sec)
{   
	GDALDataset * pdataset;          // 在这里数据集即为理解为图像文件  
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");//设置可以读入带有中文的路径
	pdataset = (GDALDataset *)GDALOpen(str,GA_ReadOnly);//文件的打开使用的是GDALOpen函数
	xlen = pdataset->GetRasterXSize();
	ylen = pdataset->GetRasterYSize();
	zlen = pdataset->GetRasterCount();
	band_selec=band_sec;
	imgsize = xlen * ylen;
	GDALRasterBand *pBand;
	pBand =pdataset->GetRasterBand(band_selec);//选择波段
	if (pBand== NULL)  
	{  
		cout<<"pBand Fail"<<endl;  
	} 
	Image_Gray = (float*)CPLMalloc(sizeof(float)*imgsize);
	pBand->RasterIO(GF_Read,0,0,xlen,ylen,Image_Gray,xlen,ylen,GDT_Float32,0,0);//提取图像的灰度
	GDALClose(pdataset);
}


void Image_kappa_evalu::init_Parameter(int class_num,int dim_num )	
{
	this->class_numb=class_num;
	this->dim_numb=dim_num;
	this->Weight=(float*)new float[class_numb];
	this->Mu=(float*)new float[class_numb*dim_numb];
	this->Sigma=(float*)new float[class_numb*dim_numb*dim_numb];

}


//一维特征的GMM估计
//Data为输入，Data_size为输入数据的长度，即，Data[Data_size],itol为迭代停止条件，maxiter为最大迭代次数
//Init_Weight等参数依次为各类的权重等参数
//固定mu的话为mu1[i]=mu0[i];不固定的话mu1[i]参与迭代,现在已写成固定均值的形式
void Image_kappa_evalu::GMM_singl(float* Data,const int Data_size,const int class_number,const double itol,const int maxiter,float * Init_Weight, float *Init_Mu,float* Init_Sigma)
{
	const int Data_num=Data_size;
	const int class_num=class_number;
	int nstep=0;
	float * weight0=new float[class_num];
	float * weight1=new float[class_num];
	float * mu0=new float[class_num];
	float * mu1=new float[class_num];
	float * sigma0=new float[class_num];
	float * sigma1=new float[class_num];
	float ** Gaussian_value;
	float ** Beta_value;
	Gaussian_value=new float*[class_num];
	Beta_value=new float*[class_num];
	for (int i=0;i<class_num;i++)
	{
        Gaussian_value[i]=new float[Data_num];
		memset(Gaussian_value[i],0,sizeof(float)*Data_num);
		Beta_value[i]= new float[Data_num];
		memset(Beta_value[i],0,sizeof(float)*Data_num);
		weight0[i]=Init_Weight[i];
		weight1[i]=0;
		mu0[i]=Init_Mu[i];
		mu1[i]=0;
		sigma0[i]=Init_Sigma[i];
		sigma1[i]=0;
	}
	float sum_mu,sum_weight,sum_sigma;
	sum_mu=sum_sigma=sum_weight=0;
	for (int i=0;i<class_num;i++)
	{
		sum_mu+=abs(mu0[i]-mu1[i]);
		sum_weight+=abs(weight0[i]-weight1[i]);
		sum_sigma+=abs(sigma0[i]-sigma1[i]);

	}
	
	while (nstep<maxiter && sum_mu>itol && sum_weight >itol && sum_sigma>itol  && sum_mu+sum_sigma+sum_weight>itol)
	{
	   nstep+=1;
		
		//计算Gaussian[i][j]
		for (int i=0;i<class_num;i++)
	      {
		    for (int j=0;j<Data_num;j++)
		    {
             float a1=1.0/(sqrt(2.0*3.141593)*sigma0[i]);
			 float a2=2.0*pow(sigma0[i],2);
			 float a3=Data[j]-mu0[i];
			 float a4=-pow(a3,2)/a2;
			 Gaussian_value[i][j]=a1*exp(a4);	
		    }
	      }
		//计算Beta[i][j]
		float* sum_Betaj=new float[Data_num];
		for (int i=0;i<Data_num;i++)
		{
		   sum_Betaj[i]=0;
		   for (int j=0;j<class_num;j++)
		   {
			   sum_Betaj[i]+=weight0[j]*Gaussian_value[j][i];
		   }
		}
		
		for (int i=0;i<class_num;i++)
		{
			for (int j=0;j<Data_num;j++)
			{
				Beta_value[i][j]=weight0[i]*Gaussian_value[i][j]/sum_Betaj[j];	
			}
		}
       delete [] sum_Betaj;

	   //计算权重，均值，标准差
	   float* sum_beta_i=new float[class_num];
	   float* sum_beta_ix=new float[class_num];
	   float* sum_beta_ixu=new float[class_num];
	 
	   for (int i=0;i<class_num;i++)
	   {
		   sum_beta_i[i]=0;
		   sum_beta_ix[i]=0;
		   sum_beta_ixu[i]=0;
		   for (int j=0;j<Data_num;j++)
		   {
			   sum_beta_i[i]+=Beta_value[i][j];
			   sum_beta_ix[i]+=Beta_value[i][j]*Data[j];
			   sum_beta_ixu[i]+=Beta_value[i][j]*pow((Data[j]-mu0[i]),2);
		   }
	   }
        for (int i=0;i<class_num;i++)
		{
			//mu1[i]=sum_beta_ix[i]/sum_beta_i[i];//不固定均值时，mu1为迭代值
			mu1[i]=mu0[i];//固定均值
			weight1[i]=sum_beta_i[i]/Data_num;
			sigma1[i]=sqrt(sum_beta_ixu[i]/sum_beta_i[i]);
		}
		delete [] sum_beta_i;
		delete [] sum_beta_ix;
		delete [] sum_beta_ixu;

		//float sum_mu,sum_weight,sum_sigma;
		sum_mu=sum_sigma=sum_weight=0;
		for (int i=0;i<class_num;i++)
		{
			sum_mu+=abs(mu0[i]-mu1[i]);
			sum_weight+=abs(weight0[i]-weight1[i]);
			sum_sigma+=abs(sigma0[i]-sigma1[i]);

		}
		
        for (int i=0;i<class_num;i++)
        {
			mu0[i]=mu1[i];
			weight0[i]=weight1[i];
			sigma0[i]=sigma1[i];
        }
     }
	 for (int i=0;i<class_num;i++)
	 {
		 this->Weight[i]=weight1[i];
		 this->Mu[i]=mu1[i];
		 this->Sigma[i]=sigma1[i];	 
	 }
	 delete [] weight0;
	 delete [] weight1;
	 delete [] mu0;
	 delete [] mu1;
	 delete [] sigma0;
	 delete [] sigma1;
	 for(int i=0;i<class_numb;i++)
		{
			delete [] Gaussian_value[i];
			delete [] Beta_value[i];
	    }
	 delete [] Gaussian_value;
	 delete [] Beta_value;
}

//多维特征GMM估计
//Data[dim_num][Data_size]为输入的特征   Data_size数据必须为实际长度，否则有数组越界危险
// Init_Weight为初始各类的权重  Weight[class_numb]
//Init_Mu为初始各类的均值  Mu[class_numb*dim_numb] Mu[11 21 31 12 22 32]
//Init_Sigma为初始各类的标准差 Sigma[class_numb*dim_numb*dim_numb] Sigma[111 112 121 122   211 212 221 222 311 312 321 322 ]
//固定均值时mu1[i][j]=mu0[i][j],不固定均值时mu1为迭代值
void Image_kappa_evalu::GMM_multi(float** Data,const int Data_size,const int class_number,const int dim_num,const double itol,const int maxiter,float * Init_Weight,float *Init_Mu,float* Init_Sigma)
{
	const int Data_num=Data_size;
	const int Dim_num=dim_num;
	const int class_num=class_number;
	int nstep=0;
	float * weight0=new float[class_num];
	float * weight1=new float[class_num];
	float ** mu0=new float*[Dim_num];
	for (int i=0;i<Dim_num;i++)
	{
		mu0[i]=new float[class_num];
	}
	float ** mu1=new float*[Dim_num];
	for (int i=0;i<Dim_num;i++)
	{
		mu1[i]=new float[class_num];
	}
	float***sigma0=new float**[class_num];
	for (int i=0;i<class_num;i++)
	{
		sigma0[i]=new float *[Dim_num];
	}
	for (int i=0;i<class_num;i++)
	{
		for (int j=0;j<Dim_num;j++)
		{
			sigma0[i][j]=new float[Dim_num];
		}
	}
	float***sigma1=new float**[class_num];
	for (int i=0;i<class_num;i++)
	{
		sigma1[i]=new float *[Dim_num];
	}
	for (int i=0;i<class_num;i++)
	{
		for (int j=0;j<Dim_num;j++)
		{
			sigma1[i][j]=new float[Dim_num];
		}
	}

	float ** Gaussian_value;
	float ** Beta_value;
	Gaussian_value=new float*[class_num];
	Beta_value=new float*[class_num];
	for (int i=0;i<class_num;i++)
	{
		Gaussian_value[i]=new float[Data_num];
		memset(Gaussian_value[i],0,Data_num*sizeof(float));
		Beta_value[i]= new float[Data_num];
		memset(Beta_value[i],0,Data_num*sizeof(float));
		weight0[i]=Init_Weight[i];
		weight1[i]=0;
		for (int j=0;j<Dim_num;j++)
		{
			mu0[j][i]=Init_Mu[j*class_num+i];
			mu1[j][i]=0;
			for (int k=0;k<Dim_num;k++)
			{
				sigma0[i][j][k]=Init_Sigma[i*Dim_num*Dim_num+j*Dim_num+k];
				sigma1[i][j][k]=0;
			}
		}
	}
	
	float sum_mu,sum_weight,sum_sigma;
	sum_mu=sum_sigma=sum_weight=0;
	for (int i=0;i<class_num;i++)
	{
		sum_weight+=abs(weight0[i]-weight1[i]);
		for (int j=0;j<Dim_num;j++)
		{
			sum_mu+=abs(mu0[j][i]-mu1[j][i]);
			for (int k=0;k<Dim_num;k++)
			{
				sum_sigma+=abs(sigma0[i][j][k]-sigma1[i][j][k]);
			}
		}

	}

	float *Mu_temp=new float[Dim_num];
	memset(Mu_temp,0,Dim_num*sizeof(float));
	float *X_temp=new float[Dim_num];
	memset(X_temp,0,Dim_num*sizeof(float));
	float **Sigma_temp=new float *[Dim_num];
	for (int i=0;i<Dim_num;i++)
	{
		Sigma_temp[i]=new float[Dim_num];
		memset(Sigma_temp[i],0,Dim_num*sizeof(float));
	}

	while (nstep<maxiter && sum_mu>itol && sum_weight >itol && sum_sigma>itol  && sum_mu+sum_sigma+sum_weight>itol)
	{
		nstep+=1;

		//计算Gaussian[i][j]
		for (int i=0;i<class_num;i++)
		{
			for (int j=0;j<Data_num;j++)
			{
				for (int k=0;k<Dim_num;k++)
				{
					X_temp[k]=Data[k][j];
					Mu_temp[k]=mu0[k][i];
					for (int q=0;q<Dim_num;q++)
					{
						Sigma_temp[k][q]=sigma0[i][k][q];
					}
				}
				Gaussian_value[i][j]=mvnpdf(X_temp,Mu_temp,Sigma_temp,Dim_num);		
			}
			
		}

		//计算Beta[i][j]
		float* sum_Betaj=new float[Data_num];
		memset(sum_Betaj,0,Data_num*sizeof(float));
		for (int i=0;i<Data_num;i++)
		{
			for (int j=0;j<class_num;j++)
			{
				sum_Betaj[i]+=weight0[j]*Gaussian_value[j][i];
			}
		}

		for (int i=0;i<class_num;i++)
		{
			for (int j=0;j<Data_num;j++)
			{
				Beta_value[i][j]=weight0[i]*Gaussian_value[i][j]/sum_Betaj[j];
			}
		}
		delete [] sum_Betaj;

		//计算权重，均值，标准差
		float* sum_beta_i=new float[class_num];memset(sum_beta_i,0,class_num*sizeof(float));
		float** sum_beta_ix=new float*[Dim_num];
		for (int i=0;i<Dim_num;i++)
		{
			sum_beta_ix[i]=new float[class_num];
			memset(sum_beta_ix[i],0,class_num*sizeof(float));
		}

		float*** sum_beta_ixu=new float**[class_num];
		for (int i=0;i<class_num;i++)
		{
			sum_beta_ixu[i]=new float*[Dim_num];
		}
		for (int i=0;i<class_num;i++)
		{
			for (int j=0;j<Dim_num;j++)
			{
				sum_beta_ixu[i][j]=new float[Dim_num];
				memset(sum_beta_ixu[i][j],0,Dim_num*sizeof(float));
			}
		}

		float** matrix_x_u=new float*[Dim_num];
		for (int i=0;i<Dim_num;i++)
		{
			matrix_x_u[i]=new float[Dim_num];
			memset(matrix_x_u[i],0,Dim_num*sizeof(float));
		}

		float* x_u_temp=new float[Dim_num];

		for (int i=0;i<class_num;i++)
		{
			for (int j=0;j<Data_num;j++)
			{
				sum_beta_i[i]+=Beta_value[i][j];
				for (int k=0;k<Dim_num;k++)
				{
					sum_beta_ix[k][i]+=Beta_value[i][j]*Data[k][j];
					x_u_temp[k]=Data[k][j]-mu0[k][i];	
				}
               
				for (int k=0;k<Dim_num;k++)
				{
					for (int q=0;q<Dim_num;q++)
					{
						matrix_x_u[k][q]=x_u_temp[k]*x_u_temp[q];
					}
				}
				for (int k=0;k<Dim_num;k++)
				{
					for (int q=0;q<Dim_num;q++)
					{
						sum_beta_ixu[i][k][q]+=Beta_value[i][j]*matrix_x_u[k][q];
					}
				}

			}

		}

        delete [] x_u_temp;
		for (int i=0;i<Dim_num;i++)
		{
			delete [] matrix_x_u[i];
		}
		delete [] matrix_x_u;


		for (int i=0;i<class_num;i++)
		{
			weight1[i]=sum_beta_i[i]/Data_num;
			for (int j=0;j<Dim_num;j++)
			{
				//mu1[j][i]=sum_beta_ix[j][i]/sum_beta_i[i];//不固定均值时，均值mu1参与迭代
				mu1[j][i]=mu0[j][i];//固定均值时
				for (int k=0;k<Dim_num;k++)
				{
					sigma1[i][j][k]=sum_beta_ixu[i][j][k]/sum_beta_i[i];
				}
			}
			
		}
		delete [] sum_beta_i;
		for (int i=0;i<Dim_num;i++)
		{
			delete [] sum_beta_ix[i];
		}
		delete [] sum_beta_ix;
		
		for (int i=0;i<class_num;i++)
		{
			for (int j=0;j<Dim_num;j++)
			{
				delete [] sum_beta_ixu[i][j];
			}
		}
		for (int i=0;i<class_num;i++)
		{
			delete []sum_beta_ixu[i];
		}
		delete [] sum_beta_ixu;

		//float sum_mu,sum_weight,sum_sigma;
		sum_mu=sum_sigma=sum_weight=0;
		for (int i=0;i<class_num;i++)
		{
			sum_weight+=abs(weight0[i]-weight1[i]);
			weight0[i]=weight1[i];
			for (int j=0;j<Dim_num;j++)
			{
				sum_mu+=abs(mu0[j][i]-mu1[j][i]);
				mu0[j][i]=mu1[j][i];
				for (int k=0;k<Dim_num;k++)
				{
					sum_sigma+=abs(sigma0[i][j][k]-sigma1[i][j][k]);
					sigma0[i][j][k]=sigma1[i][j][k];
				}
			}

		}

	}

	for (int i=0;i<class_num;i++)
	{
		this->Weight[i]=weight1[i];
		for (int j=0;j<Dim_num;j++)
		{
			this->Mu[j*class_num+i]=mu1[j][i];
			for (int k=0;k<Dim_num;k++)
			{
				this->Sigma[i*Dim_num*Dim_num+j*Dim_num+k]=sigma1[i][j][k];
			}
		}	 
	}


	delete [] Mu_temp;
	delete [] X_temp;
	for (int i=0;i<Dim_num;i++)
	{
		delete [] Sigma_temp[i];
	}
	delete [] Sigma_temp;

	delete [] weight0;
	delete [] weight1;

	for (int i=0;i<Dim_num;i++)
	{
		delete [] mu1[i];
		delete [] mu0[i];
	}
	delete [] mu0;
	delete [] mu1;

	for (int i=0;i<class_num;i++)
	{
		for (int j=0;j<Dim_num;j++)
		{
			delete [] sigma0[i][j];
			delete [] sigma1[i][j];
		}
	}
	delete [] sigma0;
	delete [] sigma1;


	for(int i=0;i<class_numb;i++)
	{
		delete [] Gaussian_value[i];
		delete [] Beta_value[i];
	}
	delete [] Gaussian_value;
	delete [] Beta_value;


}


//此函数用于根据各类估计的均值、权重、方差得参数计算kappa
//类别数为Image_kappa_evalu类中的成员变量class_numb
//各类的权重、均值、标准差都保存在Image_kappa_evalu类中的Mu，Sigma,Weight
//输出kappa保存在Image_kappa_evalu类中的kappa
void Image_kappa_evalu::kappa_evalu_singl()
{
	float* mu=new float[class_numb];
	float* sigma=new float[class_numb];
	float* weight=new float[class_numb];
	for (int i=0;i<class_numb;i++)
	{
		mu[i]=Mu[i];
		sigma[i]=Sigma[i];
		weight[i]=Weight[i];
	}
	sort_mu(mu,weight,sigma,class_numb);
	if (class_numb==2)
	{
		float h=(mu[0]+mu[1])/2;
		float p11=weight[0]*normcdf(h,mu[0],sigma[0]);
		float p22=weight[1]*(1-normcdf(h,mu[1],sigma[1]));
		float p1_other=weight[1]*normcdf(h,mu[1],sigma[1]);
		float p2_other=weight[0]*(1-normcdf(h,mu[0],sigma[0]));
		kappa=1-(p1_other+p2_other)/(1-weight[0]*(p11+p1_other)-weight[1]*(p22+p2_other));
	}
	else if (class_numb==3)
	{
		float h_1=(mu[0]+mu[1])/2;
		float h_2=(mu[1]+mu[2])/2;
		float p11=weight[0]*(normcdf(h_1,mu[0],sigma[0]));
		float p22=weight[1]*(normcdf(h_2,mu[1],sigma[1])-normcdf(h_1,mu[1],sigma[1]));
		float p33=weight[2]*(1-normcdf(h_2,mu[2],sigma[2]));
		float p1_other=weight[1]*normcdf(h_1,mu[1],sigma[1])+weight[2]*normcdf(h_1,mu[2],sigma[2]);       
		float p2_other=weight[0]*(normcdf(h_2,mu[0],sigma[0])-normcdf(h_1,mu[0],sigma[0]))+weight[2]*(normcdf(h_2,mu[2],sigma[2])-normcdf(h_1,mu[2],sigma[2]));
		float p3_other=weight[0]*(1-normcdf(h_2,mu[0],sigma[0]))+weight[1]*(1-normcdf(h_2,mu[1],sigma[1]));
		kappa=1-(p1_other+p2_other+p3_other)/(1-weight[0]*(p11+p1_other)-weight[1]*(p22+p2_other)-weight[2]*(p33+p3_other));
	}

	else if (class_numb==4)
	{
		float h1=(mu[0]+mu[1])/2;
	    float h2=(mu[1]+mu[2])/2;  
	    float h3=(mu[2]+mu[3])/2;  
	    float p11=weight[0]*normcdf(h1,mu[0],sigma[0]);
		float p22=weight[1]*(normcdf(h2,mu[1],sigma[1])-normcdf(h1,mu[1],sigma[1]));
	    float p33=weight[2]*(normcdf(h3,mu[2],sigma[2])-normcdf(h2,mu[2],sigma[2]));
	    float p44=weight[3]*(1-normcdf(h3,mu[3],sigma[3]));
		float p1_other=weight[1]*normcdf(h1,mu[1],sigma[1])+weight[2]*normcdf(h1,mu[2],sigma[2])+weight[3]*normcdf(h1,mu[3],sigma[3]);
	    float p2_other=weight[0]*(normcdf(h2,mu[0],sigma[0])-normcdf(h1,mu[0],sigma[0]))+weight[2]*(normcdf(h2,mu[2],sigma[2])-normcdf(h1,mu[2],sigma[2]))+\
		               weight[3]*(normcdf(h2,mu[3],sigma[3])-normcdf(h1,mu[3],sigma[3]));
		float p3_other=weight[0]*(normcdf(h3,mu[0],sigma[0])-normcdf(h2,mu[0],sigma[0]))+weight[1]*(normcdf(h3,mu[1],sigma[1])-normcdf(h2,mu[1],sigma[1]))+\
			           weight[3]*(normcdf(h3,mu[3],sigma[3])-normcdf(h2,mu[3],sigma[3]));
		float p4_other=weight[0]*(1-normcdf(h3,mu[0],sigma[0]))+weight[1]*(1-normcdf(h3,mu[1],sigma[1]))+weight[2]*(1-normcdf(h3,mu[2],sigma[2]));
	    kappa=1-(p1_other+p2_other+p3_other+p4_other)/(1-weight[0]*(p11+p1_other)-weight[1]*(p22+p2_other)-weight[2]*(p33+p3_other)-weight[3]*(p44+p4_other)); 

	}
	else 
		kappa=-2;	
	delete [] weight;
	delete [] mu;
	delete [] sigma;
}



//参考图部分//////////////////////////////////////////////////////////////////////////////////////////////////////
void Image_kappa_evalu::Init_Image_Ref(Image_Ref *p)
{
	ptoImage_Ref=p;
}

//最小距离分类部分//////////////////////////////////////////////////////////////////////////////////////////////////////

void Image_kappa_evalu::Init_labelofclassification()
{
	labelofClassification=new int[imgsize];
}


//Certer=[11 21 31;12 22 31]  11类别维度
void Image_kappa_evalu::NearestDistanceClassify(float** Data,const int Data_length,const int dim_size,const int class_num,float** Center )
{
	
	Init_labelofclassification();
	float** DistanceToCenter=new float*[class_num];
	for (int i=0;i<class_num;i++)
	{
		DistanceToCenter[i]=new float[Data_length];
		memset(DistanceToCenter[i],0,sizeof(float)*Data_length);
	}

	for (int i=0;i<class_num;i++)
	{
		for (int j=0;j<Data_length;j++)
		{   float sum=0;
			for (int k=0;k<dim_size;k++)
			{
				sum+=pow(Data[k][j]-Center[k][i],2);
			}
			DistanceToCenter[i][j]=sqrt(sum);
		}
	}
	
	for (int i=0;i<Data_length;i++)
	{   float min=pow(10.0,20);
	    int flag=-1;
		for (int j=0;j<class_num;j++)
		{
			if (DistanceToCenter[j][i]-min<-1e-7)
			{
				min=DistanceToCenter[j][i];
				flag=j;
			}
		}
		labelofClassification[i]=flag+1;

	}

	for (int i=0;i<class_num;i++)
	{
		delete [] DistanceToCenter[i];
	}
	delete [] DistanceToCenter;

}


void Image_kappa_evalu::KappaofReal_evalu()
{
	int* labelofref=ptoImage_Ref->Label_ref;//参考图标签
	int* labelofClassify=labelofClassification; //分类标签
	int Data_length=imgsize; //样本总数N
	int class_num=class_numb;

	int **ConfusionMatrix=new int *[class_num];
	for (int i=0;i<class_num;i++)
	{
		ConfusionMatrix[i]=new int[class_num];
	}
     for (int i=0;i<class_num;i++)
     {
		 for (int j=0;j<class_num;j++)
		 {
			 ConfusionMatrix[i][j]=0;
		 }
     }
	 int img_realsize=0;//这用于图中一部分进行统计时
	 for (int i=0;i<Data_length;i++)
	 {
		 int x=labelofref[i];
		 int y=labelofClassify[i];
		 if (x!=0)
		 {   img_realsize+=1;
			 ConfusionMatrix[x-1][y-1]+=1;
		 }
	 }

	 int*sum_row=new int[class_num];
	 int*sum_line=new int[class_num];
	 int sum_diagonal=0;
	 memset(sum_row,0,class_num*sizeof(int));
	 memset(sum_line,0,class_num*sizeof(int));
	 for (int i=0;i<class_num;i++)
	 {
		 sum_diagonal+=ConfusionMatrix[i][i];
		for (int j=0;j<class_num;j++)
		{
			sum_row[i]+=ConfusionMatrix[i][j];
			sum_line[i]+=ConfusionMatrix[j][i];    
		}
	 }

	 int sum_row_line=0;
	 for (int i=0;i<class_num;i++)
	 {
		 sum_row_line+=sum_row[i]*sum_line[i];
	 }
    Data_length=img_realsize;//这用于图中一部分进行统计时
	KappaofReal=float((Data_length*sum_diagonal-sum_row_line))/float((Data_length*Data_length-sum_row_line));
	 delete sum_row;
	 delete sum_line;

	for (int i=0;i<class_num;i++)
	{
		delete [] ConfusionMatrix[i];
	}
	delete ConfusionMatrix;
}

//提取纹理特征部分//////////////////////////////////////////////////////////////////////////////////////////////////////
 void Image_kappa_evalu::Init_TextureFeatures(const int window_size)   //保存的是去除窗口外围的纹理特征
 {
	 int realsize=(xlen-2*window_size)*(ylen-2*window_size);
	 TextureFeatures=new float *[5];
	 for (int i=0;i<5;i++)
	 {
		 TextureFeatures[i]=new float[realsize];
		 memset(TextureFeatures[i],0,realsize*sizeof(float));
	 }
 }


 //ImgBlock为输入，当前点为中心点的窗口，上下左右window_size大小内的数据
 //result_tex保存当前点为中心的窗口内计算的纹理特征，依次为能量、对比度、逆差矩、熵、相关
 //d为统计灰度共生矩阵的相邻点的距离
 //gray_scal为压缩的灰度级，默认为32
 void Image_kappa_evalu::ExtractTextureFeatures(float** ImgBlock,const int window_size,float* result_tex,const int d,const int gray_scal)
 {
	 int block_size=2*window_size+1;
	 int** Img_GrayCondense=new int* [block_size];//先将图像灰度级压缩
	 for (int i=0;i<block_size;i++)
	 {
		 Img_GrayCondense[i]=new int [block_size];
		 memset(Img_GrayCondense[i],0,block_size*sizeof(int));
	 }

	 int gap_length=256/gray_scal;
	 for (int i=0;i<block_size;i++)
	 {
		 for (int j=0;j<block_size;j++)
		 {
			 Img_GrayCondense[i][j]=(int)(floor(ImgBlock[i][j]/gap_length));
		 }
	 }

	 
//构建0度方向灰度共生矩阵，相邻点间隔为d
   float** GLCM_0=new float*[gray_scal];
   for (int i=0;i<gray_scal;i++)
   {
	   GLCM_0[i]=new float[gray_scal];
	   memset(GLCM_0[i],0,gray_scal*sizeof(float));
   }

   for (int i=0;i<block_size;i++)
   {
	   for (int j=0;j<block_size-d;j++)
	   {
		   int temp1=Img_GrayCondense[i][j];
		   int temp2=Img_GrayCondense[i][j+d];
		   GLCM_0[temp1][temp2]+=1;
		   GLCM_0[temp2][temp1]+=1;
	   }
   }
   int pair_num=2*block_size*(block_size-d);
   for (int i=0;i<gray_scal;i++)
   {
	   for (int j=0;j<gray_scal;j++)
	   {
		   GLCM_0[i][j]/=pair_num;//归一化为概率
	   }
   }

   float * Feature1=new float[5];//保存0度方向的纹理特征
   memset(Feature1,0,5*sizeof(float));
   caculate_texture(GLCM_0,gray_scal,Feature1);//计算纹理特征
   for (int i=0;i<gray_scal;i++)
   {
	   delete [] GLCM_0[i];
   }
   delete [] GLCM_0;

//构建45度方向灰度共生矩阵，相邻点间隔为d
   float** GLCM_45=new float*[gray_scal];
   for (int i=0;i<gray_scal;i++)
   {
	   GLCM_45[i]=new float[gray_scal];
	   memset(GLCM_45[i],0,gray_scal*sizeof(float));
   }

   for (int i=d;i<block_size;i++)
   {
	   for (int j=0;j<block_size-d;j++)
	   {
		   int temp1=Img_GrayCondense[i][j];
		   int temp2=Img_GrayCondense[i-d][j+d];
		   GLCM_45[temp1][temp2]+=1;
		   GLCM_45[temp2][temp1]+=1;
	   }
   }
   int pair_num45=2*(block_size-d)*(block_size-d);
   for (int i=0;i<gray_scal;i++)
   {
	   for (int j=0;j<gray_scal;j++)
	   {
		   GLCM_45[i][j]/=pair_num45;//归一化为概率
	   }
   }

   float * Feature2=new float[5];//保存45度方向的纹理特征
   memset(Feature2,0,5*sizeof(float));
   caculate_texture(GLCM_45,gray_scal,Feature2);//计算纹理特征

   for (int i=0;i<gray_scal;i++)
   {
	   delete [] GLCM_45[i];
   }
   delete [] GLCM_45;


//构建90度方向灰度共生矩阵，相邻点间隔为d
   float** GLCM_90=new float*[gray_scal];
   for (int i=0;i<gray_scal;i++)
   {
	   GLCM_90[i]=new float[gray_scal];
	   memset(GLCM_90[i],0,gray_scal*sizeof(float));
   }

   for (int i=0;i<block_size-d;i++)
   {
	   for (int j=0;j<block_size;j++)
	   {
		   int temp1=Img_GrayCondense[i][j];
		   int temp2=Img_GrayCondense[i+d][j];
		   GLCM_90[temp1][temp2]+=1;
		   GLCM_90[temp2][temp1]+=1;
	   }
   }
   int pair_num90=2*block_size*(block_size-d);
   for (int i=0;i<gray_scal;i++)
   {
	   for (int j=0;j<gray_scal;j++)
	   {
		   GLCM_90[i][j]/=pair_num90;//归一化为概率
	   }
   }

   float * Feature3=new float[5];//保存90度方向的纹理特征
   memset(Feature3,0,5*sizeof(float));
   caculate_texture(GLCM_90,gray_scal,Feature3);//计算纹理特征
   for (int i=0;i<gray_scal;i++)
   {
	   delete [] GLCM_90[i];
   }
   delete [] GLCM_90;


 //构建135度方向灰度共生矩阵，相邻点间隔为d
   float** GLCM_135=new float*[gray_scal];
   for (int i=0;i<gray_scal;i++)
   {
	   GLCM_135[i]=new float[gray_scal];
	   memset(GLCM_135[i],0,gray_scal*sizeof(float));
   }

   for (int i=d;i<block_size;i++)
   {
	   for (int j=d;j<block_size;j++)
	   {
		   int temp1=Img_GrayCondense[i][j];
		   int temp2=Img_GrayCondense[i-d][j-d];
		   GLCM_135[temp1][temp2]+=1;
		   GLCM_135[temp2][temp1]+=1;
	   }
   }
   int pair_num135=2*(block_size-d)*(block_size-d);
   for (int i=0;i<gray_scal;i++)
   {
	   for (int j=0;j<gray_scal;j++)
	   {
		   GLCM_135[i][j]/=pair_num135;//归一化为概率
	   }
   }

   float * Feature4=new float[5];//保存135度方向的纹理特征
   memset(Feature4,0,5*sizeof(float));
   caculate_texture(GLCM_135,gray_scal,Feature4);//计算纹理特征

   for (int i=0;i<gray_scal;i++)
   {
	   delete [] GLCM_135[i];
   }
   delete [] GLCM_135;

   for (int i=0;i<5;i++)
   {
	   result_tex[i]=(Feature1[i]+Feature2[i]+Feature3[i]+Feature4[i])/4;   
   }
   delete[] Feature1;
   delete[] Feature2;
   delete[] Feature3;
   delete[] Feature4;
	 for (int i=0;i<block_size;i++)
	 {
		 delete [] Img_GrayCondense[i];
	 }
	 delete [] Img_GrayCondense;

 }


 void Image_kappa_evalu:: caculate_texture(float ** GLCM,const int gray_scal,float* result)
 {
	 //能量
	 float Feature11=0;
	 //对比度
	 float Feature21=0;
	 //逆差距
	 float Feature31=0;
	 //熵
	 float Feature41=0;
	 //相关
	 float Feature51=0;
	 float ux,uy,sigx,sigy;
	 ux=uy=sigx=sigy=0;
	 for (int i=0;i<gray_scal;i++)
	 {
		 for (int j=0;j<gray_scal;j++)
		 {
			 Feature11+=pow(GLCM[i][j],2);
			 Feature21+=(i-j)*(i-j)*GLCM[i][j];
			 Feature31+=GLCM[i][j]/(1+(i-j)*(i-j));
			 if (abs(GLCM[i][j])>0.0000001)
			 {
				  Feature41-=GLCM[i][j]*log(GLCM[i][j])/log(2.0);
			 }
			 Feature51+=i*j*GLCM[i][j];
			 ux+=i*GLCM[i][j];
			 uy+=j*GLCM[i][j];
		 }
	 }

	 for (int i=0;i<gray_scal;i++)
	 {
		 for (int j=0;j<gray_scal;j++)
		 {
			 sigx+=pow((i-ux),2)*GLCM[i][j];
			 sigy+=pow((j-uy),2)*GLCM[i][j];
		 }
	 }
	 if (sigx*sigy!=0)
	 {
		 Feature51=(Feature51-ux*uy)/sqrt(sigx*sigy);
	 }
	 else
		 Feature51=1;

	 result[0]=Feature11;
	 result[1]=Feature21;
	 result[2]=Feature31;
	 result[3]=Feature41;
	 result[4]=Feature51;
 }

 void Image_kappa_evalu::GetImgTextureFeatures(const int window_size,const int d,const int gray_scal)
 {
	 Init_TextureFeatures(window_size);
	 float** Image=new float*[ylen];//开辟空间存储矩阵似的图像
	 for(int i=0;i<ylen;i++)
	{
			 Image[i]=new float[xlen];
			 memset(Image[i],0,xlen*sizeof(float));
	 }
	 for (int i=0;i<ylen;i++)
	 {
		 for(int j=0;j<xlen;j++)
		 {
			 Image[i][j]=Image_Gray[i*xlen+j];
		 }
	 }

	 int block_size=2*window_size+1;
	 float** block=new float*[block_size];
	 for (int i=0;i<block_size;i++)
	 {
		 block[i]=new float[block_size];
		 memset(block[i],0,block_size*sizeof(float));
	 }
	 float* result_tex=new float[5];
	 int count_index=-1;
	 for (int i=window_size;i<ylen-window_size;i++)
	 {
		 for (int j=window_size;j<xlen-window_size;j++)
		 {   count_index++;
			 for (int s=0;s<block_size;s++)
			 {
				 for (int q=0;q<block_size;q++)
				 {
					 block[s][q]=Image[i-window_size+s][j-window_size+q];
				 }
			 }
			 ExtractTextureFeatures(block,window_size,result_tex,d,gray_scal);
			 TextureFeatures[0][count_index]=result_tex[0];
			 TextureFeatures[1][count_index]=result_tex[1];
			 TextureFeatures[2][count_index]=result_tex[2];
			 TextureFeatures[3][count_index]=result_tex[3];
			 TextureFeatures[4][count_index]=result_tex[4];
		 }
	 }
	 delete [] result_tex;
	 for (int i=0;i<block_size;i++)
	 {
		 delete [] block[i];
	 }
	 delete [] block;

	 for(int i=0;i<ylen;i++)
		 delete [] Image[i];
	 delete [] Image;

	 //将纹理特征进行归一化
	for (int i=0;i<5;i++)
	 {
		 normlize(TextureFeatures[i],count_index+1,0.0,255.0);
	 } 

 }

 //多维Kappa估计部分
 void Image_kappa_evalu::NearestDistanceClassify(float** Data,const int Data_length,const int dim_size,const int class_num,float** Center,int* ResultofLabel)
 {
	 float** DistanceToCenter=new float*[class_num];
	 for (int i=0;i<class_num;i++)
	 {
		 DistanceToCenter[i]=new float[Data_length];
		 memset(DistanceToCenter[i],0,sizeof(float)*Data_length);
	 }

	 for (int i=0;i<class_num;i++)
	 {
		 for (int j=0;j<Data_length;j++)
		 {   float sum=0;
		    for (int k=0;k<dim_size;k++)
		    {
			    sum+=pow(Data[k][j]-Center[k][i],2);
		    }
		     DistanceToCenter[i][j]=sqrt(sum);
		 }
	 }

	 for (int i=0;i<Data_length;i++)
	 {   float min=pow(10.0,31)-1;
	 int flag=-1;
	 for (int j=0;j<class_num;j++)
	 {
		 if (DistanceToCenter[j][i]-min<-1e-7)
		 {
			 min=DistanceToCenter[j][i];
			 flag=j;
		 }
	 }
	 ResultofLabel[i]=flag+1;
	 }

	 for (int i=0;i<class_num;i++)
	 {
		 delete [] DistanceToCenter[i];
	 }
	 delete [] DistanceToCenter;
 }

  //多维特征并行使用最小距离准则下kappa运算
 //Classifyruleflag=0最小距离，Classifyruleflag=1，最小错误率
 void Image_kappa_evalu::kappa_evalu_multi(const int ClassifyRuleFlag,const float step,const float range_min,const float range_max)
 {
	 const int dim_size=dim_numb;
	 const int class_num=class_numb;
	 int classifyflag=ClassifyRuleFlag;

	 //先得到各类的参数
	 float* weight=new float[class_num];
	 float** mu=new float*[dim_size];
	 for (int i=0;i<dim_size;i++)
	 {
		 mu[i]=new float[class_num];
	 }
	 float***sigma=new float**[class_num];
	 for (int i=0;i<class_num;i++)
	 {
		 sigma[i]=new float*[dim_size];
	 }
	 for (int i=0;i<class_num;i++)
	 {
		 for (int j=0;j<dim_size;j++)
		 {
			 sigma[i][j]=new float[dim_size];
		 }
	 }
	 for (int i=0;i<class_num;i++)
	 {
		 weight[i]=Weight[i];
		 for (int j=0;j<dim_size;j++)
		 {
			 mu[j][i]=Mu[j*class_num+i];
			 for (int k=0;k<dim_size;k++)
			 {
				 sigma[i][j][k]=Sigma[i*dim_size*dim_size+j*dim_size+k];
			 }
		 }
	 }

    long int SizeofPointInOneDime=floor((abs(range_max)+abs(range_min))/step)+1;
	long int SizeofDatatemp=1;
	 for (int i=0;i<dim_size;i++)
	 {
		 SizeofDatatemp*=SizeofPointInOneDime;
	 }
	 float** Datatemp=new float*[dim_size];
	 for (int i=0;i<dim_size;i++)
	 {
		 Datatemp[i]=new float[SizeofDatatemp];
		 memset(Datatemp[i],0,sizeof(float)*SizeofDatatemp);
	 }

	 float* X_temp=new float[dim_size];
	 float* mu_temp=new float[dim_size];
	 float** sigma_temp=new float*[dim_size];
	 for (int i=0;i<dim_size;i++)
	 {
		 sigma_temp[i]=new float[dim_size];
	 }
	 int* ResultofLabel=new int[SizeofDatatemp];
	 memset(ResultofLabel,0,sizeof(int)*SizeofDatatemp);
	 float **Resultofmvnpdf=new float*[class_num];
	 for (int i=0;i<class_num;i++)
	 {
		 Resultofmvnpdf[i]=new float[SizeofDatatemp];
	 }
	 float** p=new float*[class_num];
	 for (int i=0;i<class_num;i++)
	 {
		 p[i]=new float[class_num];
		 memset(p[i],0,sizeof(float)*class_num);
	 }

	 //Kappa估计部分
	 if (dim_size==2)   //两维特征
	 {
		 float temp1=range_min;
		 float temp2=range_min;
		 for (int i=0;i<SizeofPointInOneDime;i++)
		 {
			 for (int j=0;j<SizeofPointInOneDime;j++)
			 {
			    Datatemp[0][i*SizeofPointInOneDime+j]=temp1;
				Datatemp[1][i*SizeofPointInOneDime+j]=temp2; 
				temp2+=step;
			 }
			 temp2=range_min;
			 temp1+=step;
		 }
		 if (classifyflag==0)//最小距离分类准则
		 {
			  NearestDistanceClassify(Datatemp,SizeofDatatemp,dim_size,class_num,mu,ResultofLabel);
		 }
		if ( classifyflag==1)//最小错误率分类准则
		{
			MinimumErroClassify_multi(Datatemp,SizeofDatatemp,dim_size,class_num,weight,mu,sigma,ResultofLabel);
		}
		 for (int i=0;i<class_num;i++)
		 {
			 for (int j=0;j<SizeofDatatemp;j++)
			 {
				 for (int k=0;k<dim_size;k++)
				 {
					 X_temp[k]=Datatemp[k][j];
					 mu_temp[k]=mu[k][i];
					 for (int q=0;q<dim_size;q++)
					 {
						 sigma_temp[k][q]=sigma[i][k][q];
					 }
				 }
				 Resultofmvnpdf[i][j]=step*step*mvnpdf(X_temp,mu_temp,sigma_temp,dim_size);	
				 int flag=ResultofLabel[j];
				 p[i][flag-1]+=Resultofmvnpdf[i][j];
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
			    
	 }

	 else if (dim_size==3)   //三维
	 {
		float temp1=range_min;
		float temp2=range_min;
		float temp3=range_min;
		for (int i=0;i<SizeofPointInOneDime;i++)
		{
			for (int j=0;j<SizeofPointInOneDime;j++)
			{
				for (int k=0;k<SizeofPointInOneDime;k++)
				{
					Datatemp[0][i*SizeofPointInOneDime*SizeofPointInOneDime+j*SizeofPointInOneDime+k]=temp1;
					Datatemp[1][i*SizeofPointInOneDime*SizeofPointInOneDime+j*SizeofPointInOneDime+k]=temp2;
					Datatemp[2][i*SizeofPointInOneDime*SizeofPointInOneDime+j*SizeofPointInOneDime+k]=temp3;
					temp3+=step;
				}
				temp3=range_min;
				temp2+=step;
			}
			temp2=range_min;
			temp1+=step;
		}

		if (classifyflag==0)//最小距离分类准则
		{
			NearestDistanceClassify(Datatemp,SizeofDatatemp,dim_size,class_num,mu,ResultofLabel);
		}
		if ( classifyflag==1)//最小错误率分类准则
		{
			MinimumErroClassify_multi(Datatemp,SizeofDatatemp,dim_size,class_num,weight,mu,sigma,ResultofLabel);
		}
	 for (int i=0;i<class_num;i++)
	 {
		 for (int j=0;j<SizeofDatatemp;j++)
		 {
			 for (int k=0;k<dim_size;k++)
			 {
				 X_temp[k]=Datatemp[k][j];
				 mu_temp[k]=mu[k][i];
				 for (int q=0;q<dim_size;q++)
				 {
					 sigma_temp[k][q]=sigma[i][k][q];
				 }
			 }
			 Resultofmvnpdf[i][j]=step*step*mvnpdf(X_temp,mu_temp,sigma_temp,dim_size);	
			 int flag=ResultofLabel[j];
			 p[i][flag-1]+=Resultofmvnpdf[i][j];
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
	 }
	
	
	 else      //四维以上暂时不考虑
	 {
		 kappa=-2;
	 }

	
	 for (int i=0;i<dim_size;i++)
	 {
		 delete [] Datatemp[i];
	 }
	 delete [] Datatemp;

	 for (int i=0;i<class_num;i++)
	 {
		 for (int j=0;j<dim_size;j++)
		 {
			 delete [] sigma[i][j];
		 }
	 }
	 for (int i=0;i<class_num;i++)
	 {
		 delete [] sigma[i];
	 }
	 delete [] sigma;

	 for (int i=0;i<dim_size;i++)
	 {
		delete[] mu[i];
	 }
	 delete [] mu;
	 delete [] weight;

	 delete [] ResultofLabel;
	 for (int i=0;i<dim_size;i++)
	 {
		 delete [] Resultofmvnpdf[i];
	 }
	 delete [] Resultofmvnpdf;
	 delete [] X_temp;
	 delete [] mu_temp;
	 for (int i=0;i<dim_size;i++)
	 {
		 delete[] sigma_temp[i];
	 }
	 delete [] sigma_temp;
	 for (int i=0;i<class_num;i++)
	 {
		delete [] p[i];
	 }
	 delete [] p;		
 }

 //DAG有向无环图形式kappa估计部分
 //输入为Data[dim_size][Data_size];class_num类别数目
 //Center[dim_size][class_num]为最小距离分类中各类的类中心
 //Stratg[class_num*(class_num-1)/2][2+dim_size]为有向无环图分类规则，二维数组形式存放，分类器的个数为class_num*(class_num-1)/2，
 //第1列和第2列表示当前分类器的分类类别，后dim_size列表示使用的特征，0表示不使用，1表示使用
 //举例：[(1,4,1,1);(1,3,0,1);(2,4,0,1);(1,2,1,1);(2,3,1,1);(3,4,1,1)]
 //目前只写到5类的情况，更多类暂时不考虑
 void Image_kappa_evalu::NearestDistanceClassify_DAG(float** Data,const int Data_size,const int dim_size,const int class_num,float** Center,int** Stratg,int* ResultofLabel)
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
		 for (int j=0;j<NumofFeatures;j++)
		 {
			 Mu_temp[j]=new float[2];
		 }

		 for (int j=0;j<2;j++)
		 {
			 for (int k=0;k<NumofFeatures;k++)
			 {
				 int numofF=indexofFeature[k];
				 int indexofclassifier=Stratg[i][j]-1;
				 Mu_temp[k][j]=Center[numofF][indexofclassifier];
			 }
		 }
		NearestDistanceClassify(Data_temp,Data_size,NumofFeatures,2,Mu_temp,ResultofAllClassifiers[i]);
	
		delete [] indexofFeature;

		for (int j=0;j<NumofFeatures;j++)
		{
			delete [] Mu_temp[j];
		}
		delete [] Mu_temp;

		for (int j=0;j<NumofFeatures;j++)
		{
			delete [] Data_temp[j];
		}
		delete [] Data_temp;
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


//此函数用于根据各类估计的权重、均值、协方差等参数计算DAG或者二叉树分类规则时的kappa,Classifyruleflag=0,最小距离 Classifyruleflag=1，最小错误率
//计算所用的各类参数在Image_kappa_evalu类中的Weight,Mu,Sigma
//多维特征DAG形式kappa计算或者二叉树形式，flag为0为DGA，flag为1为二叉树
//目前写到四类三维
//输出kappa保存在Image_kappa_evalu类中的kappa
void Image_kappa_evalu::kappa_evalu_multi_DAGOrBiTree(const int Classifyruleflag,int** Strategy,const int flag,const float step,const float range_min,const float range_max)
{
	const int dim_size=dim_numb;
	const int class_num=class_numb;

	//先得到各类的参数
	float* weight=new float[class_num];
	float** mu=new float*[dim_size];
	for (int i=0;i<dim_size;i++)
	{
		mu[i]=new float[class_num];
	}
	float***sigma=new float**[class_num];
	for (int i=0;i<class_num;i++)
	{
		sigma[i]=new float*[dim_size];
	}
	for (int i=0;i<class_num;i++)
	{
		for (int j=0;j<dim_size;j++)
		{
			sigma[i][j]=new float[dim_size];
		}
	}
	for (int i=0;i<class_num;i++)
	{
		weight[i]=Weight[i];
		for (int j=0;j<dim_size;j++)
		{
			mu[j][i]=Mu[j*class_num+i];
			for (int k=0;k<dim_size;k++)
			{
				sigma[i][j][k]=Sigma[i*dim_size*dim_size+j*dim_size+k];
			}
		}
	}

	long int SizeofPointInOneDime=floor((abs(range_max)+abs(range_min))/step)+1;
	long int SizeofDatatemp=1;
	for (int i=0;i<dim_size;i++)
	{
		SizeofDatatemp*=SizeofPointInOneDime;
	}
	float** Datatemp=new float*[dim_size];
	for (int i=0;i<dim_size;i++)
	{
		Datatemp[i]=new float[SizeofDatatemp];
		memset(Datatemp[i],0,sizeof(float)*SizeofDatatemp);
	}

	float* X_temp=new float[dim_size];
	float* mu_temp=new float[dim_size];
	float** sigma_temp=new float*[dim_size];
	for (int i=0;i<dim_size;i++)
	{
		sigma_temp[i]=new float[dim_size];
	}
	int* ResultofLabel=new int[SizeofDatatemp];
	memset(ResultofLabel,0,sizeof(int)*SizeofDatatemp);
	float **Resultofmvnpdf=new float*[class_num];
	for (int i=0;i<class_num;i++)
	{
		Resultofmvnpdf[i]=new float[SizeofDatatemp];
	}
	float** p=new float*[class_num];
	for (int i=0;i<class_num;i++)
	{
		p[i]=new float[class_num];
		memset(p[i],0,sizeof(float)*class_num);
	}

	//Kappa估计部分
	if (dim_size==2)   //两维特征
	{
		float temp1=range_min;
		float temp2=range_min;
		for (int i=0;i<SizeofPointInOneDime;i++)
		{
			for (int j=0;j<SizeofPointInOneDime;j++)
			{
				Datatemp[0][i*SizeofPointInOneDime+j]=temp1;
				Datatemp[1][i*SizeofPointInOneDime+j]=temp2; 
				temp2+=step;
			}
			temp2=range_min;
			temp1+=step;
		}

		if (Classifyruleflag==0 && flag==0)//基于最小距离的DAG
		{
		   NearestDistanceClassify_DAG(Datatemp,SizeofDatatemp,dim_size,class_num,mu,Strategy,ResultofLabel);
		}
		if (Classifyruleflag==0 && flag==1) //基于最小距离的二叉树
		{
		   NearestDistanceClassify_BinaryTree(Datatemp,SizeofDatatemp,dim_size,class_num,mu,Strategy,ResultofLabel);
		}
		if (Classifyruleflag==1 && flag==0) //基于最小错误率的DAG
		{
		   MinimumErroClassify_DAG(Datatemp,SizeofDatatemp,dim_size,class_num,weight,mu,sigma,Strategy,ResultofLabel);
		}
		if (Classifyruleflag==1 && flag==1)//基于最小错误率的二叉树暂时不考虑，以后需要再添加
		{

		}

		for (int i=0;i<class_num;i++)
		{
			for (int j=0;j<SizeofDatatemp;j++)
			{
				for (int k=0;k<dim_size;k++)
				{
					X_temp[k]=Datatemp[k][j];
					mu_temp[k]=mu[k][i];
					for (int q=0;q<dim_size;q++)
					{
						sigma_temp[k][q]=sigma[i][k][q];
					}
				}
				Resultofmvnpdf[i][j]=step*step*mvnpdf(X_temp,mu_temp,sigma_temp,dim_size);	
				int flag=ResultofLabel[j];
				p[i][flag-1]+=Resultofmvnpdf[i][j];
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

	}

	else if (dim_size==3)   //三维
	{
		float temp1=range_min;
		float temp2=range_min;
		float temp3=range_min;
		for (int i=0;i<SizeofPointInOneDime;i++)
		{
			for (int j=0;j<SizeofPointInOneDime;j++)
			{
				for (int k=0;k<SizeofPointInOneDime;k++)
				{
					Datatemp[0][i*SizeofPointInOneDime*SizeofPointInOneDime+j*SizeofPointInOneDime+k]=temp1;
					Datatemp[1][i*SizeofPointInOneDime*SizeofPointInOneDime+j*SizeofPointInOneDime+k]=temp2;
					Datatemp[2][i*SizeofPointInOneDime*SizeofPointInOneDime+j*SizeofPointInOneDime+k]=temp3;
					temp3+=step;
				}
				temp3=range_min;
				temp2+=step;
			}
			temp2=range_min;
			temp1+=step;
		}


		if (Classifyruleflag==0 && flag==0)//基于最小距离的DAG
		{
			NearestDistanceClassify_DAG(Datatemp,SizeofDatatemp,dim_size,class_num,mu,Strategy,ResultofLabel);
		}
		if (Classifyruleflag==0 && flag==1) //基于最小距离的二叉树
		{
			NearestDistanceClassify_BinaryTree(Datatemp,SizeofDatatemp,dim_size,class_num,mu,Strategy,ResultofLabel);
		}
		if (Classifyruleflag==1 && flag==0) //基于最小错误率的DAG
		{
			MinimumErroClassify_DAG(Datatemp,SizeofDatatemp,dim_size,class_num,weight,mu,sigma,Strategy,ResultofLabel);
		}
		if (Classifyruleflag==1 && flag==1)//基于最小错误率的二叉树暂时不考虑，以后需要再添加
		{

		}

		for (int i=0;i<class_num;i++)
		{
			for (int j=0;j<SizeofDatatemp;j++)
			{
				for (int k=0;k<dim_size;k++)
				{
					X_temp[k]=Datatemp[k][j];
					mu_temp[k]=mu[k][i];
					for (int q=0;q<dim_size;q++)
					{
						sigma_temp[k][q]=sigma[i][k][q];
					}
				}
				Resultofmvnpdf[i][j]=step*step*mvnpdf(X_temp,mu_temp,sigma_temp,dim_size);	
				int flag=ResultofLabel[j];
				p[i][flag-1]+=Resultofmvnpdf[i][j];
			}
		}
		if (class_num==2)//两类三维
		{
			float p11=weight[0]*p[0][0];
			float p12=weight[0]*p[0][1];
			float p21=weight[1]*p[1][0];
			float p22=weight[1]*p[1][1];
			kappa=1-(p12+p21)/(1-weight[0]*(p11+p21)-weight[1]*(p22+p12));
		}
		else if (class_num==3) //三类三维
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
		else if (class_num==4) //四类三维
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
	}


	else      //四维以上暂时不考虑
	{
		kappa=-2;
	}

	for (int i=0;i<dim_size;i++)
	{
		delete [] Datatemp[i];
	}
	delete [] Datatemp;

	for (int i=0;i<class_num;i++)
	{
		for (int j=0;j<dim_size;j++)
		{
			delete [] sigma[i][j];
		}
	}
	for (int i=0;i<class_num;i++)
	{
		delete [] sigma[i];
	}
	delete [] sigma;

	for (int i=0;i<dim_size;i++)
	{
		delete[] mu[i];
	}
	delete [] mu;
	delete [] weight;

	delete [] ResultofLabel;
	for (int i=0;i<dim_size;i++)
	{
		delete [] Resultofmvnpdf[i];
	}
	delete [] Resultofmvnpdf;
	delete [] X_temp;
	delete [] mu_temp;
	for (int i=0;i<dim_size;i++)
	{
		delete[] sigma_temp[i];
	}
	delete [] sigma_temp;
	for (int i=0;i<class_num;i++)
	{
		delete [] p[i];
	}
	delete [] p;
}


//二叉树形式多维特征kappa估计部分
//输入为Data[dim_size][Data_size];class_num类别数目
//Center[dim_size][class_num]为最小距离分类中各类的类中心
//Stratg[class_num-1][class_num+dim_size]为二叉树形式分类规则，二维数组形式存放，分类器的个数为（class_num-1）
//分类器中1，-1表示使用的类别，分别表示二叉树的左右枝，0表示不用
//特征使用规则为1表用，0表不用
//4类2维特征举例：[(1,1,1,-1, 1,1);(1,1,-1,0, 1,1)；(1,-1,0,0, 1,1)]
//目前只写到5类的情况，更多类暂时不考虑
//三类strategy规则：[1 1 -1;1 -1 0]
//四类规则[1 1 -1 -1;1 -1 0 0;0 0 1 -1]或者[1 1 1 -1;1 1 -1 0;1 -1 0 0] 1与-1的个数必须遵循此规律，但是1与-1的先后顺序可以变化，如1 -1 1 -1也行

void Image_kappa_evalu::NearestDistanceClassify_BinaryTree(float** Data,const int Data_size,const int dim_size,const int class_num,float** Center,int** Stratg,int* ResultofLabel)
{
	const int numofStratgline=class_num-1;//分类规则strategy的行，即分类器的个数
	const int numofStratgcolumn=class_num+dim_size;//分类规则strategy的列，即特征的个数加类别的个数
	int** ResultofAllClassifiers=new int*[numofStratgline];
	int** NumofLeftRightIneachClassifier=new int*[numofStratgline];//记录每个分类器中1，-1的个数
	for (int i=0;i<numofStratgline;i++)
	{
		ResultofAllClassifiers[i]=new int[Data_size];
		memset(ResultofAllClassifiers[i],0,sizeof(int)*Data_size);
		NumofLeftRightIneachClassifier[i]=new int[2];
		memset(NumofLeftRightIneachClassifier[i],0,2*sizeof(int));
	}
	//得到所有样本进行所有分类器的分类结果
	for (int i=0;i<numofStratgline;i++)
	{
		//统计每个分类器所使用的特征个数
		int NumofFeatures=0;
		for (int j=class_num;j<numofStratgcolumn;j++)//第class_num位后存放特征使用情况
		{
			if (Stratg[i][j]==1)
			{
				NumofFeatures++;
			}
		}
		//记录使用特征的标号
		int * indexofFeature=new int[NumofFeatures];
		int sub=0;
		for (int k=class_num;k<numofStratgcolumn;k++)
		{
			if (Stratg[i][k]==1)
			{
				indexofFeature[sub]=k-class_num;
				sub++;
			}
		}
		float** Data_temp=new float*[NumofFeatures];
		for (int j=0;j<NumofFeatures;j++)
		{
			Data_temp[j]=new float[Data_size];
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
		for (int j=0;j<NumofFeatures;j++)
		{
			Mu_temp[j]=new float[class_num];
		}

		for (int j=0;j<class_num;j++)
		{
			for (int k=0;k<NumofFeatures;k++)
			{
				int numofF=indexofFeature[k];
				Mu_temp[k][j]=Center[numofF][j];
			}
		}
		//统计当前分类器分类类别的情况，即1与-1的个数
		int NumofleftInClassifier=0;//1的个数
		int NumofRightInClassifier=0;//-1的个数
		for (int j=0;j<class_num;j++)
		{
			if (Stratg[i][j]==1)
			{
				NumofleftInClassifier++;
			}
			if (Stratg[i][j]==-1)
			{
				NumofRightInClassifier++;
			}
		}
		NumofLeftRightIneachClassifier[i][0]=NumofleftInClassifier;//记录每个分类器 1 -1的个数
		NumofLeftRightIneachClassifier[i][1]=NumofRightInClassifier;
		int * indexofLeft=new int[NumofleftInClassifier];
		int * indexofRight=new int[NumofRightInClassifier];
		int sub1=0;
		int sub2=0;
		for (int k=0;k<class_num;k++)
		{
			if (Stratg[i][k]==1)
			{
				indexofLeft[sub1]=k;
				sub1++;
			}
			if (Stratg[i][k]==-1)
			{
				indexofRight[sub2]=k;
				sub2++;
			}
		}
		float** MuofTwoBranch=new float*[NumofFeatures];
		for (int j=0;j<NumofFeatures;j++)
		{
			MuofTwoBranch[j]=new float[2];
			memset(MuofTwoBranch[j],0,2*sizeof(float));
		}
		for (int j=0;j<NumofFeatures;j++)
		{
			for (int k=0;k<NumofleftInClassifier;k++)
			{
				int indexL=indexofLeft[k];
				MuofTwoBranch[j][0]+=Mu_temp[j][indexL];
			}
			for (int k=0;k<NumofRightInClassifier;k++)
			{
				int indexR=indexofRight[k];
				MuofTwoBranch[j][1]+=Mu_temp[j][indexR];
			}
		}
		for (int j=0;j<NumofFeatures;j++) //求标号为1的所有类别的类中心
		{
			MuofTwoBranch[j][0]/=NumofleftInClassifier;
			MuofTwoBranch[j][1]/=NumofRightInClassifier;
		}
		NearestDistanceClassify(Data_temp,Data_size,NumofFeatures,2,MuofTwoBranch,ResultofAllClassifiers[i]);

		for (int j=0;j<NumofFeatures;j++)
		{
			delete [] MuofTwoBranch[j];
		}
		delete [] MuofTwoBranch;
		delete [] indexofLeft;
		delete [] indexofRight;
		delete [] indexofFeature;
		for (int j=0;j<NumofFeatures;j++)
		{
			delete [] Mu_temp[j];
		}
		delete [] Mu_temp;
		for (int j=0;j<NumofFeatures;j++)
		{
			delete [] Data_temp[j];
		}
		delete [] Data_temp;
	}

	//根据各分类器的结果判定最终分类标签号
	
	if (class_num==3)
	{
		for (int j=0;j<Data_size;j++)
		{
			if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==1)
			{   int label=0;
				for (int i=0;i<class_num;i++)
				{
					if (Stratg[0][i]==1 && Stratg[1][i]==1)
					{
						label=i+1;
					}
				}
				ResultofLabel[j]=label;
			}
			else if (ResultofAllClassifiers[0][j]==2)
			{
				int label=0;
				for (int i=0;i<class_num;i++)
				{
					if (Stratg[0][i]==-1)
					{
						label=i+1;
					}
				}
				ResultofLabel[j]=label;
			}
			else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==2)
			{
				int label=0;
				for (int i=0;i<class_num;i++)
				{
					if (Stratg[0][i]==1 && Stratg[1][i]==-1)
					{
						label=i+1;
					}
				}
				ResultofLabel[j]=label;
				
			}
			else
			{

			}

		}
	}

	else if (class_num==4)
	{
		
		if (NumofLeftRightIneachClassifier[0][1]==2)//第一个分类器为1,1，-1，-1的情况
		{
			for (int j=0;j<Data_size;j++)
			{
				if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==1)
				{  
					int label=0;
				    for (int i=0;i<class_num;i++)
				    {
					   if (Stratg[0][i]==-1 && Stratg[2][i]==1)
					   {
						  label=i+1;
					   }
				     }
				     ResultofLabel[j]=label;
				}
				else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[2][j]==2)
				{   
					int label=0;
					for (int i=0;i<class_num;i++)
					{
						if (Stratg[0][i]==-1 && Stratg[2][i]==-1)
						{
							label=i+1;
						}
					}
					ResultofLabel[j]=label;	
				}
				else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==2)
				{   
					int label=0;
					for (int i=0;i<class_num;i++)
					{
						if (Stratg[0][i]==1 && Stratg[1][i]==-1)
						{
							label=i+1;
						}
					}
					ResultofLabel[j]=label;
				}
				else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==1)
				{   
					int label=0;
					for (int i=0;i<class_num;i++)
					{
						if (Stratg[0][i]==1 &&  Stratg[1][i]==1 )
						{
							label=i+1;
						}
					}
					ResultofLabel[j]=label;
				}
				else
				{

				}
	
			}
		}
		if (NumofLeftRightIneachClassifier[0][1]==1)//第一个分类器为1,1,1，-1的情况
		{
			for (int j=0;j<Data_size;j++)
			{
				if (ResultofAllClassifiers[0][j]==2 )
				{   
					int label=0;
				    for (int i=0;i<class_num;i++)
				    {
					   if (Stratg[0][i]==-1)
					   {
						   label=i+1;
					   }
				     }
				    ResultofLabel[j]=label;
				}
				else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==2)
				{   
					int label=0;
					for (int i=0;i<class_num;i++)
					{
						if (Stratg[0][i]==1 && Stratg[1][i]==-1)
						{
							label=i+1;
						}
					}
					ResultofLabel[j]=label;	
				}
				else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==1 && ResultofAllClassifiers[2][j]==2)
				{   
					int label=0;
					for (int i=0;i<class_num;i++)
					{
						if (Stratg[0][i]==1 && Stratg[1][i]==1 && Stratg[2][i]==-1)
						{
							label=i+1;
						}
					}
					ResultofLabel[j]=label;
				}
				else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==1 && ResultofAllClassifiers[2][j]==1)
				{   
					int label=0;
					for (int i=0;i<class_num;i++)
					{
						if (Stratg[0][i]==1 && Stratg[1][i]==1 && Stratg[2][i]==1 )
						{
							label=i+1;
						}
					}
					ResultofLabel[j]=label;
				}
				else
				{

				}

			}


		}
	}
	else if (class_num==5)
	{
		if (NumofLeftRightIneachClassifier[0][1]==2)
		{
			for (int j=0;j<Data_size;j++)
			{
				if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[3][j]==2)
				{   int label=0;
				for (int i=0;i<class_num;i++)
				{
					if (Stratg[0][i]==-1 && Stratg[3][i]==-1)
					{
						label=i+1;
					}
				}
				ResultofLabel[j]=label;
				}
				else if (ResultofAllClassifiers[0][j]==2  && ResultofAllClassifiers[3][j]==1)
				{   
					int label=0;
					for (int i=0;i<class_num;i++)
					{
						if (Stratg[0][i]==-1 && Stratg[3][i]==1)
						{
							label=i+1;
						}
					}
					ResultofLabel[j]=label;	
				}
				else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==2)
				{   
					int label=0;
					for (int i=0;i<class_num;i++)
					{
						if (Stratg[0][i]==1 && Stratg[1][i]==-1)
						{
							label=i+1;
						}
					}
					ResultofLabel[j]=label;
				}
				else if (ResultofAllClassifiers[0][j]==1 && ResultofAllClassifiers[1][j]==1 && ResultofAllClassifiers[2][j]==2)
				{   
					int label=0;
					for (int i=0;i<class_num;i++)
					{
						if (Stratg[0][i]==1 && Stratg[1][i]==1 && Stratg[2][i]==-1)
						{
							label=i+1;
						}
					}
					ResultofLabel[j]=label;

				}
				else if (ResultofAllClassifiers[0][j]==1 && ResultofAllClassifiers[1][j]==1 && ResultofAllClassifiers[2][j]==1)
				{   
					int label=0;
					for (int i=0;i<class_num;i++)
					{
						if (Stratg[0][i]==1 && Stratg[1][i]==1 && Stratg[2][i]==1)
						{
							label=i+1;
						}
					}
					ResultofLabel[j]=label;

				}
				else
				{

				}

			}
		}
		if (NumofLeftRightIneachClassifier[0][1]==1)
		{
			for (int j=0;j<Data_size;j++)
			{
				if (ResultofAllClassifiers[0][j]==2 )
				{   int label=0;
				for (int i=0;i<class_num;i++)
				{
					if (Stratg[0][i]==-1)
					{
						label=i+1;
					}
				}
				ResultofLabel[j]=label;
				}
				else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==2)
				{   
					int label=0;
					for (int i=0;i<class_num;i++)
					{
						if (Stratg[0][i]==1 && Stratg[1][i]==-1)
						{
							label=i+1;
						}
					}
					ResultofLabel[j]=label;	
				}
				else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==1 && ResultofAllClassifiers[2][j]==2)
				{   
					int label=0;
					for (int i=0;i<class_num;i++)
					{
						if (Stratg[0][i]==1 && Stratg[1][i]==1 && Stratg[2][i]==-1)
						{
							label=i+1;
						}
					}
					ResultofLabel[j]=label;
				}
				else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==1 && ResultofAllClassifiers[2][j]==1 && ResultofAllClassifiers[3][j]==2 )
				{   
					int label=0;
					for (int i=0;i<class_num;i++)
					{
						if (Stratg[0][i]==1 && Stratg[1][i]==1 && Stratg[2][i]==1  && Stratg[3][i]==-1)
						{
							label=i+1;
						}
					}
					ResultofLabel[j]=label;
				}
				else if (ResultofAllClassifiers[0][j]==1  && ResultofAllClassifiers[1][j]==1 && ResultofAllClassifiers[2][j]==1 && ResultofAllClassifiers[3][j]==1 )
				{   
					int label=0;
					for (int i=0;i<class_num;i++)
					{
						if (Stratg[0][i]==1 && Stratg[1][i]==1 && Stratg[2][i]==1  && Stratg[3][i]==1)
						{
							label=i+1;
						}
					}
					ResultofLabel[j]=label;
				}
				else
				{

				}

			}
	     }
		}
		else
		{
			
        }
		for (int i=0;i<numofStratgline;i++)
		{
			delete [] ResultofAllClassifiers[i]; 
			delete [] NumofLeftRightIneachClassifier[i];
		}
		delete [] ResultofAllClassifiers;
		delete [] NumofLeftRightIneachClassifier;
}
Image_kappa_evalu* Image_kappa_evalu::image = NULL;