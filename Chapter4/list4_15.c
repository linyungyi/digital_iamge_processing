
#include "image.h"
#include <math.h>
#include <stdlib.h>
#define Pai 3.14159265359

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
	int res;
	int nx,ny;

	if(ac<5) {
		printf("ƒp??[ƒ^‚ª‘«‚è‚Ü‚¹‚ñ");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("‰æ?‚ª“Ç‚ß‚Ü‚¹‚ñ");
		return;
	}
	nx=atoi(av[3]);
	ny=atoi(av[4]);


	outimg=createImage(nx,ny,24);

	transform(img,outimg);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}
double getVorg(double x,double y,double r)
{
	double f;
	double rt;
	double l,s;
	double theta;
	double vy;

	if(y==0.0) {
		vy=0.0;
	}
	else {
		rt=(r*r-x*x)/(y*y);
		vy=1.0/sqrt(rt);
		if(y<0.0) vy= -vy;
	}
	return vy;
}


int transform(ImageData *img,ImageData *outimg)
{
	int x,y;
	int x1,y1;
	int x2,y2;
	int rx1,ry1,rx2,ry2;
	Pixel col,bcol;

	double tx1,ty1,tx2,ty2;
	double xx,yy;
	double fx,fy;
	double aa,bb,cc,dd;

	double fox,foy;
	double tox,toy;
	double vx,vy;
	double theta,rr;
	double rate;
    int r1,g1,b1;
	double tmp,a2;
	double dx,dy,dz;

	x1=0;
	y1=0;
	x2=outimg->width - 1;
	y2=outimg->height - 1;

	rx1=0;
	ry1=0;
	rx2=img->width - 1;
	ry2=img->height - 1;

	tx1=(double)x1;
	ty1=(double)y1;
	tx2=(double)x2;
	ty2=(double)y2;

	aa=(ty2-ty1)/2.0;
	a2=aa*aa;
	bb=(tx2-tx1)/2.0;
	cc=(double)(ry2-ry1);
	cc=cc/2.0;
	dd=(double)(rx2-rx1);
	dd=dd/2.0;

	toy=(ty2+ty1)/2.0;
	tox=(tx2+tx1)/2.0;
	foy=((double)(ry2+ry1))/2.0;
	fox=((double)(rx2+rx1))/2.0;

	bcol.r=bcol.g=bcol.b=128;
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			xx=(double)x-tox;
			xx=aa*xx/bb;
			yy=(double)y-toy;
			rr=sqrt(xx*xx+yy*yy);
			if(rr<=aa) {
				vx=getVorg(yy,xx,aa);
				vy=getVorg(xx,yy,aa);
				fx=vx*dd+fox;
				fy=vy*cc+foy;
    			if(interpolate(img,fx,fy,&col)) {	
    				setPixel(outimg,x,y,&col);	
    			}
    			else {
    				setPixel(outimg,x,y,&bcol);	
    			}
    		}
    		else {
				setPixel(outimg,x,y,&bcol);	
    		}
		}
	}
}


int linarPol(int c1,int c2,int c3,int c4,double xr,double yr)
{
	double d,e,f;
	int res;

	d=(double)c1*(1.0-xr)+(double)c3*xr;
	e=(double)c2*(1.0-xr)+(double)c4*xr;
	f=d*(1.0-yr)+e*yr;
	res=(f+0.5);
	if(res>255) res=255;
	else if(res<0) res=0;
	return res;
}


int interpolate(ImageData *img,double x,double y,Pixel *col)
{
	int lx,ly;
	int val;
	int adr;
	int mx,my,dep;
	int px,py;
	double xrate,yrate;
	Pixel col1,col2,col3,col4;

	px=(int)x;
	py=(int)y;
	xrate=x-(double)px;
	yrate=y-(double)py;
	val=
	getPixel(img,px  ,py  ,&col1);
	getPixel(img,px  ,py+1,&col2);
	getPixel(img,px+1,py  ,&col3);
	getPixel(img,px+1,py+1,&col4);
	col->r=linarPol(col1.r,col2.r,col3.r,col4.r,xrate,yrate);
	col->g=linarPol(col1.g,col2.g,col3.g,col4.g,xrate,yrate);
	col->b=linarPol(col1.b,col2.b,col3.b,col4.b,xrate,yrate);
	return val;
}
