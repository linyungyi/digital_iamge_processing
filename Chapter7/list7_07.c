#include "image.h"
#include <stdlib.h>
#include <math.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
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

	// 結果格納用画像データ作成
	outimg=createImage(img->width,img->height,24);
	
	effect(img,outimg);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

// エフェクト処理
int effect(ImageData *img,ImageData *outimg)
{
	int val;
	int i;
	int x,y;
	int xx,yy;
	int rr,gg,bb;
	int th,th_e;
	int ehist[256];
	Pixel col;
	int sam,sam1,sam2;
	int max;
	int fil1[3][3]={
			-1,-2,-1,
			 0, 0, 0,
			 1, 2, 1
	};
	int fil2[3][3]={
			-1, 0, 1,
			-2, 0, 2,
			-1, 0, 1
	};
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	// 微分ヒストグラム計算
	for(i=0;i<256;i++) {
		ehist[i]=0;
	}
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			rr=col.r;
			sam1=sam2=0;
			for(yy=0;yy<3;yy++) {
				for(xx=0;xx<3;xx++) {
					val = getPixel(img,x+xx-1,y+yy-1,&col);	//画像上の画素情報を取得
					gg=col.r;
					sam1+=(gg*fil1[xx][yy]);
					sam2+=(gg*fil2[xx][yy]);
				}
			}
			sam=sam1*sam1+sam2*sam2;
			ehist[rr]+=sam;
		}
	}
	max=-1;
	for(i=0;i<256;i++) {
		if(max<ehist[i]) {
			th=i;
			max=ehist[i];
		}
	}

	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			rr=col.r;
			gg=col.g;
			bb=col.b;
			if(th<rr) rr=255;
			else rr=0;
			if(th<gg) gg=255;
			else gg=0;
			if(th<bb) bb=255;
			else bb=0;
			col.r = rr;
			col.g = gg;
			col.b = bb;
			setPixel(outimg,x,y,&col);	// 画像に値をセットする
		}
	}
	return TRUE;
}

