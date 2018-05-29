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



int transform(ImageData *img,ImageData *outimg)
{
	int x,y;
	int x1,y1;
	int x2,y2;
	int rx1,ry1,rx2,ry2;
	Pixel col;
	int h1,w1;
	int h2,w2;
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
	col.r=col.g=col.b=128;
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			fx=(x-x1)*w1/(double)w2+rx1;
			fy=(y-y1)*h1/(double)h2+ry1;
			if(interpolate(img,fx,fy,&col)) {	
				setPixel(outimg,x,y,&col);	
			}
		}
	}
}

double naiseki(double *x,double *y)
{
	int i;
	double res;

	res=0.0;
	for(i=0;i<4;i++) {
		res+=(x[i]*y[i]);
	}
	return res;
}

double cubicFunc(double x)
{
	double ax;
	double res;

	if(x<0) ax= -x;
	else ax=x;

	if(0.0 <= ax && ax < 1.0) {
		res=1.0 - 2.0*ax*ax + ax*ax*ax;
	}
	else if(1.0 <= ax && ax < 2.0) {
		res=4.0 - 8.0*ax +5.0*ax*ax - ax*ax*ax;
	}
	else {
		res=0.0;
	}
	return res;
}

int biCubic(double x1,double y1,int *buf)
{
	int i,j;
	double fx[4],fy[4];
	double tmp[4],mat[4];
	double dx,dy;
	int px,py;
	int res;

	px=(int)x1;
	py=(int)y1;
	dx=x1-(double)px;
	dy=y1-(double)py;

	fx[0]=cubicFunc(1.0+dx);
	fx[1]=cubicFunc(    dx);
	fx[2]=cubicFunc(1.0-dx);
	fx[3]=cubicFunc(2.0-dx);

	fy[0]=cubicFunc(1.0+dy);
	fy[1]=cubicFunc(    dy);
	fy[2]=cubicFunc(1.0-dy);
	fy[3]=cubicFunc(2.0-dy);

	for(i=0;i<4;i++) {
		for(j=0;j<4;j++) {
			mat[j]=(double)buf[j*4+i];
		}
		tmp[i]=naiseki(fy,mat);
	}
	res=(int)naiseki(tmp,fx);
	if(res<0) res=0;
	else if(res>255) res=255;
	return res;
}

int interpolate(ImageData *img,double x1,double y1,Pixel *pix)
{
	int px,py;
	int val;
	int adr2;
	int r1,g1,b1,o1;
	int lx2,ly2;
	Pixel col2;
	int bufR[16],bufG[16],bufB[16];

	px=(int)x1;
	py=(int)y1;

	for(ly2=0;ly2<4;ly2++) {
		adr2=ly2*4;
		for(lx2=0;lx2<4;lx2++) {
			val = getPixel(img,px+lx2-1,py+ly2-1,&col2);	
			bufR[adr2]=col2.r;
			bufG[adr2]=col2.g;
			bufB[adr2]=col2.b;
			adr2++;
		}
	}
	r1=biCubic(x1,y1,bufR);
	g1=biCubic(x1,y1,bufG);
	b1=biCubic(x1,y1,bufB);

	pix->r=r1;
	pix->g=g1;
	pix->b=b1;
	return TRUE;
}
