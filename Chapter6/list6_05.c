#include "image.h"
#include <stdlib.h>

main(int ac,char *av[])
{
    ImageData *img1,*img2;
    Pixel rgb1,rgb2,rgb3;
    int alpha;
    int rr,gg,bb,r1;
    int size;
    int xx,yy,mx,my;
    int i;
    int res;

	if(ac<3) return 4;
	res=readBMPfile(av[1],&img1);
	if(res<0) {
		printf("‰æ‘œ(1)‚ª“Ç‚ß‚Ü‚¹‚ñ");
		return;
	}
	res=readBMPfile(av[2],&img2);
	if(res<0) {
		printf("‰æ‘œ(2)‚ª“Ç‚ß‚Ü‚¹‚ñ");
		return;
	}
    mx=img2->width;
    my=img2->height;
    if(mx>img1->width) mx=img1->width;
    if(my>img1->height) my=img1->height;
    for(yy=0;yy<my;yy++) {
	    for(xx=0;xx<mx;xx++) {
	        alpha=(xx*255+mx/2)/mx;
	    	getPixel(img1,xx,yy,&rgb1);
	    	getPixel(img2,xx,yy,&rgb2);
	    	if(inRegion(xx,yy)==0) {
	    	    rgb3=rgb1;
	    	}
	    	else {
	    	    rgb3=rgb2;
	    	}
	    	setPixel(img1,xx,yy,&rgb3);
	    }
    }
	writeBMPfile(av[3],img1);
	disposeImage(img1);
	disposeImage(img2);
}

// —Ìˆæ‚Ì‰¡•
#define REGIONSIZE 10

int inRegion(int x,int y)
{
    int wx;

    wx=x/REGIONSIZE;
    return wx%2;
}