#include "image.h"
#include <stdlib.h>
#include <math.h>


main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<3) {
		printf("ƒp??[ƒ^‚ª‘«‚è‚Ü‚¹‚ñ");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("‰æ?‚ª“Ç‚ß‚Ü‚¹‚ñ");
		return;
	}


	outimg=createImage(img->width,img->height,24);
	
	effect(img,outimg );

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

int effect(ImageData *img,ImageData *outimg)
{
	int val;
	int sobel1[9]={
		 1, 0,-1,
		 2, 0,-2,
		 1, 0,-1};
	int sobel2[9]={
		 1, 2, 1,
		 0, 0, 0,
		-1,-2,-1};
	int x,y;
	int xx,yy;
	double dx,dy,dz;
	int hh;
	int du,dd,dl,dr;
	int endn;
	int c1,c2;
	int rrx,ggx,bbx;
	int rry,ggy,bby;
	int rr,gg,bb;
	Pixel col,ncol;
	int *sobel;
	int sadr;
	int rate,vh;
	int gray;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	adjustLightSourse();
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			rrx=ggx=bbx=0;
			rry=ggy=bby=0;
			sadr=0;
			for(yy=0;yy<3;yy++) {
				for(xx=0;xx<3;xx++) {
					val = getPixel(img,x+xx-1,y+yy-1,&col);	“¾
					rr=col.r;
					gg=col.g;
					bb=col.b;
					gray=(bb*28+77*rr+gg*151)/256;
					rrx+= gray*sobel1[sadr];
					rry+= gray*sobel2[sadr];
					sadr++;
				}
			}
			dx=(double)rrx;
			dy=(double)rry;
			dz= 16.0;
			val = getPixel(img,x+xx-1,y+yy-1,&col);	
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
