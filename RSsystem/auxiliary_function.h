
float normcdf(const float x,const float mu,const float sigma,const float interval=0.001); //��̬�ֲ������


//��ά��̬�����ܶȺ���
//X[dim_num]Ϊ�����X������Mu[dim_num]Ϊ��ֵ��Sigma[dim_num][dim_num]ΪЭ����
float mvnpdf(float* X,float* Mu,float ** Sigma,const int dim_num);
void sort_mu(float * mu,float* weight,float * sigma,int class_num);//����mu���������������

//���������һЩ����
float det(float* A,int m,int n);//����������ʽ
void MatrixTranspose(float* A,float* B,int m,int n);   //����ת�ã�B��A��ת��
void MatrixInverse(float* A,float* B,int m,int n);   /*��������,B��A����*/ 

//�����һ���ĺ���
void normlize(float * Array,const int array_size,const float range_min,const float range_max);

