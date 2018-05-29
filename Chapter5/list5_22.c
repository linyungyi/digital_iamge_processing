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

// エフェクト処理
int effect(ImageData *img,ImageData *outimg)
{
	int x,y;
	int val;
	int rr,gg,bb;
	int adr;
	Pixel col;
	int pat[16]={
		 10, 9, 8, 7,
		 11, 2, 1, 6,
		 12, 3, 4, 5,
		 13,14,15,16
	};

	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			rr=col.r;
			gg=col.g;
			bb=col.b;
			adr=(x%4)+(y%4)*4;
			rr/=16;
			gg/=16;
			bb/=16;
			if(rr<pat[adr]) rr=0;
			else rr=255;
			if(gg<pat[adr]) gg=0;
			else gg=255;
			if(bb<pat[adr]) bb=0;
			else bb=255;
			col.r = rr;
			col.g = gg;
			col.b = bb;
			setPixel(outimg,x,y,&col);	// 画像に値をセットする
		}
	}
	return TRUE;
}
