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

int getThreshold(int *hist,int n)
{
	int th;
	int i,j;
	double min;
	int sa,sb,da,db;
	double ma,mb,mt;
	double oa,ob;
	double wa,wb,wt;
	double kai;
	double oo;

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
		if(wa!=0.0 && wb!=0.0) {
			oa=ob=0.0;
			for(j=0;j<i;j++) {
				oo=(double)(j-ma);
				oo=oo*oo*hist[j];
				oa+=oo;
			}
			oa/=((double)sa);
			for(j=i+1;j<n;j++) {
				oo=(double)(j-mb);
				oo=oo*oo*hist[j];
				ob+=oo;
			}
			ob/=((double)sb);
			kai=wa*log(oa/wa)+wb*log(ob/wb);
			if(kai>0.0 && (th==0 || min>kai)) {
				min=kai;
				th=i;
			}
		}
	}
	return th;
}

// エフェクト処理
int effect(ImageData *img,ImageData *outimg)
{
	int val;
	int i;
	int x,y;
	int rr,gg,bb;
	int th;
	int hist[256];
	Pixel col;

	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	// ヒストグラム計算
	for(i=0;i<256;i++) hist[i]=0;
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			rr=col.r;
			hist[rr]++;
		}
	}
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

