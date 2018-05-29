#include "image.h"
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<5) {
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
	
	effect(img,outimg,atoi(av[3]),atoi(av[4]));

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

int getThreshold(int *hist,int n,double *k)
{
	int th;
	int i,j;
	double max,max_mt;
	int sa,sb,da,db;
	double ma,mb,mt;
	double wa,wb,wt;
	double oo,ot,st,dt;
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
			max_mt=mt;
			max=kai;
			th=i;
		}
	}
	st=0;
	dt=0;
	for(j=0;j<n;j++) {
		oo=(double)j-max_mt;
		oo=oo*oo*(double)hist[j];
		dt+=oo;
		st+=(double)hist[j];
	}
	ot=dt/st;
	*k=max/ot;
	return th;
}
// エフェクト処理
// p 割合 (0...100)
int effect(ImageData *img,ImageData *outimg,int area,int kpi)
{
	int val;
	int i;
	int x,y;
	int xx,yy;
	int lx,ly;
	int sx,ex,sy,ey;
	int sam,nn;
	int first;
	int rr,gg,bb;
	int th,nowth,newth;
	int hist[256];
	double a,kp;
	Pixel col;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	kp=(double)kpi/100.0;
	lx=(x2-x1)/area;
	ly=(y2-y1)/area;
	nowth=-1;
	for(y=0;y<=ly;y++) {
		for(x=0;x<=lx;x++) {
			// 部分領域の座標
			sx=x1+x*area;
			ex=x1+(x+1)*area;
			sy=y1+y*area;
			ey=y1+(y+1)*area;
			// ヒストグラム取得
			for(i=0;i<256;i++) hist[i]=0;
			for(yy=sy;yy<ey;yy++) {
				for(xx=sx;xx<ex;xx++) {
					val = getPixel(img,xx,yy,&col);	//画像上の画素情報を取得
					rr=col.r;
					hist[rr]++;
				}
			}
			newth=getThreshold(hist,256,&a);
			if(a>kp || th<0) {	// 双峰的な場合，閾値を生かす
				nowth=newth;
			}
			if(a>kp) th=nowth;
			else th=(nowth+newth)/2;
			for(yy=sy;yy<ey;yy++) {
				for(xx=sx;xx<ex;xx++) {
					val = getPixel(img,xx,yy,&col);	//画像上の画素情報を取得
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
					setPixel(outimg,xx,yy,&col);	// 画像に値をセットする
				}
			}
		}
	}
	return TRUE;
}

