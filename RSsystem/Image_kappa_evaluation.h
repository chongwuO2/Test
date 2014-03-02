#pragma once
#include "gdal_priv.h"
#include "auxiliary_function.h"
#include "Image_Ref.h"
#include "resource.h"
#include "Canshuguji.h"
#include <math.h>
#include <algorithm>


//点坐标的结构体
typedef struct Point
{
	int x;
	int y;
}Point;
//表达图像区域的结构体
typedef struct Regin
{
	Point topleft;
	Point bottomright;
}Regin;

//进行kappa估计的图像类
class Image_kappa_evalu
{
public:
	int xlen; //图像宽
	int ylen; //高
	int zlen; //波段数
	int imgsize;//宽*高，图像大小

	int band_selec;//选择的波段
	int class_numb;//类别数
	int dim_numb;//维数

	float kappa; //估计kappa
	float * AllKappa; //中间结果（R)
	long int SizeofAllKappa;//(R)

	float * Image_Gray; //灰度特征
	float ** TextureFeatures;//纹理特征；能量、对比度、逆差矩、熵、相关

	float * Weight;//各类的权重  Weight[class_numb]
	float * Mu;//各类的均值  Mu[class_numb*dim_numb] Mu[11 21 31 12 22 32]
	float * Sigma;//各类的标准差 Sigma[class_numb*dim_numb*dim_numb] Sigma[111 112 121 122   211 212 221 222 311 312 321 322 ]

	Canshuguji *pro;//传参数估计窗口地址的指针

    // 参考图部分
	Image_Ref * ptoImage_Ref;

	//最小距离分类部分
	int * labelofClassification;
	float KappaofReal;

	//判断是否已经进行实例化
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

	//图像初始化部分
	void init_GrayImg(CString str,int band_sec);//band_sec为选择的波段号
	void init_Parameter(int class_num,int dim_num );//class_num为类别数，dim_num为维数

	//提取纹理特征部分
	void GetImgTextureFeatures(const int window_size,const int d=1,const int gray_scal=32);


	//Gmm估计部分
	void GMM_multi(float** Data,const int Data_size,const int class_num,const int dim_num,const double itol,const int maxiter,float * Init_Weight,float *Init_Mu,float* Init_Sigma);
	void GMM_singl(float* Data,const int Data_size,const int class_num,const double itol,const int maxiter,float * Init_Weight, float *Init_Mu,float* Init_Sigma);
	void kappa_evalu_singl();//单维特征kappa运算，基于最小距离
	void kappa_evalu_singl_minimumerro();//单维特征kappa运算，基于最小错误率
    void kappa_evalu_multi(const int Classifyruleflag,const float step=1.0,const float range_min=-30,const float range_max=280);//多维特征并行kappa计算,Classifyruleflag=0表示最小距离，Classifyruleflag=1表示最小错误率
    void kappa_evalu_multi_DAGOrBiTree(const int Classifyruleflag,int** Strategy,const int flag,const float step=1.0,const float range_min=-30,const float range_max=280);//多维特征DAG形式kappa计算或者二叉树形式，flag为0为DGA，flag为1为二叉树。Classifyruleflag=0表示最小距离，Classifyruleflag=1表示最小错误率
	


	//固定均值，在区间内遍历均值得到GMM估计结果，寻找最大概率kappa
	void FindHighestFrequencyKappa_singl(const int ClassifyRuleflag,float* Data,const int Data_size,float * Init_Weight, float *Init_Mu,float* Init_Sigma,float *IteratRegion,const float step=1.0);//Classifyruleflag=0表示最小距离，Classifyruleflag=1表示最小错误率
	void FindHighestFrequencyKappa_multi(const int ClassifyRuleflag,float** Data,const int Data_size,float * Init_Weight, float *Init_Mu,float* Init_Sigma,float *IteratRegion,const float step=1.0,const float kappaevalu_step=1.0);//Classifyruleflag=0表示最小距离，Classifyruleflag=1表示最小错误率
	void FindHighestFrequencyKappa_DAGOrBiTree(const int Classifyruleflag,int** Strategy,const int flag,float** Data,const int Data_size,float * Init_Weight, float *Init_Mu,float* Init_Sigma,float *IteratRegion,const float step=1.0,const float kappaevalu_step=1.0);//基于最小距离

	//参考图初始化部分
	void Init_Image_Ref(Image_Ref *);
	
	//最小距离分类部分
	void NearestDistanceClassify(float** Data,const int Data_size,const int dim_size,const int class_num,float** Center);
	void KappaofReal_evalu();//最小距离分类后的kappa计算

	void Init_TextureFeatures(const int window_size);//暂时设为public


private:  //内部调用函数
	//最小距离分类部分
	void Init_labelofclassification();

	//提取纹理特征部分 内部调用函数
	//void Init_TextureFeatures(const int window_size);

	void ExtractTextureFeatures(float** ImgBlock,const int window_size,float* result_tex,const int d=1,const int gray_scal=32);
	void caculate_texture(float ** GLCM,const int gray_scal,float* result);//ExtractTextureFeatures中调用的子函数

	//多维GMM运算部分
	void NearestDistanceClassify(float** Data,const int Data_size,const int dim_size,const int class_num,float** Center,int* ResultofLabel);//Kappa计算中调用函数
	//DAG有向无环图
	void NearestDistanceClassify_DAG(float** Data,const int Data_size,const int dim_size,const int class_num,float** Center,int** Stratg,int* ResultofLabel);//DAG下kappa计算调用函数,基于最小距离
	
	//二叉树形式最小距离分类
	void NearestDistanceClassify_BinaryTree(float** Data,const int Data_size,const int dim_size,const int class_num,float** Center,int** Stratg,int* ResultofLabel);//二叉树形式下kappa计算调用函数
	
	//寻找数组中出现最多的那个数值
	float HighestFrequenceInArray(float * Array,const long int Array_size,const float interval=0.01);//最后固定均值遍历得到一系列kappa，寻求最大概率kappa时调用的函数

	//最小错误率分类部分
	void MinimumErroClassify_singl(float* Data,const int Data_size,const int class_num,float *weight,float* Center,float *sigma,int* ResultofLabel);//利用最小错误率时Kappa计算中调用函数
	void MinimumErroClassify_multi(float** Data,const int Data_size,const int dim_size,const int class_num,float *weight,float** Center,float ***sigma,int* ResultofLabel);//利用最小错误率时Kappa计算中调用函数
	void MinimumErroClassify_DAG(float** Data,const int Data_size,const int dim_size,const int class_num,float *weight,float** Center,float ***sigma,int** Stratg,int* ResultofLabel);//DAG下kappa计算调用函数,基于最小错误率
};
