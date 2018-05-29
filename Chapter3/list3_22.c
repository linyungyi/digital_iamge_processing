#include "image.h"
#include <math.h>
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<6) {
		printf("ƒp??[ƒ^‚ª‘«‚è‚Ü‚¹‚ñ");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("‰æ?‚ª“Ç‚ß‚Ü‚¹‚ñ");
		return;
	}


	outimg=createImage(img->width,img->height,24);
	
	effect(img,outimg,atoi(av[3]),atoi(av[4]),atoi(av[5]));

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

int absi(int i)
{
	if(i<0) return -i;
	return i;
}

int effect(ImageData *img,ImageData *outimg,int r1,int g1,int b1)
{
	int val;
	int x,y;
	int xx,yy;
	int hh;
	int du,dd,dl,dr;
	int rrx,ggx,bbx;
	int rry,ggy,bby;
	int rr,gg,bb,gray;
	int rate;
	Pixel col;
	int *sobel;
	int sadr;
	int sobel1[9]={
		 1, 0,-1,
		 2, 0,-2,
		 1, 0,-1};
	int sobel2[9]={
		 1, 2, 1,
		 0, 0, 0,
		-1,-2,-1};
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

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
			rate=(int)(sqrt((double)(rrx*rrx+rry*rry)));
			if(rate>1000) rate=1000;
			val = getPixel(img,x,y,&col);	
			rr=col.r;
			gg=col.g;
			bb=col.b;
			col.r = (r1*rate+rr*(1000-rate))/1000;
			col.g = (g1*rate+gg*(1000-rate))/1000;
			col.b = (b1*rate+bb*(1000-rate))/1000;
			setPixel(outimg,x,y,&col);	
		}
	}
	return TRUE;
}
