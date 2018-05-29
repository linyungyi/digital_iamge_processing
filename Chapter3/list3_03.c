#include "image.h"
#include <math.h>
#include <stdlib.h>

int robert1[9]={
	 0, 0, 0,
	 0, 1, 0,
	 0, 0,-1};
int robert2[9]={
	 0, 0, 0,
	 0, 0, 1,
	 0,-1, 0};

int absi(int i)
{
	if(i<0) return -i;
	return i;
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
	int x,y;
	int i;
	int val;
	int xx,yy;
	int rrx,ggx,bbx;
	int rry,ggy,bby;
	int rr,gg,bb;
	Pixel col;
	int sadr;
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

					rrx+= col.r*robert1[sadr];
					ggx+= col.g*robert1[sadr];
					bbx+= col.b*robert1[sadr];
					rry+= col.r*robert2[sadr];
					ggy+= col.g*robert2[sadr];
					bby+= col.b*robert2[sadr];
					sadr++;
				}
			}
			rrx=absi(rrx);
			ggx=absi(ggx);
			bbx=absi(bbx);
			rry=absi(rry);
			ggy=absi(ggy);
			bby=absi(bby);
			if(rrx>rry) rr=rrx;
			else rr=rry;
			if(ggx>ggy) gg=ggx;
			else gg=ggy;
			if(bbx>bby) bb=bbx;
			else bb=bby;
			col.r = rr;
			col.g = gg;
			col.b = bb;
			setPixel(outimg,x,y,&col);	
		}
	}
	return TRUE;
}
