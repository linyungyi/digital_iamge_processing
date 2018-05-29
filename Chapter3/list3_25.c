#include "image.h"
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
	
	effect(img,outimg,atoi(av[3]));

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

int rnoise(int a,int b)
{
	int r;

	return rand()%a-b;
}

int effect(ImageData *img,ImageData *outimg,int noise)
{
	int val;
	int x,y;
	int ro,go,bo;
	int sum;
	int endn;
	int nnn;
	Pixel col;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	nnn=noise*2+1;
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			val = getPixel(img,x+rnoise(nnn,noise),y+rnoise(nnn,noise),&col);	“¾
			setPixel(outimg,x,y,&col);	
		}
	}
	return TRUE;
}
