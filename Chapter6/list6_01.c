#include "image.h"
#include <stdlib.h>

main(int ac,char *av[])
{
    ImageData *img1,*img2;
    Pixel rgb1,rgb2,rgb3;
    int res;
    int alpha;
    int rr,gg,bb,r1;
    int size;
    int xx,yy,mx,my;
    int i;

	if(ac<7) return 4;
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
    rr=atoi(av[3]);
    gg=atoi(av[4]);
    bb=atoi(av[5]);
    r1=atoi(av[6]);
    mx=img1->width;
    my=img1->height;
    if(mx>img1->width) mx=img1->width;
    if(my>img1->height) my=img1->height;
    for(yy=0;yy<my;yy++) {
	    for(xx=0;xx<mx;xx++) {
	    	getPixel(img1,xx,yy,&rgb1);
	    	getPixel(img2,xx,yy,&rgb2);
	    	if(inArea(&rgb2,rr,gg,bb,r1)) {
	    	    rgb3=rgb1;
	    	}
	    	else {
	    	    rgb3=rgb2;
	    	}
	    	setPixel(img1,xx,yy,&rgb3);
	    }
    }
	writeBMPfile(av[7],img1);
	disposeImage(img1);
	disposeImage(img2);
}

int inArea(Pixel *rgb,int rr,int gg,int bb,int r)
{
    int dr,dg,db;
    int v1,v2;

    dr=(rgb->r - rr);
    dg=(rgb->g - gg);
    db=(rgb->b - bb);
    v1=dr*dr+dg*dg+db*db;
    v2=r*r;
    if(v1<=v2) return TRUE;
    return FALSE;
}
