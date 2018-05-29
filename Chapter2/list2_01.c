#include "image.h"
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<4) {
		printf("		");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("		");
		return;
	}


	outimg=createImage(img->width,img->height,24);
	
	effect(img,outimg,atoi(av[3]),atoi(av[4]));

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}


int effect(ImageData *img,ImageData *outimg,int param1,int param2)
{
	int x,y;
	int i;
	int val;
	Pixel pix;

	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&pix);	
			pix.r = ((param2-param1)*pix.r)/255+param1;
			pix.g = ((param2-param1)*pix.g)/255+param1;
			pix.b = ((param2-param1)*pix.b)/255+param1;
			setPixel(outimg,x,y,&pix);	
		}
	}
	return 0;
}
