
#include "image.h"
#include <math.h>
#include <stdlib.h>

#define PAI 3.1415926

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
		printf("パ??ータが足りません");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("画?が読めません");
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



int transform(ImageData *img,ImageData *outimg)
{
	int x,y;
	int x1,y1;
	int x2,y2;
	int rx1,ry1,rx2,ry2;
	Pixel col;
	int h1,w1;
	int h2,w2;
	double theta;
	double ro;
	double wo;
	double ww;
	double fx,fy;

	x1=0;
	y1=0;
	x2=outimg->width - 1;
	y2=outimg->height - 1;

	rx1=0;
	ry1=0;
	rx2=img->width - 1;
	ry2=img->height - 1;


	w1=rx2-rx1+1;
	h1=ry2-ry1+1;
	w2=x2-x1+1;
	h2=y2-y1+1;
	ro=(double)w2/2.0;
	col.r=col.g=col.b=128;
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			ww=(double)(x-x1);
			if(ro-ww != 0.0)
				theta=atan(sqrt(2.0*ro*ww-ww*ww)/(ro-ww));
			else
				theta=PAI/2.0;
			if(theta<0.0) theta+=PAI;
			fx=(theta*(double)w1)/PAI+rx1;
			fy=(y-y1)*h1/(double)h2+ry1;
			if(interpolate(img,fx,fy,&col)) {	
				setPixel(outimg,x,y,&col);	
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

 線形補間法
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
