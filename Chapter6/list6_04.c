#include "image.h"
#include <stdlib.h>
#include <math.h>

main(int ac,char *av[])
{
    ImageData *img1,*img2;
    Pixel rgb1,rgb2,rgb3;
    int alpha;
    int rr,gg,bb,r1,r2;
    int size;
    int xx,yy,mx,my;
    int i,res;

	if(ac<8) return 4;
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
    rr=atoi(av[3]);
    gg=atoi(av[4]);
    bb=atoi(av[5]);
    r1=atoi(av[6]);
    r2=atoi(av[7]);
    printf("READ!\n");
    mx=img2->width;
    my=img2->height;
    if(mx>img1->width) mx=img1->width;
    if(my>img1->height) my=img1->height;
    for(yy=0;yy<my;yy++) {
	    for(xx=0;xx<mx;xx++) {
	    	getPixel(img1,xx,yy,&rgb1);
	    	getPixel(img2,xx,yy,&rgb2);
	    	alpha=getAlpha(&rgb2,rr,gg,bb,r1,r2);
	    	rgb3.r=blend(rgb1.r,rgb2.r,alpha);
	    	rgb3.g=blend(rgb1.g,rgb2.g,alpha);
	    	rgb3.b=blend(rgb1.b,rgb2.b,alpha);
	    	setPixel(img1,xx,yy,&rgb3);
	    }
    }
	writeBMPfile(av[8],img1);
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

int getAlpha(Pixel *rgb,int rr,int gg,int bb,int r1,int r2)
{
    int dr,dg,db;
    int v1,v2;
    double vv;

    dr=(rgb->r - rr);
    dg=(rgb->g - gg);
    db=(rgb->b - bb);
    v1=dr*dr+dg*dg+db*db;
    vv=sqrt((double)v1);
    if(vv<=(double)r1) return CMAX;
    if(vv>=(double)r2) return 0;
    return (int)((double)CMAX*(vv-(double)r2)/(double)(r1-r2));
}
