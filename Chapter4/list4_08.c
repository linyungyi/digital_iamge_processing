#include "image.h"
#include <math.h>
#include <stdlib.h>

#define PAI   3.1415926

// v1とv2の内容を入れ替える
int swapval(int *v1,int *v2)
{
	int dum;

	dum= *v1;
	*v1 = *v2;
	*v2 = dum;
}

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	double rad;
	double dosu;
	int res;

	if(ac<4) {
		printf("パ??ータが足りません");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("画?が読めません");
		return;
	}
	sscanf(av[3],"%lf",&dosu);
	// ?ジア?に変換
	rad=dosu*PAI/180.0;

	transform(img,&outimg,rad);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

double sinTH,cosTH;		
double zmRate=1.0;	   
int reg_x1=0,reg_y1=0;
int reg_x2=0,reg_y2=0;

void setSine(double theta)
{
	sinTH=sin(theta);
	cosTH=cos(theta);
}

 座標変換（変形先→変形元）
int getOrgCod(int x,int y,int ox,int oy,double *tx,double *ty)
{
	double cx,cy;

	cx=x-ox;
	cy=y-oy;
	*tx=( cx*cosTH+cy*sinTH)/zmRate+reg_x1;
	*ty=(-cx*sinTH+cy*cosTH)/zmRate+reg_y1;
}

// 座標変換（変形元→変形先）
int getRotCod(int x,int y,int ox,int oy,int *tx,int *ty)
{
	double cx,cy;
	double fx,fy;

	cx=(double)(x-reg_x1);
	cy=(double)(y-reg_y1);
	fx=zmRate*(cx*cosTH-cy*sinTH)+(double)ox;
	fy=zmRate*(cx*sinTH+cy*cosTH)+(double)oy;

	*tx=(int)(fx+0.5);
	*ty=(int)(fy+0.5);
}



int transform(ImageData *img,ImageData **outimg_p,double theta)
{
	ImageData *outimg;
	int x,y;
	double x1,y1;
	double x2,y2;
	int wt,ht;
	int ox,oy;
	Pixel col,bcol;
	int h1,w1;
	int h2,w2;
	int tx,ty;
	double xx,yy;
	double px[4],py[4];
	int i;

	setSine(theta);

	px[0]=0;
	py[0]=0;

	px[1]=(double)(img->width - 1);
	py[1]=0;

	px[2]=(double)(img->width - 1);
	py[2]=(double)(img->height - 1);

	px[3]=0;
	py[3]=(double)(img->height - 1);

	x1=px[0];	y1=py[0];
	x2=px[0];	y2=py[0];
	ox=px[0];	oy=py[0];
	
	for(i=0;i<4;i++) {
		getRotCod((int)px[i],(int)py[i],ox,oy,&tx,&ty);
		px[i]=tx;
		py[i]=ty;
	}
	for(i=1;i<4;i++) {				
		xx=px[i];
		yy=py[i];
		if(xx<x1) x1=xx;
		if(yy<y1) y1=yy;
		if(xx>x2) x2=xx;
		if(yy>y2) y2=yy;
	}
	ox= -x1;
	oy= -y1;
	wt=(int)(x2-x1+1.5);
	ht=(int)(y2-y1+1.5);

	*outimg_p=createImage(wt,ht,24);
	outimg=(*outimg_p);

	bcol.r=bcol.g=bcol.b=128;
	for(y=0;y<ht;y++) {			
		for(x=0;x<wt;x++) {
			getOrgCod(x,y,ox,oy,&xx,&yy);
			if(interpolate(img,xx,yy,&col)) {
				setPixel(outimg,x,y,&col);	
			}
			else {
				setPixel(outimg,x,y,&bcol);	
			}
		}
	}
}


int interpolate(ImageData *img,double x1,double y1,Pixel *pix)
{
	int x,y;
	
	x=(int)(x1+0.5);
	y=(int)(y1+0.5);
	return getPixel(img,x,y,pix);
}
