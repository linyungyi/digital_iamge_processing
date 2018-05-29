#include "image.h"
#include <math.h>
#include <stdlib.h>


int fil1[9]={
	 1, 0,-1,
	 2, 0,-2,
	 1, 0,-1};
int fil2[9]={
	 1, 2, 1,
	 0, 0, 0,
	-1,-2,-1};

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

					rrx+= col.r*fil1[sadr];
					ggx+= col.g*fil1[sadr];
					bbx+= col.b*fil1[sadr];
					rry+= col.r*fil2[sadr];
					ggy+= col.g*fil2[sadr];
					bby+= col.b*fil2[sadr];
					sadr++;
				}
			}
			col.r=(int)(sqrt((double)(rrx*rrx+rry*rry))/8.0);
			col.g=(int)(sqrt((double)(ggx*ggx+ggy*ggy))/8.0);
			col.b=(int)(sqrt((double)(bbx*bbx+bby*bby))/8.0);
			setPixel(outimg,x,y,&col);	
		}
	}
	return TRUE;
}
