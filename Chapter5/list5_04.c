#include "image.h"
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<3+9) {
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
	
	effect(img,outimg,atoi(av[3]),atoi(av[4]),atoi(av[5]),atoi(av[6]),atoi(av[7]),atoi(av[8]),atoi(av[9]),atoi(av[10]),atoi(av[11]) );

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

int func_v(int rr,int gg,int bb,int a,int b,int c)
{
    int v;

    v=a*rr+b*gg+c*bb;
    v/=255;
    if(v<0) v=0;
    if(v>255) v=255;
    return v;
}

// エフェクト処理
int effect(ImageData *img,ImageData *outimg,int a,int b,int c,int d,int e,int f,int g,int h,int i,int j)
{
	int x,y;
	int val;
	int rr,gg,bb;
	int rrt,grt,brt;
	Pixel col;

	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			rr=col.r;
			gg=col.g;
			bb=col.b;
			col.r = func_v(rr,gg,bb,a,b,c);
			col.g = func_v(rr,gg,bb,d,e,f);
			col.b = func_v(rr,gg,bb,g,h,i);
			setPixel(outimg,x,y,&col);	// 画像に値をセットする
		}
	}
	return TRUE;
}
