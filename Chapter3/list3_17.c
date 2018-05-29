#include "image.h"
#include <stdlib.h>
#include <math.h>

#define PAI2 (double)(3.14159*2.0)

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<7) {
		printf("ƒp??[ƒ^‚ª‘«‚è‚Ü‚¹‚ñ");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("‰æ?‚ª“Ç‚ß‚Ü‚¹‚ñ");
		return;
	}


	outimg=createImage(img->width,img->height,24);
	
	effect(img,outimg,atoi(av[3]),atoi(av[4]),atoi(av[5]),atoi(av[6]));

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

double getRefOrg(double dx,double dz,double h,double ref)
{
	double sinA,tanA;
	double sinB,tanB;
	double rA;
	double res;

	rA=sqrt(dx*dx+dz*dz);
	sinA= -dx/rA;
	tanA= -dx/dz;
	sinB=ref*sinA;
	tanB=sqrt(1.0/(1.0-sinB*sinB)-1.0);
	if(dx>0.0) tanB= -tanB;
	res=(tanA-tanB)/(1.0+tanA*tanB);
	return h*res;
}

int effect(ImageData *img,ImageData *outimg,int ampint ,int wwint ,int refint,int vert)
{
	int val;
	int x,y;
	int xx,yy;
	int rr,gg,bb,oo;
	int endn;
	Pixel col,pixb,ncol;
	int rate;
	int smx;
	double amp,ww;
	int tmp;
	int adr;
	int *mask_pat,mask_sum,max_val;
	int gotu_do;
	int *gotu;
	int bx,by;
	double dx,dy,dy2,dz;
	double ref,ddx,ddy;
	double kH;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	amp=(double)ampint/100.0;
	ww=(double)wwint;
	vert=atoi(PMsGetParameter(PARAM_EFFECT,2));
	ref=(double)refint/100.0;
	adjustLightSourse();
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			if(!vert) {
				dx= 0.0;
				dy= -PAI2/ww*amp*cos(PAI2/ww*(double)(y-y1));
				dz= 1.0;
				kH= amp*(sin(PAI2/ww*(double)(y-y1))+1.0)+10.0;
			}
			else {
				dx= -PAI2/ww*amp*cos(PAI2/ww*(double)(x-x1));
				dy= 0;
				dz= 1.0;
				kH= amp*(sin(PAI2/ww*(double)(x-x1))+1.0)+10.0;
			}
			ddx=getRefOrg(dx,dz,kH,ref);
			ddy=getRefOrg(dy,dz,kH,ref);
			val = getPixel(img,(int)(x+ddx),(int)(y+ddy),&col);	
			rr=lighting(col.r,dx,dy,dz,(double)(x-x1),(double)(y-y1),0.0);
			gg=lighting(col.g,dx,dy,dz,(double)(x-x1),(double)(y-y1),0.0);
			bb=lighting(col.b,dx,dy,dz,(double)(x-x1),(double)(y-y1),0.0);
			col.r = rr;
			col.g = gg;
			col.b = bb;
			setPixel(outimg,x,y,&col);	
		}
	}
	return TRUE;
}
