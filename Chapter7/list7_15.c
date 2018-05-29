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
	int val;
	int x,y;
	int xx,yy;
	int i;
	int mx,my;
	int rmx,rmy;
	int d;
	int g,g2;
	double kyori,min;
	Pixel col,ncol;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	for(x=x1;x<=x2;x++) {
		for(y=y1;y<=y2;y++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			g=col.r;
			// 極大（近傍画素の中で最大値を取る）の場合のみ残す
			for(yy=y-1;yy<=y+1;yy++) {
				for(xx=x-1;xx<=x+1;xx++) {
					val = getPixel(img,xx,yy,&ncol);	//画像上の画素情報を取得
					g2=ncol.r;
					if(g2>g) {
						col.r=0;
						col.g=0;
						col.b=0;
					}
				}
			}
			setPixel(outimg,x,y,&col);	// 画像に値をセットする
		}
	}
}
