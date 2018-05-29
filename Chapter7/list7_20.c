#include "image.h"
#include <math.h>
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int cx,cy,cr;
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

	hough(img,&cx,&cy,&cr);
	printf("(%d,%d) r=%d\n",cx,cy,cr);

	disposeImage(img);
}

#define R_MAX 50

#define MPI 3.14159

int addHough(int *himage,int x,int y,int r,int hmx,int hmy,int hmr)
{
	if(x<0) return;
	if(x>=hmx) return;
	if(y<0) return;
	if(y>=hmy) return;
	if(r<0) return;
	if(r>=hmr) return;
	himage[x+y*hmx+r*hmx*hmy]++;
}

int hough(ImageData *img,int *cx,int *cy,int *cr)
{
	int i;
	int x,y,r;
	int te;
	ImageData *outimg;
	Pixel col,scol;
	int ro,romax;
	double teta,orgro;
	int mx,my;
	int hmx,hmy,hmr;
	int xx,yy,rr;
	double dx,dy;
	int maxVal,mmx,mmy,mmr;
	
	int *himage;	//ハフ変換した結果

	mx=img->width-1;
	my=img->height-1;
	
	scol.r=0;
	scol.g=0;
	scol.b=0;

	hmx=mx;
	hmy=my;
	hmr=R_MAX;
	himage=malloc(sizeof(int)*hmx*hmy*hmr);
	
	
	for(i=0;i<hmx*hmy;i++) himage[i]=0;
	for(y=0;y<my;y++) {
		for(x=0;x<mx;x++) {
			getPixel(img,x,y,&col);	//画像上の画素情報を取得
			if(col.r>0) {
				for(yy=0;yy<hmy;yy++) {
					dy=yy-y;
					dy=dy*dy;
					for(xx=0;xx<hmx;xx++) {
						dx=xx-x;
						dx=dx*dx;
						rr=(int)(sqrt(dx+dy)+0.5);
						addHough(himage,xx,yy,rr,hmx,hmy,hmr);
					}
				}
			}
		}
	}

	maxVal=-1;
	for(r=0;r<hmr;r++) {
		for(y=0;y<hmy;y++) {
			for(x=0;x<hmx;x++) {
				int hval;
			
				hval=himage[x+y*hmx+r*hmx*hmy];
				if(hval>maxVal) {
					maxVal=hval;
					mmx=x;
					mmy=y;
					mmr=r;
				}
			}
		}
	}
	*cx=mmx;
	*cy=mmy;
	*cr=mmr;

	return TRUE;
}
