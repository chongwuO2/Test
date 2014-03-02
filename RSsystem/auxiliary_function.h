
float normcdf(const float x,const float mu,const float sigma,const float interval=0.001); //正态分布求积分


//多维正态概率密度函数
//X[dim_num]为输入的X向量，Mu[dim_num]为均值，Sigma[dim_num][dim_num]为协方差
float mvnpdf(float* X,float* Mu,float ** Sigma,const int dim_num);
void sort_mu(float * mu,float* weight,float * sigma,int class_num);//根据mu排序调整其他参数

//矩阵运算的一些函数
float det(float* A,int m,int n);//求矩阵的行列式
void MatrixTranspose(float* A,float* B,int m,int n);   //矩阵转置，B是A的转置
void MatrixInverse(float* A,float* B,int m,int n);   /*矩阵求逆,B是A的逆*/ 

//数组归一化的函数
void normlize(float * Array,const int array_size,const float range_min,const float range_max);

