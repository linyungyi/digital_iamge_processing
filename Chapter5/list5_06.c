#include "image.h"
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<4) {
		printf("パラメータが足りません");
		return;
	}

	// ファイルより画像データの読み込み
	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("画像が読めません");
		return;
	}

	// 結果格納用画像データ作成
	outimg=createImage(img->width,img->height,24);
	
	effect(img,outimg,atoi(av[3]) );

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}


#define SCALE 255.0
#define hSCALE 256.0
#define GETA 2.0
#define hGETA 2.0

#define sgn(x) ((x)<0 ? (-1.0):(1.0))
#define CINT(x)  ((int)((x)+sgn(x)*0.5))
#define FL float

int RGBtoHSV(int r,int g,int b,int *h,int *s,int *v)
{
	FL rr,gg,bb;
	FL hh,ss,vv;
	FL cmax,cmin,cdes;

	rr=(FL)r;
	gg=(FL)g;
	bb=(FL)b;

	if(rr>gg) cmax=rr;
	else cmax=gg;
	if(bb>cmax) cmax=bb;

	if(rr<gg) cmin=rr;
	else cmin=gg;
	if(bb<cmin) cmin=bb;

	cdes=cmax-cmin;
	vv=cmax;
	if(cdes!=0.0) {
		ss=cdes*SCALE/cmax;
		if(cmax==rr) {
			hh=(gg-bb)*SCALE/cdes;
		}
		else if(cmax==gg) {
			hh=(bb-rr)*SCALE/cdes+2.0*hSCALE;
		}
		else {
			hh=(rr-gg)*SCALE/cdes+4.0*hSCALE;
		}
	}
	else if(cmax!=0.0) {
		ss=cdes*SCALE/cmax;
		hh=0.0;
	}
	else {
		ss=0.0;
		hh=0.0;
	}
	if(hh<0.0) hh+=6.0*hSCALE;

	*h=CINT(hh*hGETA);
	*s=CINT(ss*GETA);
	*v=CINT(vv*GETA);
}

int HSVtoRGB(int h,int s,int v,int *r,int *g,int *b)
{
	FL hh,ss,vv;
	FL rr,gg,bb;

	if(h==6*hGETA*(hSCALE)) h=0;
	hh=(FL)h/hGETA/*/(SCALE)*/;
	ss=(FL)s/GETA/*/SCALE;*/;
	vv=(FL)v/GETA/*/SCALE*/;

	switch((int)(hh/hSCALE)) {
	case 0:
		rr=1.0*SCALE;
		gg=hh;
		bb=0.0;
		break;
	case 1:
		rr=2.0*hSCALE-hh;
		gg=1.0*SCALE;
		bb=0.0;
		break;
	case 2:
		rr=0.0;
		gg=1.0*SCALE;
		bb=hh-2.0*hSCALE;
		break;
	case 3:
		rr=0;
		gg=4.0*hSCALE-hh;
		bb=1.0*SCALE;
		break;
	case 4:
		rr=hh-4.0*hSCALE;
		gg=0;
		bb=1.0*SCALE;
		break;
	case 5:
		rr=1.0*SCALE;
		gg=0;
		bb=6.0*hSCALE-hh;
		break;
	}

	rr=(rr+(1.0*SCALE-rr)*(1.0*SCALE-ss)/(SCALE))*vv/SCALE;
	gg=(gg+(1.0*SCALE-gg)*(1.0*SCALE-ss)/(SCALE))*vv/SCALE;
	bb=(bb+(1.0*SCALE-bb)*(1.0*SCALE-ss)/(SCALE))*vv/SCALE;
	*r=CINT(rr);
	*g=CINT(gg);
	*b=CINT(bb);
	if(*r > 255) *r=255;
	if(*g > 255) *g=255;
	if(*b > 255) *b=255;
}

// エフェクト処理
int effect(ImageData *img,ImageData *outimg,int gap)
{
	int val;
	int x,y;
	int rr,gg,bb;
	int hh,ss,vv;
	Pixel col;

	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			rr=col.r;
			gg=col.g;
			bb=col.b;
			RGBtoHSV(rr,gg,bb,&hh,&ss,&vv);
			hh=hh+gap*510;
			if(hh>3060) hh-=3060;
			if(hh<0) hh+=3060;
			HSVtoRGB(hh,ss,vv,&rr,&gg,&bb);
			col.r = rr;
			col.g = gg;
			col.b = bb;
			setPixel(outimg,x,y,&col);	// 画像に値をセットする
		}
	}
	return TRUE;
}
