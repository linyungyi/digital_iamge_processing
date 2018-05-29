#include "image.h"
#include <math.h>
#include <stdlib.h>

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
	//double rad;
	double a,b,c,d,e,f;
	int res;

	if(ac<3+2+6) {
		printf("パ??ータが足りません");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("画?が読めません");
		return;
	}
	sscanf(av[5],"%lf",&a);
	sscanf(av[6],"%lf",&b);
	sscanf(av[7],"%lf",&c);
	sscanf(av[8],"%lf",&d);
	sscanf(av[9],"%lf",&e);
	sscanf(av[10],"%lf",&f);



	transform(img,&outimg,atoi(av[3]),atoi(av[4]),a,b,c,d,e,f);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}


int transform(ImageData *img,ImageData **outimg_p,int wt,int ht,double a,double b,double c,double d,double e,double f)
{
	ImageData *outimg;
	int x,y;
	double x1,y1;
	double x2,y2;
	Pixel col,bcol;
	int h1,w1;
	int h2,w2;
	int tx,ty;
	double xx,yy;
	double px[4],py[4];
	int i;

	int dx,dy;
	double n,m;
	double det;

	*outimg_p=createImage(wt,ht,24);
	outimg=(*outimg_p);

	bcol.r=bcol.g=bcol.b=128;
	for(y=0;y<ht;y++) {			
		for(x=0;x<wt;x++) {
		    tx=(double)(x);
		    ty=(double)(y);
		    xx=(a*tx+b*ty+c);
		    yy=(d*tx+e*ty+f);
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
