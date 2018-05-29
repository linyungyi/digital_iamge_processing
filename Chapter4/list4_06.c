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
	int ox,oy;
	double fx1,fy1,fx2,fy2;
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
	ox=0;
	oy=0;
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			fx1=(x-x1)*w1/(double)w2+rx1;
			fy1=(y-y1)*h1/(double)h2+ry1;
			fx2=(x+1-x1)*w1/(double)w2+rx1;
			fy2=(y+1-y1)*h1/(double)h2+ry1;
			toSmall(img,fx1,fy1,fx2,fy2,&col);		
			setPixel(outimg,x,y,&col);	
		}
	}
}


int toSmall(ImageData *img,double x1,double y1,double x2,double y2,Pixel *res)
{
	int sx,sy,ex,ey;
	int x,y;
	int val;
	double e1,e2;
	double ph,pw;
	double rr,gg,bb,oo;
	double s;			
	double ss;
	Pixel from;


	sx=(int)x1;
	sy=(int)y1;
	ex=(int)x2;
	ey=(int)y2;
	if((double)ex!=x2) ex++;
	if((double)ey!=y2) ey++;
	s=0.0;
	rr=gg=bb=oo=0.0;
	for(y=sy;y<ey;y++) {

		e1=(double)y;
		e2=(double)(y+1);
		if(e1<y1) e1=y1;	
		if(e2>y2) e2=y2;	
		ph=e2-e1;
		for(x=sx;x<ex;x++) {
			val = getPixel(img,x,y,&from);	
			e1=(double)x;
			e2=(double)(x+1);
			if(e1<x1) e1=x1;	
			if(e2>x2) e2=x2;	
			pw=e2-e1;
			ss=pw*ph;
			s+=ss;
			rr+=(double)(from.r)*ss;
			gg+=(double)(from.g)*ss;
			bb+=(double)(from.b)*ss;
		}
	}
	if(s==0.0) {
		return FALSE;
	}
	rr/=s;
	gg/=s;
	bb/=s;
	oo/=s;
	res->r=(unsigned char)(rr+.5);
	res->g=(unsigned char)(gg+.5);
	res->b=(unsigned char)(bb+.5);
	return TRUE;
}
