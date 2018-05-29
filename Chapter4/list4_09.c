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
	double xgap,ygap;
	int res;

	if(ac<5) {
		printf("パ??ータが足りません");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("画?が読めません");
		return;
	}
	sscanf(av[3],"%lf",&xgap);
	sscanf(av[4],"%lf",&ygap);

	transform(img,&outimg,xgap,ygap);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}


int transform(ImageData *img,ImageData **outimg_p,double xgap,double ygap)
{
	ImageData *outimg;
	int x,y;
	double x1,y1;
	double x2,y2;
	int wt,ht;
	Pixel col,bcol;
	int h1,w1;
	int h2,w2;
	int tx,ty;
	double xx,yy;
	double px[4],py[4];
	int i;

	int dx,dy;
	double n,m;
	double aa,bb,cc,dd;
	double det;

	px[0]=0;
	py[0]=0;

	px[1]=(double)(img->width - 1);
	py[1]=0;

	px[2]=(double)(img->width - 1);
	py[2]=(double)(img->height - 1);

	px[3]=0;
	py[3]=(double)(img->height - 1);

	x1=px[0];	y1=py[0];
	x2=px[2]+xgap;	y2=py[2]+ygap;

	wt=(int)(x2-x1+0.5);
	ht=(int)(y2-y1+0.5);

	*outimg_p=createImage(wt,ht,24);
	outimg=(*outimg_p);

    n=ygap/px[2];
    m=xgap/py[2];

    aa=1.0;
    bb= -m;
    cc= -n;
    dd=1.0;
    det=aa*dd-bb*cc;

	bcol.r=bcol.g=bcol.b=128;
	for(y=0;y<ht;y++) {			
		for(x=0;x<wt;x++) {
		    tx=(double)(x);
		    ty=(double)(y);
		    xx=(aa*tx+bb*ty)/det;
		    yy=(cc*tx+dd*ty)/det;
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
