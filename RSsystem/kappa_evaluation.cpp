// kappa_evaluation.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Image_kappa_evaluation.h"
#include <iostream>
using namespace std;

int main()
{ 

	/*Image_kappa_evalu MyImg;
    GDALDataset * DataSet;          // ͼ���ļ�  
    GDALAllRegister();              //ע������ 
   //DataSet =(GDALDataset *)GDALOpen("C:\\F55_85_135.BMP",GA_ReadOnly);//�ļ��Ĵ�ʹ�õ���GDALOpen����
	DataSet =(GDALDataset *)GDALOpen("C:\\Kob_Germany_Raipeye123band.BMP",GA_ReadOnly);
	if (DataSet == NULL)  
    {  
        cout<<"cant open Image";  
    }  
	MyImg.init_GrayImg(DataSet,3);//�õ��Ҷ�ͼ��
	MyImg.init_Parameter(3,2);//��������������ά��
	/*float* gray=MyImg.Image_Gray;
	double itol=0.00001;
	int maxiter=10000;
	float Init_Weight[3]={0.3,0.2,0.5};
	float Init_Mu[3]={15,50,139};
	float Init_Sigma[3]={7,23,27};
	MyImg.GMM_singl(MyImg.Image_Gray,MyImg.imgsize,3,itol,maxiter,Init_Weight,Init_Mu,Init_Sigma);//GMM����
	MyImg.kappa_evalu_singl();//Kappa����
	cout<<"kappa="<<MyImg.kappa<<endl;

	//����ο�ͼ���Բ���
	GDALDataset * DataSet_ref;                    
	DataSet_ref =(GDALDataset *)GDALOpen("C:\\Fref.BMP",GA_ReadOnly);
	Image_Ref A;
	Image_Ref * p=&A;
	p->init_Image_Ref(DataSet_ref);
	ColortoLabel colorbox[3]={{143,195,31,3},{126,206,244,1},{0,113,48,2}};
	p->SetColorBox(colorbox,3);
	p->GetLabel();
	int *label_ref=p->Label_ref;
	MyImg.Init_Image_Ref(p);
	
	//��С���������Բ���
	float **Center=new float *[1];
	*Center=new float[3];
	 Center[0][0]=14.1747;Center[0][1]=50.5833;Center[0][2]=139.6729;
	
	float ** Data=new float *[1];
	*Data=MyImg.Image_Gray;
	MyImg.NearestDistanceClassify(Data,MyImg.imgsize,MyImg.dim_numb,MyImg.class_numb,Center);
	int *label_cla=MyImg.labelofClassification;

	MyImg.KappaofReal_evalu();
	cout<<"MyImg.KappaofReal="<<MyImg.KappaofReal;*/
	


	//��ȡ������Բ���  ����������ȷ
	/*int window_size=10;
	MyImg.GetImgTextureFeatures(window_size);
	int real_size=(MyImg.xlen-2*window_size)*(MyImg.ylen-2*window_size);
	char *path="d:\\Cjieguo\\TextureFeatures.txt";
	FILE*fp;
    if((fp=fopen(path,"a+"))!=NULL)
	{
		for(int index1=0;index1<real_size;index1++)
			for(int index2=0;index2<5;index2++)
				fprintf(fp,"%f ",MyImg.TextureFeatures[index2][index1]);
		fclose(fp);
	}*/


	/*int window_size=10;
	MyImg.Init_TextureFeatures(10);
	int real_size=(MyImg.xlen-2*window_size)*(MyImg.ylen-2*window_size);
	
	char *path="d:\\Cjieguo\\TextureFeatures.txt";
	FILE*fp;
	if((fp=fopen(path,"r+"))!=NULL)
	{
		for(int index1=0;index1<real_size;index1++)
			for(int index2=0;index2<5;index2++)
				fscanf(fp,"%f ",&(MyImg.TextureFeatures[index2][index1]));
		fclose(fp);
	}*/

/*	//��άGMM�㷨���Բ���
	double itol=0.00001;
	int maxiter=10000;
	int dim_num=2;
	int class_num=3;
	float Init_Weight[3]={0.3333,0.3333,0.3333};
	float Init_Mu[6]={144.2,205.7,36.5,156.3,54.6,232.3};
	float Init_Sigma[12]={931.9965,-896.2,-896.2,965.8, 446.4,-471.8,-471.8,548.3, 456.8,-309.7,-309.7,245.5};
	float**Data=new float*[2];
	Data[0]=MyImg.TextureFeatures[2];
	Data[1]=MyImg.TextureFeatures[3];
	//MyImg.GMM_multi(Data,real_size,class_num,dim_num,itol,maxiter,Init_Weight,Init_Mu,Init_Sigma);
	//MyImg.kappa_evalu_multi(1,1.0,-50.0,300.0);
	float * IteratRegion=new float[12];
	IteratRegion[0]=-1,IteratRegion[1]=1,IteratRegion[2]=-1,IteratRegion[3]=1,IteratRegion[4]=-0,IteratRegion[5]=0;
	IteratRegion[6]=0,IteratRegion[7]=0,IteratRegion[8]=0,IteratRegion[9]=0,IteratRegion[10]=0,IteratRegion[11]=0;
	//MyImg.FindHighestFrequencyKappa_multi(0,Data,real_size,Init_Weight, Init_Mu,Init_Sigma,IteratRegion);//������ȷ
	
	
	
	//DAG���Բ���
	//Strategy[3][4]={{1,3,1,1},{1,2,1,1},{2,3,1,1}};
	
	int **Strategy=new int *[3];
	for (int i=0;i<3;i++)
	{
	Strategy[i]=new int[4];
	}
	Strategy[0][0]=1;Strategy[0][1]=3;Strategy[0][2]=1;Strategy[0][3]=1;
	Strategy[1][0]=1;Strategy[1][1]=2;Strategy[1][2]=1;Strategy[1][3]=1;
	Strategy[2][0]=2;Strategy[2][1]=3;Strategy[2][2]=1;Strategy[2][3]=1;
	//MyImg.kappa_evalu_multi_DAGOrBiTree(1,Strategy,0,1.0,-50.0,300.0);
	MyImg.FindHighestFrequencyKappa_DAGOrBiTree(0,Strategy,0,Data,real_size,Init_Weight, Init_Mu,Init_Sigma,IteratRegion);

     //���������Բ���
	//Strategy[2][5]={{1,1,-1,1,1},{1,-1,0,1,1}};
	/*int **Strategy=new int *[2];
	for (int i=0;i<2;i++)
	{
		Strategy[i]=new int[5];
	}
	Strategy[0][0]=1;Strategy[0][1]=1;Strategy[0][2]=-1;Strategy[0][3]=1;Strategy[0][4]=1;
	Strategy[1][0]=1;Strategy[1][1]=-1;Strategy[1][2]=0;Strategy[1][3]=1;Strategy[1][4]=1;
	MyImg.kappa_evalu_multi_DAGOrBiTree(0,Strategy,1,1.0,-50.0,300.0);*/
	/*for (int i=0;i<2;i++)
	{
		delete [] Strategy[i];
	}
	delete [] Strategy;*/


	/*delete [] Data[0];
	delete [] Data[1];
	delete [] Data;
	/*delete [] Data[0];
	delete [] Data;
	delete [] Center[0];
	delete [] Center;*/
	//GDALClose(DataSet);
	
	return 0;
}

