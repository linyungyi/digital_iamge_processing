#include "image.h"
#include <stdlib.h>

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

int effect(ImageData *img,ImageData *outimg,int l_wid,int l_det,int cst,int gst)
{
	int val;
	int x,y;
	int ro,go,bo;
	int sum;
	int endn;
	int flag;
	int mitudo;
	Pixel col,ncol;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	for(y=y1;y<=y2;y++) {
		if((y%l_det)<l_wid) flag=TRUE;
		else flag=FALSE;
		for(x=x1;x<=x2;x++) {
			val = getPixel(img,x,y,&col);	
			val = getPixel(img,x+gst,y,&ncol);	
			ro= col.r;
			ro=(ro + ncol.r)/2;
			go= col.g;
			go=(go + ncol.g)/2;
			bo= col.b;
			bo=(bo + ncol.b)/2;
			if(flag) {
				ro+=(cst-127);
				go+=(cst-127);
				bo+=(cst-127);
			}
			col.r = ro;
			col.g = go;
			col.b = bo;
			setPixel(outimg,x,y,&col);	
		}
	}
	return TRUE;
}
