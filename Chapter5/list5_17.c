#include "image.h"
#include <stdlib.h>

main(){}

// 色ごとの画素数を記録するためのリスト
typedef struct CINFO
{
	int r;
	int g;
	int b;
	int num;
	struct CINFO *next;
} ColInfo;

ColInfo* getMaxNumColor(ColInfo *clist);

// リストによるヒストグラムを求める
int getColorHist(ImageData *img)
{
	int mx,my;		// 画像の大きさ
	int val;
	int x,y;
	int i;
	Pixel col;
	ColInfo clist,*dum;
	int rr,gg,bb;

	mx=img->width;
	my=img->height;

	initCinfo(&clist);
	for(y=0;y<my;y++) {
		for(x=0;x<mx;x++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			rr=col.r;
			gg=col.g;
			bb=col.b;
			if(!existInList(&clist,rr,gg,bb)) {	// リストに色が存在しなかったら
				insColor(&clist,rr,gg,bb);		// リストに追加
			}
		}
	}
}

int initCinfo(ColInfo *clist)
{
	clist->r=0;
	clist->g=0;
	clist->b=0;
	clist->num=0;
	clist->next=NULL;
}

int existInList(ColInfo *clist,int rr,int gg,int bb)
{
	ColInfo *i;

	for(i=clist;i!=NULL; i=i->next) {
		if(clist->r == rr &&
		   clist->g == gg &&
		   clist->b == bb ) {
			return TRUE;
		}
	}
	return FALSE;
}

ColInfo* lastColInfo(ColInfo *clist)
{
	ColInfo *i;

	i=clist;
	while(i!=NULL) {
		if(i->next ==NULL) return i;
	}
	return NULL;
}

int insColor(ColInfo *clist,int rr,int gg,int bb)
{
	ColInfo *dum,*last;

	dum=malloc(sizeof(ColInfo));
	if(dum==NULL) return FALSE;
	last=lastColInfo(clist);
	initCinfo(dum);
	last->next=dum;
	return TRUE;
}
