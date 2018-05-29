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

int getThresholdPtile(int *hist,int n,int p)
{
	int i,j;
	int sam,ps;

	sam=0;
	for(i=0;i<n;i++) {
		sam+=hist[i];
	}
	ps=sam*p/100;
	sam=0;
	for(i=0;i<n;i++) {
		sam+=hist[i];
		if(sam>=ps) return i;
	}
	return n-1;
}

int getThreshold(int *hist,int n)
{
	int th;
	int i,j;
	double max;
	int sa,sb,da,db;
	double ma,mb,mt;
	double wa,wb,wt;
	double kai;

	max= (-1000.0);
	th=0;
	for(i=1;i<n-1;i++) {
		da=0;
		sa=0;
		for(j=0;j<i;j++) {
			da+=(hist[j]*j);
			sa+=hist[j];
		}
		db=0;
		sb=0;
		for(j=i;j<n;j++) {
			db+=(hist[j]*j);
			sb+=hist[j];
		}
		if(sa!=0.0)
			ma=(double)da/(double)sa;
		else
			ma=0.0;
		if(sb!=0.0)
			mb=(double)db/(double)sb;
		else
			mb=0.0;
		mt=(double)(da+db)/(double)(sa+sb);
		wa=(double)sa/(double)(sa+sb);
		wb=(double)sb/(double)(sa+sb);
		kai=wa*(ma-mt)*(ma-mt)+wb*(mb-mt)*(mb-mt);
		if(max<kai) {
			max=kai;
			th=i;
		}
	}
	return th;
}

// エフェクト処理
int effect(ImageData *img,ImageData *outimg)
{
	int val;
	int i,j;
	int x,y;
	int xx,yy;
	int pix;
	int rr,gg,bb;
	int th,th_e;
	int *hist2;
	int ehist[256];
	int sam;
	int a;
	int hist[256];
	int fil[3][3]={
			 0, 1, 0,
			 1,-4, 1,
			 0, 1, 0
	};
	int x1,y1,x2,y2;
	Pixel col;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	// ptileのパラメータ
	a=90;
	// ヒストグラム計算
	for(i=0;i<256;i++) {
		hist[i]=0;
		ehist[i]=0;
	}
	hist2=malloc(sizeof(int)*256*256);
	for(y=0;y<256;y++) {
		for(x=0;x<256;x++) {
			hist2[x+y*256]=0;
		}
	}
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			rr=col.r;
			sam=0;
			for(yy=0;yy<3;yy++) {
				for(xx=0;xx<3;xx++) {
					val = getPixel(img,x+xx-1,y+yy-1,&col);	//画像上の画素情報を取得
					gg=col.r;
					sam+=(gg*fil[xx][yy]);
				}
			}
			if(sam<0) sam=0;
			if(sam>255) sam=255;
			hist2[rr+sam*256]++;
			ehist[sam]++;
		}
	}
	th_e=getThresholdPtile(ehist,256,a);
	for(i=0;i<256;i++) {
		for(j=th_e;j<256;j++) {
			hist[i]+=hist2[i+j*256];
		}
	}
	free(hist2);
	th=getThreshold(hist,256);

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

