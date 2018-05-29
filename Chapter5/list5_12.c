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
	
	effect(img,outimg);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

// 色集合（パレット）の作成
int makepallete(int *r,int *g,int *b)
{
	int i;
	int rr,gg,bb;
	for(i=0;i<256;i++) {
		rr=(i>>5)&7;
		gg=(i>>2)&7;
		bb=i&3;
		r[i]=rr*255/7;
		g[i]=gg*255/7;
		b[i]=bb*255/3;
	}
}

// 色集合より最も近い色を取得
int get_color(Pixel *col,int *r,int *g,int *b)
{
	int i,v;
	int rr,gg,bb;
	int min,minV;

	minV=2100000000;
	rr=col->r;
	gg=col->g;
	bb=col->b;
	for(i=0;i<256;i++) {
		v=(rr-r[i])*(rr-r[i])+(gg-g[i])*(gg-g[i])+(bb-b[i])*(bb-b[i]);
		if(minV>v) {
			min=i;
			minV=v;
		}
	}
	return min;
}

// エフェクト処理
int effect(ImageData *img,ImageData *outimg)
{
	int x,y;
	int val;
	int rr,gg,bb;
	int pr[256],pg[256],pb[256];
	int idx;
	int lv;
	Pixel col;

	makepallete(pr,pg,pb);
	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			rr=col.r;
			gg=col.g;
			bb=col.b;
			idx=get_color(&col,pr,pg,pb);
			col.r=pr[idx];
			col.g=pg[idx];
			col.b=pb[idx];
			setPixel(outimg,x,y,&col);	// 画像に値をセットする
		}
	}
	return TRUE;
}
