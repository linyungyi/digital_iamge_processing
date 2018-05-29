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
int getMin(int *x,int n)
{
	int res;
	int i;

	res=x[0];
	for(i=1;i<n;i++) {
		if(res>x[i]) res=x[i];
	}
	return res;
}

int dis_trans(short *dist,int mx,int my)
{
	int x,y;
	int sx,sy;
	int d,nd;
	int k[4];
	int mink;

	for(y=0;y<my;y++) {
		for(x=0;x<mx;x++) {
			d=getDist(dist,x,y,mx+2);
			if(d) {
				k[0]=getDist(dist,x-1,y  ,mx+2)+10;
				k[1]=getDist(dist,x  ,y-1,mx+2)+10;
				k[2]=getDist(dist,x-1,y-1,mx+2)+10;// Del
				k[3]=getDist(dist,x+1,y-1,mx+2)+10;// Del
				nd=getMin(k,4); // nd=getMin(k,2);
				if(nd<d) setDist(dist,x,y,mx+2,nd);
			}
		}
	}
	for(y=my-1;y>=0;y--) {
		for(x=mx-1;x>=0;x--) {
			d=getDist(dist,x,y,mx+2);
			if(d) {
				k[0]=getDist(dist,x+1,y  ,mx+2)+10;
				k[1]=getDist(dist,x  ,y+1,mx+2)+10;
				k[2]=getDist(dist,x+1,y+1,mx+2)+10;// Del
				k[3]=getDist(dist,x-1,y+1,mx+2)+10;// Del
				nd=getMin(k,4); // nd=getMin(k,2);
				if(nd<d) setDist(dist,x,y,mx+2,nd);
			}
		}
	}
}

// エフェクト処理
int effect(ImageData *img,ImageData *outimg)
{
	int val;
	int x,y;
	int i;
	int mx,my;
	int rmx,rmy;
	int d;
	int prev,tmp;
	short *dist;
	double kyori,min;
	Pixel col;
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

	dist=malloc(sizeof(short)*mx*my);
	for(y=0;y<my;y++) {
		for(x=0;x<mx;x++) {
			dist[x+y*mx]=0;
		}
	}
	/* Step 1 */
	d=0;
	for(x=x1;x<=x2;x++) {
		for(y=y1;y<=y2;y++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			if(col.r>0) {
				setDist(dist,x-x1,y-y1,mx,30000);
			}
		}
	}
	dis_trans(dist,rmx,rmy);
	// 以下は可視化処理
	for(x=x1;x<=x2;x++) {
		for(y=y1;y<=y2;y++) {
			d=getDist(dist,x-x1,y-y1,mx);
			col.r=d;
			col.g=d;
			col.b=d;
			setPixel(outimg,x,y,&col);	// 画像に値をセットする
		}
	}
	free(dist);
}

int setDist(short *dist,int x,int y,int mx,int d)
{
	dist[x+1+(y+1)*mx]=d;
}

int getDist(short *dist,int x,int y,int mx)
{
	return dist[(x+1)+(y+1)*mx];
}
