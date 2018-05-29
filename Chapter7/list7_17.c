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

int del_pat[8][9]={
	 0, 0,-1,
	 0, 1, 1,
	-1, 1,-1,

	 0, 0, 0,
	-1, 1,-1,
	 1, 1,-1,

	-1, 0, 0,
	 1, 1, 0,
	-1, 1,-1,

	 1,-1, 0,
	 1, 1, 0,
	-1,-1, 0,

	-1, 1,-1,
	 1, 1, 0,
	-1, 0, 0,

	-1, 1, 1,
	-1, 1,-1,
	 0, 0, 0,

	-1, 1,-1,
	 0, 1, 1,
	 0, 0,-1,

	 0,-1,-1,
	 0, 1, 1,
	 0,-1, 1
};

// エフェクト処理
int effect(ImageData *img,ImageData *outimg)
{
	int i,n;
	int x,y;
	int xx,yy;
	int mx,my;
	int rmx,rmy;
	int del;
	Pixel col;
	int val;
	char *img1,*img2;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	rmx=(x2-x1)+1;
	rmy=(y2-y1)+1;
	/* バッファは周囲１画素分余分にとる */
	mx=rmx+2;
	my=rmy+2;

	img1=malloc(sizeof(char)*mx*my);
	img2=malloc(sizeof(char)*mx*my);
	for(i=0;i<mx*my;i++) {
		img1[i]=0;
		img2[i]=0;
	}

	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			getPixel(img,x,y,&col);	//画像上の画素情報を取得
			if(col.r>0) {
				setBuf(img1,x-x1,y-y1,mx,1);
			}
		}
	}
	del=1;
	while(del>0) {
		del=0;
		for(n=0;n<8;n++) {
			for(y=0;y<my;y++) {
				for(x=0;x<mx;x++) {
					val=getBuf(img1,x,y,mx);
					if(val && delPoint(img1,x,y,mx,del_pat[n])) {
						val=0;
						del++;
					}
					setBuf(img2,x,y,mx,val);
				}
			}
			for(i=0;i<mx*my;i++) {
				img1[i]=img2[i];
				img2[i]=0;
			}
		}
	}
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			val=getBuf(img1,x-x1,y-y1,mx);
			if(val) {
				col.r=255;
				col.g=255;
				col.b=255;
			}
			else {
				col.r=0;
				col.g=0;
				col.b=0;
			}
			setPixel(outimg,x+xx,y+yy,&col);
		}
	}
	return TRUE;
}

int delPoint(char *buf,int x,int y,int mx,int *pat)
{
	int sum;
	int val,p;
	int xx,yy;

	sum=0;
	for(yy=0;yy<=2;yy++) {
		for(xx=0;xx<=2;xx++) {
			val=getBuf(buf,x+xx-1,y+yy-1,mx);
			p=pat[xx+yy*3];
			if(p>=0 && val!=p) sum++;
		}
	}
	if(sum>0) return FALSE;
	return TRUE;
}

int setBuf(char *buf,int x,int y,int mx,int d)
{
	buf[x+1+(y+1)*mx]=d;
}

int getBuf(char *buf,int x,int y,int mx)
{
	return buf[(x+1)+(y+1)*mx];
}
