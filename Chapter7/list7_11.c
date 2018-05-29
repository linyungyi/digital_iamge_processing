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
	int x,y,xx,yy;
	int xx1,yy1;
	int xx2,yy2;
	int sx1,sy1;
	int sx2,sy2;
	int neis,area;
	int ro,go,bo;
	int sum;
	int endn;
	int len,max;
	double ang,sinTH,cosTH;
	double dx,dy,rx,ry;
	double max_x,max_y;
	double min_x,min_y;
	Pixel col;
	Points pnt,*p,*q;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	sum=0;

	pnt.next=NULL;
	traceContour(img,&pnt,x1,y1,x2,y2);
	max=-10;
	for(p=&pnt;p->next!=NULL;p=p->next) {
		for(q=p->next;q!=NULL;q=q->next) {
			xx1=p->x;
			yy1=p->y;
			xx2=q->x;
			yy2=q->y;
			xx=xx2-xx1;
			yy=yy2-yy1;
			len=xx*xx+yy*yy;
			if(max<len) {
				max=len;
				sx1=xx1;
				sy1=yy1;
				sx2=xx2;
				sy2=yy2;
			}
		}
	}
	if(sx2==sx1) {
		ang=3.1415926/2.0;
	}
	else {
		ang=atan((double)(sy2-sy1)/(double)(sx2-sx1));
	}
	min_x=1000000;
	min_y=1000000;
	max_x= -1000000;
	max_y= -1000000;
	sinTH=sin(-ang);
	cosTH=cos(-ang);
	for(p=&pnt;p->next!=NULL;p=p->next) {
		dx=(double)(p->x - sx1);
		dy=(double)(p->y - sy1);
		rx=cosTH*dx-sinTH*dy;
		ry=sinTH*dx+cosTH*dy;
		if(min_y>ry) min_y=ry;
		if(max_y<ry) max_y=ry;
    }
    
    printf("MaxLength=%f\n",sqrt((double)max));
    printf("  座標=(%d,%d)-(%d,%d)\n",sx1,sy1,sx2,sy2);
    printf("Angle=%f\n",ang*180.0/3.1415926);
    printf("Breadth=%f\n",max_y-min_y);
    
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
}
