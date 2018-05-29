#include "image.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PAI   3.1415926

#define WSIZE 27

void setMaskValue(double *G,int msize,int i,int j,double val)
{
	G[i*msize+j]=val;
}

void makeGauss(double scale,int msize,double *Gx,double *Gy,double *Gxy,double *Gxx,double *Gyy)
{
    int    i,j,k,m,skip;
    int    x0,y0,value ;
    int    vmin,vmax,range,val;
    double vx,vy,vxy,vxx,vyy,r0,exponent,dvalue;

    x0 = y0 = WSIZE/2;
    for(i=0; i<msize; i++){
    	for(j=0; j < msize; j++){
    	    r0 = (double)((i-y0)*(i-y0)+(j-x0)*(j-x0));

		    exponent = 1.0/(2.0*PAI*scale) * exp(-r0/(2.0*scale));
		    vx  = (-(double)(j-x0)/scale) * exponent;
		    vy  = (-(double)(i-y0)/scale) * exponent;
		    vxy = ((double)((i-y0)*(j-x0))/(scale*scale)) * exponent;
		    vxx = ((double)(j-x0)*(j-x0)/(scale*scale)-1.0/scale) * exponent;
		    vyy = ((double)(i-y0)*(i-y0)/(scale*scale)-1.0/scale) * exponent;

		    setMaskValue(Gx,msize,i,j,vx);
		    setMaskValue(Gy,msize,i,j,vy);
		    setMaskValue(Gxy,msize,i,j,vxy);
		    setMaskValue(Gxx,msize,i,j,vxx);
		    setMaskValue(Gyy,msize,i,j,vyy);
    	}
    }
}

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	double scale,th;

	if(ac<5) {
		printf("ƒp??[ƒ^‚ª‘«‚è‚Ü‚¹‚ñ");
		return;
	}

	sscanf(av[3],"%lf",&scale);
	sscanf(av[4],"%lf",&th);


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("‰æ?‚ª“Ç‚ß‚Ü‚¹‚ñ");
		return;
	}


	outimg=createImage(img->width,img->height,24);

	effect(img,outimg,scale,th,WSIZE);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}


int effect(ImageData *img,ImageData *outimg,double scale,double th,int msize)
{
	int x,y;
	int i;
	int val;
	int xx,yy;
	int wt,ht;
	int xo,yo;
	int rrx,ggx,bbx;
	int rry,ggy,bby;
	int rr,gg,bb,gray;
	Pixel col;
	int sadr,adr;
	int x1,y1,x2,y2;

	double Lx,Ly,Lxy,Lxx,Lyy;
	double *lap,*edge;
	double *Gx,*Gy,*Gxy,*Gxx,*Gyy;

	Gx=(double*)malloc(sizeof(double)*msize*msize);
	Gy=(double*)malloc(sizeof(double)*msize*msize);
	Gxy=(double*)malloc(sizeof(double)*msize*msize);
	Gxx=(double*)malloc(sizeof(double)*msize*msize);
	Gyy=(double*)malloc(sizeof(double)*msize*msize);

	makeGauss(scale,msize,Gx,Gy,Gxy,Gxx,Gyy);

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;


	xo = yo = WSIZE/2;

	wt=x2-x1+1;
	ht=y2-y1+1;
	lap = (double *)malloc(wt*ht*sizeof(double));
	edge = (double *)malloc(wt*ht*sizeof(double));

	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			Lx=Ly=Lxy=Lxx=Lyy=0.0;
			for(yy=0;yy<msize;yy++) {
				for(xx=0;xx<msize;xx++) {
					val = getPixel(img,x+xx-xo,y+yy-yo,&col);	“¾

					rr=col.r;
					gg=col.g;
					bb=col.b;
					gray=(bb*28+77*rr+gg*151)/256;

					Lx  += (double)gray * Gx[yy*msize+xx];
					Ly  += (double)gray * Gy[yy*msize+xx];
					Lxy += (double)gray * Gxy[yy*msize+xx];
					Lxx += (double)gray * Gxx[yy*msize+xx];
					Lyy += (double)gray * Gyy[yy*msize+xx];

				}
			}
			lap[x+y*wt] =(Lx*Lx*Lxx+2.0*Lxy*Lx*Ly+Ly*Ly*Lyy);
			edge[x+y*wt] =sqrt(Lx*Lx+Ly*Ly);
		}
	}
	for(y=y1;y<=y2-1;y++) {
		for(x=x1;x<=x2-1;x++) {
			adr=x+y*wt;
			col.r=0;
			col.g=0;
			col.b=0;

			if(edge[adr]>=th && (lap[adr]*lap[adr+1]<0.0 || lap[adr]*lap[adr+wt]<0.0) ) {
				col.r=255;
				col.g=255;
				col.b=255;
			}
			setPixel(outimg,x,y,&col);	
		}
	}
	free(lap);
	free(edge);

	return TRUE;

}
