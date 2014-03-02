#include "StdAfx.h"
#include "Image_Ref.h"
//#include <iostream>
//using namespace std;

Image_Ref::Image_Ref()
{
	xlen=0;
	ylen=0;
	zlen=0;
	imgsize=0;
	Image_refR=NULL;
	Image_refG=NULL;
	Image_refB=NULL;
	Label_ref=NULL;
	class_numb=0;
	ColorBox=NULL;
}


Image_Ref::~Image_Ref()
{
	CPLFree(Image_refR);
	CPLFree(Image_refG);
	CPLFree(Image_refB);
	delete ColorBox;
	delete Label_ref;

}

void Image_Ref::init_Image_Ref(GDALDataset* pdataset)
{

	xlen = pdataset->GetRasterXSize();
	ylen = pdataset->GetRasterYSize();
	zlen = pdataset->GetRasterCount();
	imgsize = xlen * ylen;
	GDALRasterBand *pBandR=pdataset->GetRasterBand(1);
	/*if (pBandR== NULL)  
	{  
		cout<<"pBandR Fail"<<endl;  
	} */
	Image_refR = (float*)CPLMalloc(sizeof(float)*imgsize);
	pBandR->RasterIO(GF_Read,0,0,xlen,ylen,Image_refR,xlen,ylen,GDT_Float32,0,0);

	GDALRasterBand *pBandG=pdataset->GetRasterBand(2);
	Image_refG = (float*)CPLMalloc(sizeof(float)*imgsize);
	pBandG->RasterIO(GF_Read,0,0,xlen,ylen,Image_refG,xlen,ylen,GDT_Float32,0,0);

	GDALRasterBand *pBandB=pdataset->GetRasterBand(3);
	Image_refB = (float*)CPLMalloc(sizeof(float)*imgsize);
	pBandB->RasterIO(GF_Read,0,0,xlen,ylen,Image_refB,xlen,ylen,GDT_Float32,0,0);
}



void Image_Ref::SetClassNum(int num){class_numb=num;}

void Image_Ref::SetColorBox(ColortoLabel* colorbox,int numb)
{
	SetClassNum(numb);
	ColorBox=new ColortoLabel[class_numb];
	memset(ColorBox,0,class_numb*sizeof(ColortoLabel));
	for (int i=0;i<class_numb;i++)
	{
		ColorBox[i]=colorbox[i];

	}
}

void Image_Ref::GetLabel()
{
	Label_ref=new int [imgsize];
	memset(Label_ref,0,imgsize*sizeof(imgsize));
	for (int i=0;i<imgsize;i++)
	{
		for (int j=0;j<class_numb;j++)
		{
			if (Image_refR[i]==float(ColorBox[j].R) && Image_refG[i]==float(ColorBox[j].G) && Image_refB[i]==float(ColorBox[j].B))
			{
				Label_ref[i]=ColorBox[j].label;
				break;
			}
		}
		
	}
			
}
