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
	
	effect(img,outimg,atoi(av[3]));

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

// エフェクト処理
int effect(ImageData *img,ImageData *outimg,int lv)
{
	int x,y;
	int val;
	int rr,gg,bb;
	int vv;
	int adr;
	Pixel col;

	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			rr=col.r;
			gg=col.g;
			bb=col.b;

			vv=rr*lv/255;
			rr = vv*255/lv;

			vv=gg*lv/255;
			gg = vv*255/lv;

			vv=bb*lv/255;
			bb = vv*255/lv;

			col.r = rr;
			col.g = gg;
			col.b = bb;

			setPixel(outimg,x,y,&col);	// 画像に値をセットする
		}
	}
	return TRUE;
}
