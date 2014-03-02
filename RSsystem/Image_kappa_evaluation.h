#pragma once
#include "gdal_priv.h"
#include "auxiliary_function.h"
#include "Image_Ref.h"
#include "resource.h"
#include "Canshuguji.h"
#include <math.h>
#include <algorithm>


//������Ľṹ��
typedef struct Point
{
	int x;
	int y;
}Point;
//���ͼ������Ľṹ��
typedef struct Regin
{
	Point topleft;
	Point bottomright;
}Regin;

//����kappa���Ƶ�ͼ����
class Image_kappa_evalu
{
public:
	int xlen; //ͼ���
	int ylen; //��
	int zlen; //������
	int imgsize;//��*�ߣ�ͼ���С

	int band_selec;//ѡ��Ĳ���
	int class_numb;//�����
	int dim_numb;//ά��

	float kappa; //����kappa
	float * AllKappa; //�м�����R)
	long int SizeofAllKappa;//(R)

	float * Image_Gray; //�Ҷ�����
	float ** TextureFeatures;//�����������������Աȶȡ����ء��ء����

	float * Weight;//�����Ȩ��  Weight[class_numb]
	float * Mu;//����ľ�ֵ  Mu[class_numb*dim_numb] Mu[11 21 31 12 22 32]
	float * Sigma;//����ı�׼�� Sigma[class_numb*dim_numb*dim_numb] Sigma[111 112 121 122   211 212 221 222 311 312 321 322 ]

	Canshuguji *pro;//���������ƴ��ڵ�ַ��ָ��

    // �ο�ͼ����
	Image_Ref * ptoImage_Ref;

	//��С������ಿ��
	int * labelofClassification;
	float KappaofReal;

	//�ж��Ƿ��Ѿ�����ʵ����
	static Image_kappa_evalu* image;
private:
	Image_kappa_evalu();
public:
	static Image_kappa_evalu* getInstance() {
		if (image == NULL) {
			image = new Image_kappa_evalu();
		}
		return image;
	}
	virtual ~Image_kappa_evalu();

	//ͼ���ʼ������
	void init_GrayImg(CString str,int band_sec);//band_secΪѡ��Ĳ��κ�
	void init_Parameter(int class_num,int dim_num );//class_numΪ�������dim_numΪά��

	//��ȡ������������
	void GetImgTextureFeatures(const int window_size,const int d=1,const int gray_scal=32);


	//Gmm���Ʋ���
	void GMM_multi(float** Data,const int Data_size,const int class_num,const int dim_num,const double itol,const int maxiter,float * Init_Weight,float *Init_Mu,float* Init_Sigma);
	void GMM_singl(float* Data,const int Data_size,const int class_num,const double itol,const int maxiter,float * Init_Weight, float *Init_Mu,float* Init_Sigma);
	void kappa_evalu_singl();//��ά����kappa���㣬������С����
	void kappa_evalu_singl_minimumerro();//��ά����kappa���㣬������С������
    void kappa_evalu_multi(const int Classifyruleflag,const float step=1.0,const float range_min=-30,const float range_max=280);//��ά��������kappa����,Classifyruleflag=0��ʾ��С���룬Classifyruleflag=1��ʾ��С������
    void kappa_evalu_multi_DAGOrBiTree(const int Classifyruleflag,int** Strategy,const int flag,const float step=1.0,const float range_min=-30,const float range_max=280);//��ά����DAG��ʽkappa������߶�������ʽ��flagΪ0ΪDGA��flagΪ1Ϊ��������Classifyruleflag=0��ʾ��С���룬Classifyruleflag=1��ʾ��С������
	


	//�̶���ֵ���������ڱ�����ֵ�õ�GMM���ƽ����Ѱ��������kappa
	void FindHighestFrequencyKappa_singl(const int ClassifyRuleflag,float* Data,const int Data_size,float * Init_Weight, float *Init_Mu,float* Init_Sigma,float *IteratRegion,const float step=1.0);//Classifyruleflag=0��ʾ��С���룬Classifyruleflag=1��ʾ��С������
	void FindHighestFrequencyKappa_multi(const int ClassifyRuleflag,float** Data,const int Data_size,float * Init_Weight, float *Init_Mu,float* Init_Sigma,float *IteratRegion,const float step=1.0,const float kappaevalu_step=1.0);//Classifyruleflag=0��ʾ��С���룬Classifyruleflag=1��ʾ��С������
	void FindHighestFrequencyKappa_DAGOrBiTree(const int Classifyruleflag,int** Strategy,const int flag,float** Data,const int Data_size,float * Init_Weight, float *Init_Mu,float* Init_Sigma,float *IteratRegion,const float step=1.0,const float kappaevalu_step=1.0);//������С����

	//�ο�ͼ��ʼ������
	void Init_Image_Ref(Image_Ref *);
	
	//��С������ಿ��
	void NearestDistanceClassify(float** Data,const int Data_size,const int dim_size,const int class_num,float** Center);
	void KappaofReal_evalu();//��С���������kappa����

	void Init_TextureFeatures(const int window_size);//��ʱ��Ϊpublic


private:  //�ڲ����ú���
	//��С������ಿ��
	void Init_labelofclassification();

	//��ȡ������������ �ڲ����ú���
	//void Init_TextureFeatures(const int window_size);

	void ExtractTextureFeatures(float** ImgBlock,const int window_size,float* result_tex,const int d=1,const int gray_scal=32);
	void caculate_texture(float ** GLCM,const int gray_scal,float* result);//ExtractTextureFeatures�е��õ��Ӻ���

	//��άGMM���㲿��
	void NearestDistanceClassify(float** Data,const int Data_size,const int dim_size,const int class_num,float** Center,int* ResultofLabel);//Kappa�����е��ú���
	//DAG�����޻�ͼ
	void NearestDistanceClassify_DAG(float** Data,const int Data_size,const int dim_size,const int class_num,float** Center,int** Stratg,int* ResultofLabel);//DAG��kappa������ú���,������С����
	
	//��������ʽ��С�������
	void NearestDistanceClassify_BinaryTree(float** Data,const int Data_size,const int dim_size,const int class_num,float** Center,int** Stratg,int* ResultofLabel);//��������ʽ��kappa������ú���
	
	//Ѱ�������г��������Ǹ���ֵ
	float HighestFrequenceInArray(float * Array,const long int Array_size,const float interval=0.01);//���̶���ֵ�����õ�һϵ��kappa��Ѱ��������kappaʱ���õĺ���

	//��С�����ʷ��ಿ��
	void MinimumErroClassify_singl(float* Data,const int Data_size,const int class_num,float *weight,float* Center,float *sigma,int* ResultofLabel);//������С������ʱKappa�����е��ú���
	void MinimumErroClassify_multi(float** Data,const int Data_size,const int dim_size,const int class_num,float *weight,float** Center,float ***sigma,int* ResultofLabel);//������С������ʱKappa�����е��ú���
	void MinimumErroClassify_DAG(float** Data,const int Data_size,const int dim_size,const int class_num,float *weight,float** Center,float ***sigma,int** Stratg,int* ResultofLabel);//DAG��kappa������ú���,������С������
};
