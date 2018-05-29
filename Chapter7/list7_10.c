#include "image.h"
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<2) {
		printf("パラメータが足りません");
		return;
	}

	// ファイルより画像データの読み込み
	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("画像が読めません");
		return;
	}

	analyze(img);

	disposeImage(img);
}

int analyze(ImageData *img)
{
	int val;
	int x,y;
	int xx,yy;
	int neis,area;
	int sum;
	Pixel col;

	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	sum=0;
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			if(col.r>0) {
				sum++;
			}
		}
	}
	printf("Area=%d\n",sum);
	return TRUE;
}
