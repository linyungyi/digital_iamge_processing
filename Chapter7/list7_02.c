#include "image.h"
#include <stdlib.h>

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
	
	effect(img,outimg,128);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}


// エフェクト処理
int effect(ImageData *img,ImageData *outimg,int th)
{
	int val;
	int i;
	int x,y;
	int rr,gg,bb;
	int endn;
	Pixel col;

	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

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

