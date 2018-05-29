#include "image.h"
#include <math.h>
#include <stdlib.h>

int fil1[9]={
	 1, 1, 1,
	 1,-2, 1,
	-1,-1,-1};
int fil2[9]={
	 1, 1, 1,
	 1,-2,-1,
	 1,-1,-1};
int fil3[9]={
	 1, 1,-1,
	 1,-2,-1,
	 1, 1,-1};
int fil4[9]={
	 1,-1,-1,
	 1,-2,-1,
	 1, 1, 1};
int fil5[9]={
	-1,-1,-1,
	 1,-2, 1,
	 1, 1, 1};
int fil6[9]={
	-1,-1, 1,
	-1,-2, 1,
	 1, 1, 1};
int fil7[9]={
	-1, 1, 1,
	-1,-2, 1,
	-1, 1, 1};
int fil8[9]={
	 1, 1, 1,
	-1,-2, 1,
	-1,-1, 1};

int absi(int i)
{
	if(i<0) return -i;
	return i;
}

int* getFilter(int i)
{
	switch(i) {
	case 0:
		return fil1;
	case 1:
		return fil2;
	case 2:
		return fil3;
	case 3:
		return fil4;
	case 4:
		return fil5;
	case 5:
		return fil6;
	case 6:
		return fil7;
	case 7:
		return fil8;
	}
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
	int i;
	int val;
	int x,y;
	int xx,yy;
	int hh;
	int du,dd,dl,dr;
	int endn;
	int c1,c2;
	int rrx,ggx,bbx;
	int rr,gg,bb;
	Pixel col;
	int *fil;
	int sadr;
	int rate,vh;
	int maxR,maxG,maxB;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			maxR=maxG=maxB= -10000;
			for(i=0;i<8;i++) {
				fil=getFilter(i);
				rrx=ggx=bbx=0;
				sadr=0;
				for(yy=0;yy<3;yy++) {
					for(xx=0;xx<3;xx++) {
						val = getPixel(img,x+xx-1,y+yy-1,&col);	
						rrx+= col.r*fil[sadr];
						ggx+= col.g*fil[sadr];
						bbx+= col.b*fil[sadr];
						sadr++;
					}
				}
				if(maxR<rrx) maxR=rrx;
				if(maxG<ggx) maxG=ggx;
				if(maxB<bbx) maxB=bbx;
			}
			maxR/=10;
			maxG/=10;
			maxB/=10;
			col.r = maxR;
			col.g = maxG;
			col.b = maxB;
			setPixel(outimg,x,y,&col);	
		}
	}
	return TRUE;
}
