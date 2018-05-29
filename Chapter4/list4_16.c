
#include "image.h"
#include <math.h>
#include <stdlib.h>
#define Pai 3.14159265358

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int nx,ny;
	int ox,oy;
	int x1,y1;
	double rate;

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


	ox=atoi(av[5]);
	oy=atoi(av[6]);

	x1=atoi(av[7]);
	y1=atoi(av[8]);

	sscanf(av[9],"%lf",&rate);

	outimg=createImage(nx,ny,24);

	transform(img,outimg,ox,oy,x1,y1,rate);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

double getAngle(double dx,double dy)
{
	double th;

	if(dy==0.0) {
		if(dx<0.0)
			th=-Pai/2.0;
		else
			th= Pai/2.0;
	}
	else {
		th=atan(dx/-dy);
		if(dy>0.0) {
			if(dx==0.0) th=Pai;
			if(dx<0.0) th= th-Pai;
			if(dx>0.0) th= th+Pai;
		}
	}
	return th;
}

double getAngleP(int x1,int y1,int ox,int oy)
{
	double dx,dy;

	dx=(double)(x1-ox);
	dy=(double)(y1-oy);
	return getAngle(dx,dy);
}

double getR(int x,int y,int x1,int y1)
{
	double dx,dy;

	dx=(double)(x-x1);
	dy=(double)(y-y1);
	return sqrt(dx*dx+dy*dy);
}

int minVal(int n1,int n2,int n3)
{
	int res;

	if(n1<n2) res=n1;
	else res=n2;
	if(n3<res) res=n3;

	return res;
}

int maxVal(int n1,int n2,int n3)
{
	int res;

	if(n1>n2) res=n1;
	else res=n2;
	if(n3>res) res=n3;

	return res;
}

int transform(ImageData *img,ImageData *outimg,int ox,int oy,int xx,int yy,double rate_r)
{
	int x,y;
	int x1,y1;
	int x2,y2;
	int x3,y3;
	int sx,sy,ex,ey;
	double tth2;
	int rx1,ry1,rx2,ry2;
	Pixel col,bcol;
	double h1,w1;
	double fx,fy;
	double th,R;
	double th_rate;
    double rrr,th1,th2;

	rx1=0;
	ry1=0;
	rx2=img->width - 1;
	ry2=img->height - 1;

    x1=xx;  y1=yy;
    x2=ox+(ox-xx);
    y2=yy;
    x3=ox;  y3=oy;

	rrr=getR(x1,y1,ox,oy);
	th1=getAngleP(x1,y1,ox,oy);
	th2=getAngleP(x2,y2,ox,oy);


	sx=minVal(x1,x2,x3);
	sy=minVal(y1,y2,y3);
	ex=maxVal(x1,x2,x3);
	ey=maxVal(y1,y2,y3);
	if(th2<th1) tth2=th2+(Pai*2);
	else tth2=th2;
	th_rate=tth2-th1;
	if(th1<0 && tth2>0) {
		sy=y3-(int)(rrr+0.9);
	}
	if(tth2>Pai*2.0) {
		sy=y3-(int)(rrr+0.9);
	}
	if(th1<(Pai/2.0) && tth2>(Pai/2.0)) {
		ex=x3+(int)(rrr+0.9);
	}
	if(tth2>(Pai*5.0/2.0)) {
		ex=x3+(int)(rrr+0.9);
	}
	if(th1<Pai && tth2>Pai) {
		ey=y3+(int)(rrr+0.9);
	}
	if(tth2>Pai) {
		ey=y3+(int)(rrr+0.9);
	}
	if(th1<-(Pai/2.0) && tth2>-(Pai/2.0)) {
		sx=x3-(int)(rrr+0.9);
	}
	if(tth2>(Pai*3.0/2.0)) {
		sx=x3-(int)(rrr+0.9);
	}
	w1=(double)(rx2-rx1+1);
	h1=(double)(ry2-ry1+1);
	bcol.r=bcol.g=bcol.b=128;
	for(y=sy;y<=ey;y++) {
		for(x=sx;x<=ex;x++) {
			th=getAngle((double)(x-x3),(double)(y-y3));
			if((th1 >0.0 || tth2 >0.0) && th<th1) th+=(Pai*2);
			th-=th1;
			th/=th_rate;
			R=getR(x,y,ox,oy);
			R/=rrr;
			R-=rate_r;
			R/=(1.0-rate_r);
			fx=th*w1+(double)rx1;
			fy=(1.0-R)*h1+(double)ry1;
			if(interpolate(img,fx,fy,&col)) {	
				setPixel(outimg,x,y,&col);	
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
