#include "image.h"
#include <math.h>
#include <stdlib.h>

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
	int i,adr;
	int px[4],py[4];
	int res;

	if(ac<(3+4*2)) {
		printf("パ??ータが足りません");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("画?が読めません");
		return;
	}
	adr=3;
	for(i=0;i<4;i++) {
		px[i]=atoi(av[adr]);
		adr++;
		py[i]=atoi(av[adr]);
		adr++;
	}

	transform(img,&outimg,px,py);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

int resolve2Equ(double a,double b,double c,double *x1,double *x2)
{
	double dd;

	if(a==0.0) {
		if(c==0.0) return FALSE;
		*x1=-b/c;
		*x2=-b/c;
		return TRUE;
	}
	dd=b*b-4.0*a*c;
	*x1=0.0;
	*x2=0.0;
	if(dd<0.0) {
		return FALSE;
	}
	*x1=(-b+sqrt(dd))/(2.0*a);
	*x2=(-b-sqrt(dd))/(2.0*a);
	return TRUE;
}

int resolveEqu(double a,double b,double c,double d,
				double e,double f,double g,double h,
				double *xa,double *ya,double *xb,double *yb)
{
	double p1,p2,p3;
	double x1,x2;
	double y1,y2;
	double tm;
	char cbuf[256];

	p1=(double)(a*h-f*c);
	p2=(double)(d*f-c*g+b*h-a*e);
	p3=(double)(d*g-b*e);
	if(p1==0.0) {
		*xa=d/b;
		*ya=(b*e-d*g)/(b*h+d*f);
		*xb=-1000;
		*yb=-1000;
		return TRUE;
	}
	if(!resolve2Equ(p1,p2,p3,ya,yb)) {
		*xa=-1000;
		*xb=-1000;
		return TRUE;
	}
	tm=(a*(*ya)+b);
	if(tm)
		*xa=(d-c*(*ya))/tm;
	else {
		*xa=-1000;
	}
	tm=(a*(*yb)+b);
	if(tm)
		*xb=(d-c*(*yb))/tm;
	else
		*xb=-1000.0;
	return TRUE;
}


int transform(ImageData *img,ImageData **outimg_p,int *px,int *py)
{
	ImageData *outimg;
	int x,y;
	int wt,ht;

	int i;
	Pixel col,bcol;
	int h1,w1;
	int h2,w2;
    int rx1,ry1,rx2,ry2;
	double pa[4],pb[4];
	double aa,bb,cc,dd,ee,ff,gg,hh;
	int xa,ya,xb,yb;
	int xc,yc,xd,yd;

	rx1=0;
	ry1=0;
	rx2=img->width-1;
	ry2=img->height-1;

	wt=px[0];	ht=py[0];
	for(i=1;i<4;i++) {
		x=px[i];
		y=py[i];
		if(x>wt) wt=x;
		if(y>ht) ht=y;
	}
	wt++;
	ht++;
	*outimg_p=createImage(wt,ht,24);
	outimg=(*outimg_p);
	// 変形先領域の頂点を取得
	xa=(double)px[0];
	ya=(double)py[0];
	xb=(double)px[1];
	yb=(double)py[1];
	xc=(double)px[2];
	yc=(double)py[2];
	xd=(double)px[3];
	yd=(double)py[3];

	aa=(double)(xc-xd-xb+xa);
	bb=(double)(xb-xa);
	cc=(double)(xd-xa);
	ff=(double)(yc-yd-yb+ya);
	gg=(double)(yb-ya);
	hh=(double)(yd-ya);


	bcol.r=bcol.g=bcol.b=128;
	for(y=0;y<ht;y++)	{
		for(x=0;x<wt;x++)	{
			double ss1,tt1,ss2,tt2;
			double xx,yy;
			double ttx,tty;

			dd=(double)(x-xa);
			ee=(double)(y-ya);

			if(!resolveEqu(aa,bb,cc,dd,ee,ff,gg,hh,&ss1,&tt1,&ss2,&tt2)) {
				printf("??できません\n");
				return;
			}
る解を用いる
			if(ss1>=0 && tt1>=0 && ss1<=1.0 && tt1<=1.0) {
				ttx=ss1*(rx2-rx1)+rx1;
				tty=tt1*(ry2-ry1)+ry1;
			}
			else if(ss2>=0 && tt2>=0 && ss2<=1.0 && tt2<=1.0) {
				ttx=ss2*(rx2-rx1)+rx1;
				tty=tt2*(ry2-ry1)+ry1;
			}
			else {
				// 0<=s<=1 0<=t<=1となる解がなければ何もしない
				setPixel(outimg,x,y,&bcol);	
				continue;
			}
			if(interpolate(img,ttx,tty,&col)) {
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
