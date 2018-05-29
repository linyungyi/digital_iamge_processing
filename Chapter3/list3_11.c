#include "image.h"
#include <math.h>
#include <stdlib.h>

int absi(int x)
{
	if(x<0) return -x;
	return x;
}

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
	
	effect(img,outimg);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}



int effect(ImageData *img,ImageData *outimg)
{
	int val;
	int x,y;
	int xx,yy;
	int rr,gg,bb,oo;
	Pixel col,ncol;
	int rate;
	int sumr,sumg,sumb;
	int smx;
	int ef,sq;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	for(y=y1;y<=y2;y++) {
		x=x1;
		for(x=x1;x<=x2;x++) {
			sumr=sumg=sumb=0;
			for(xx= -2;xx<=2;xx++) {
				val = getPixel(img,x+xx,y,&ncol);	
				sumr+=ncol.r;
				sumg+=ncol.g;
				sumb+=ncol.b;
			}
			col.r=sumr/5;
			col.g=sumg/5;
			col.b=sumb/5;
			setPixel(outimg,x,y,&col);	
		}
	}
	return TRUE;
}
