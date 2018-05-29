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
		printf("画像(1)が読めません");
		return;
	}
	res=readBMPfile(av[2],&img2);
	if(res<0) {
		printf("画像(2)が読めません");
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
	    	rgb3.r=blend(rgb1.r,rgb2.r,alpha);
	    	rgb3.g=blend(rgb1.g,rgb2.g,alpha);
	    	rgb3.b=blend(rgb1.b,rgb2.b,alpha);
	    	setPixel(img1,xx,yy,&rgb3);
	    }
    }
	writeBMPfile(av[3],img1);
	disposeImage(img1);
	disposeImage(img2);
}

#define CMAX 255
int blend(int c1,int c2,int a)
{
    int v;

    v= (c1*a+(CMAX-a)*c2)/CMAX;
    if(v<0) v=0;
    if(v>CMAX) v=CMAX;
    return v;
}
