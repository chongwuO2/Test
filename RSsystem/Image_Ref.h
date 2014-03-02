#include "gdal_priv.h"

//参考图类
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
	int xlen; //图像宽
	int ylen; //高
	int zlen;
	int imgsize;//宽*高，图像大小
	float *Image_refR;
	float *Image_refG;
	float *Image_refB;

public:
	int *Label_ref;//图像真实标签大小为imagsize
	int class_numb;
	ColortoLabel* ColorBox;
};
	