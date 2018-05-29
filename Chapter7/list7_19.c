#include "image.h"
#include <math.h>
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	double ang,dis;
	int x,y,mx,my;

	if(ac<3) {
		printf("パラメータが足りません");
		return;
	}

	// ファイルより画像データの読み込み
	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("画像が読めません");
		return;
	}

	hough(img,&outimg,&ang,&dis);
	printf("%f %f\n",ang,dis);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

#define THETA_MAX 32
#define RO_MAX 50
#define MPI 3.14159

int addHough(int *himage,int x,int y,int hmx,int hmy)
{
	if(x<0) return;
	if(x>=hmx) return;
	if(y<0) return;
	if(y>=hmy) return;
	himage[x+y*hmx]++;
}

int hough(ImageData *img,ImageData **oimg,double *ang,double *dis)
{
	int i;
	int x,y;
	int te;
	ImageData *outimg;
	Pixel col,scol;
	int ro,romax;
	double teta,orgro;
	int mx,my;
	int hmx,hmy;
	int maxVal,mmx,mmy;
	
	int *himage;	//ハフ変換した結果

	mx=img->width-1;
	my=img->height-1;
	
	scol.r=0;
	scol.g=0;
	scol.b=0;
	// 対角線の長さを求める
	romax=(int)(sqrt(mx*mx+my*my)+1);
	hmx=THETA_MAX;
	hmy=RO_MAX*2;//romax*2;
	himage=malloc(sizeof(int)*hmx*hmy);
	
	
	for(i=0;i<hmx*hmy;i++) himage[i]=0;
	for(y=0;y<my;y++) {
		for(x=0;x<mx;x++) {
			getPixel(img,x,y,&col);	//画像上の画素情報を取得
			if(col.r>0) {
				for(te=0;te<THETA_MAX;te++) {
					teta=MPI*(double)te/THETA_MAX;
					orgro=x*cos(teta)+y*sin(teta);
					ro=(int)(orgro*RO_MAX/romax +  0.5);
					addHough(himage,te,ro+RO_MAX,hmx,hmy);
					//setPixel(outimg,te,ro+romax,&scol);
				}
			}
		}
	}

	*oimg=createImage(hmx,hmy,24);
	outimg=(*oimg);
	
	maxVal=-1;
	for(y=0;y<hmy;y++) {
		for(x=0;x<hmx;x++) {
			int hval;
		
			hval=himage[x+y*hmx];
			if(hval>maxVal) {
				maxVal=hval;
				mmx=x;
				mmy=y;
			}
			scol.r=hval;
			scol.g=0;
			scol.b=0;
			setPixel(outimg,x,y,&scol);
		}
	}
	*ang=180*(double)mmx/THETA_MAX;
	*dis=(double)(mmy-RO_MAX)*romax/RO_MAX;

	return TRUE;
}
