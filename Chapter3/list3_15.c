#include "image.h"
#include <math.h>
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<4) {
		printf("ƒp??[ƒ^‚ª‘«‚è‚Ü‚¹‚ñ");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("‰æ?‚ª“Ç‚ß‚Ü‚¹‚ñ");
		return;
	}


	outimg=createImage(img->width,img->height,24);
	
	effect(img,outimg,atof(av[3]));

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

#define NN 17
#define NF 8

int effect(ImageData *img,ImageData *outimg,double ef)
{
	int val;
	double rate;
	int i;
	int x,y;
	int xx,yy;
	int rr,gg,bb,oo;
	int endn;
	Pixel col,pixb,ncol;
	int mx;
	int pat_sum,pat;
	double rad,dis,disI;
	double ox,oy;
	double disMAX;
	double dx,dy;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	ox=(double)(x2-x1)/2.0;
	oy=(double)(y2-y1)/2.0;
	dx=(double)x2-ox;
	dy=(double)y2-oy;
	disMAX=sqrt(dx*dx+dy*dy);

	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			rr=gg=bb=oo=0;
			dx=(double)x-ox;
			dy=(double)y-oy;
			if(dx!=0.0) rad=atan(dy/dx);
			else rad=3.14159265/2.0;
			rad+=(3.14159265/2.0);
			dis=sqrt(dx*dx+dy*dy);
			rate=ef*dis/disMAX;
			rate/=((double)NF);
			pat_sum=0;
			for(i=0;i<NN;i++) {
				if(i==NF) pat=3;
				else pat=1;
				disI=(double)(i-NF)*rate;
				xx=(int)(disI*cos(rad))+x;
				yy=(int)(disI*sin(rad))+y;
				val = getPixel(img,xx,yy,&ncol);	
				rr+=ncol.r * pat;
				gg+=ncol.g * pat;
				bb+=ncol.b * pat;
				oo+= pat;
				pat_sum+=pat;
			}
			col.r = rr/(oo);
			col.g = gg/(oo);
			col.b = bb/(oo);
			setPixel(outimg,x,y,&col);	
		}
	}
	return TRUE;
}
