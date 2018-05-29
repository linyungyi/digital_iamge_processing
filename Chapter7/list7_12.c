#include "image.h"
#include <stdlib.h>
#include <math.h>

typedef struct PNTS {
	int x,y;
	struct PNTS *next;
} Points;

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	double len,ang,bre;
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

	int x,y,xx,yy;
	int sum;
	int len,max;
	double rlen;
	Pixel col;
	Points pnt,*p,*q;
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

	pnt.next=NULL;
	len=traceContour(img,&pnt,x1,y1,x2,y2);

	rlen=(double)len/100.0;

	printf("Length=%f\n",rlen);
	printf("Area=%d\n",sum);
	printf("Circle=%f\n", ((double)sum)*4.0*3.1415927/(rlen*rlen) );

	for(p=pnt.next;p!=NULL;p=q) {
		q=p->next;
		free(p);
	}
	return TRUE;
}

int drct_x[8]=
	{-1, 0, 1, 1, 1, 0,-1,-1};
int drct_y[8]=
	{ 1, 1, 1, 0,-1,-1,-1, 0};

int traceContour(ImageData *img,Points *pnt,int x1,int y1,int x2,int y2)
{
	int val;

	int x,y,i,j;
	int xx,yy,sx,sy;
	int dist;
	int first;
	int ro,go,bo;
	int sum;
	int len;
	Pixel col,scol;
	Points *npt,*tail;

	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			if(col.r>0) goto BREAK;
		}
	}
	if(y>y2) return;
BREAK:
	len=0;
	dist=0;
	first=TRUE;
	sx=x;
	sy=y;
	pnt->x=x;
	pnt->y=y;
	pnt->next=NULL;
	tail=NULL;
	while((sx!=x || sy!=y) || first) {
		first=FALSE;
		for(i=0;i<8;i++) {
			j=(i+dist)%8;
			xx=x+drct_x[j];
			yy=y+drct_y[j];
			if(getPixel(img,xx,yy,&col))
				if(col.r>0) break;
		}
		if(i==8) break;
		x=xx;
		y=yy;
		// 長さを求める
		if(j%2==0) len+=141; //斜め方向は√2
		else len+=100;

		dist=(j+6)%8;
		npt=malloc(sizeof(Points));
		npt->x=x;
		npt->y=y;
		npt->next=NULL;
		if(tail) {
			tail->next=npt;
		}
		else {
			pnt->next=npt;
		}
		tail=npt;
	}
	return len;
}
