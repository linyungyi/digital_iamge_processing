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
	int i,m;
	int x,y;
	int xx,yy;
	int mx,my;
	int rmx,rmy;
	int del;
	Pixel col;
	int val,nval;
	char *img1;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	rmx=(x2-x1)+1;
	rmy=(y2-y1)+1;
	/* 距離バッファは周囲１画素分余分にとる */
	mx=rmx+2;
	my=rmy+2;

	img1=malloc(sizeof(char)*mx*my);
	for(i=0;i<mx*my;i++) {
		img1[i]=0;
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
		for(y=0;y<rmy;y++) {
			for(x=0;x<rmx;x++) {
				val=getBuf(img1,x,y,mx);
				if(val==1) {
					if(delPoint(img1,x,y,mx)) {
						val=-1;
						del++;
					}
				}
				setBuf(img1,x,y,mx,val);
			}
		}
		for(i=0;i<mx*my;i++) {
			if(img1[i]==-1)
				img1[i]=0;
		}
	}
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			val=getBuf(img1,x-x1,y-y1,mx);
			if(val==1) {
				col.r=255;
				col.g=255;
				col.b=255;
			}
			else {
				col.r=0;
				col.g=0;
				col.b=0;
			}
			setPixel(outimg,x,y,&col);
		}
	}
	return TRUE;
}

int delPoint(char *buf,int x,int y,int mx)
{
	int n[9],sn[9];
	int i,j,k;
	int sum,psum;

	n[1]=getBuf(buf,x+1,y  ,mx);
	n[2]=getBuf(buf,x+1,y-1,mx);
	n[3]=getBuf(buf,x  ,y-1,mx);
	n[4]=getBuf(buf,x-1,y-1,mx);
	n[5]=getBuf(buf,x-1,y  ,mx);
	n[6]=getBuf(buf,x-1,y+1,mx);
	n[7]=getBuf(buf,x  ,y+1,mx);
	n[8]=getBuf(buf,x+1,y+1,mx);
	for(i=1;i<9;i++) {
		if(n[i]<0) sn[i] = -n[i];
		else sn[i]=n[i];
	}
	/* 境界であるか */
	if(n[1]+n[3]+n[5]+n[7]==4) return FALSE;
	
	sum=psum=0;
	for(i=1;i<9;i++) {
		psum+=sn[i];
		if(n[i]>0)
			sum+=n[i];
	}
	/* 端点か */
	if(psum<2) return FALSE;
	
	/* 孤立点か */
	if(sum<1) return FALSE;

	/* 連結性を保持できるか */
	sum=getConnect(sn);
	if(sum!=1) return FALSE;

	/* 連結性を保持できるか2 */
	for(i=1;i<9;i++) {
		int tmp;

		if(n[i]<0) sn[i]=0;
	}
	sum=getConnect(sn);
	if(sum!=1) return FALSE;
	return TRUE;
}

int getConnect(int *sn)
{
	int sum;
	int i,j,k;

	sum=0;
	for(i=1;i<9;i+=2) {
		j=i+1;
		k=i+2;
		if(j>8) j-=8;
		if(k>8) k-=8;
		sum+=(sn[i]-sn[i]*sn[j]*sn[k]);
	}
	return sum;
}

int setBuf(char *buf,int x,int y,int mx,int d)
{
	buf[x+1+(y+1)*mx]=d;
}

int getBuf(char *buf,int x,int y,int mx)
{
	return buf[(x+1)+(y+1)*mx];
}
