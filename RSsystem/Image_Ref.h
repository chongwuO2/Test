#include "gdal_priv.h"

//�ο�ͼ��
typedef struct  
{
	int R;
	int G;
	int B;
	int label;
}ColortoLabel;

class Image_Ref
{  public:
   Image_Ref();
   virtual ~Image_Ref();
   void init_Image_Ref(GDALDataset* pdataset);
   void SetClassNum(int num);
   void SetColorBox(ColortoLabel* colorbox,int numb);
   void GetLabel();

private:
	int xlen; //ͼ���
	int ylen; //��
	int zlen;
	int imgsize;//��*�ߣ�ͼ���С
	float *Image_refR;
	float *Image_refG;
	float *Image_refB;

public:
	int *Label_ref;//ͼ����ʵ��ǩ��СΪimagsize
	int class_numb;
	ColortoLabel* ColorBox;
};
	