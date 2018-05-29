#include "image.h"
#include <math.h>
#include <stdlib.h>

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

int absi(int i)
{
	if(i<0) return -i;
	return i;
}


int effect(ImageData *img,ImageData *outimg)
{
	int val;
	int x,y;
	int xx,yy;
	int i;
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
	int fil[5*5],para,fsam;
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

					rrx+= col.r*sobel1[sadr];
					ggx+= col.g*sobel1[sadr];
					bbx+= col.b*sobel1[sadr];
					rry+= col.r*sobel2[sadr];
					ggy+= col.g*sobel2[sadr];
					bby+= col.b*sobel2[sadr];
					sadr++;
				}
			}
			para=(int)(sqrt((double)(rrx*rrx+rry*rry))/8.0);
			para=30-para;
			if(para<0) para=0;
			for(i=0;i<25;i++) {
				fil[i]=para;
			}
			fil[2*5+2]=30;
			rr=gg=bb=0;
			fsam=0;
			sadr=0;
			for(yy= -2;yy<3;yy++) {
				for(xx= -2;xx<3;xx++) {
					val = getPixel(img,x+xx,y+yy,&col);	
“¾
					rr+= col.r*fil[sadr];
					gg+= col.g*fil[sadr];
					bb+= col.b*fil[sadr];
					fsam+=fil[sadr];
					sadr++;
				}
			}
			col.r = rr/fsam;
			col.g = gg/fsam;
			col.b = bb/fsam;
			setPixel(outimg,x,y,&col);	
		}
	}
	return TRUE;
}
