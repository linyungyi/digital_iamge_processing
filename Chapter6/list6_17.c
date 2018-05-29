#include "image.h"
#include <stdlib.h>
#include <math.h>

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

	if(ac<4) return 4;
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
	    	getPixel(img1,xx,yy,&rgb1);
	    	getPixel(img2,xx,yy,&rgb2);
	    	func(&rgb1,&rgb2,&rgb3);
	    	setPixel(img1,xx,yy,&rgb3);
	    }
    }
	writeBMPfile(av[3],img1);
	disposeImage(img1);
	disposeImage(img2);
}

int func(Pixel *c1,Pixel *c2,Pixel *c3)
{
    int h1,s1,v1;
    int h2,s2,v2;
    int rr,gg,bb;

    RGBtoHSV(c1->r,c1->g,c1->b,&h1,&s1,&v1);
    RGBtoHSV(c2->r,c2->g,c2->b,&h2,&s2,&v2);

    HSVtoRGB(h2,s1,v1,&rr,&gg,&bb);
    c3->r=rr;
    c3->g=gg;
    c3->b=bb;

}

#define SCALE 255.0
#define hSCALE 256.0
#define GETA 2.0
#define hGETA 2.0

#define sgn(x) ((x)<0 ? (-1.0):(1.0))
#define CINT(x)  ((int)((x)+sgn(x)*0.5))
#define FL float

int RGBtoHSV(int r,int g,int b,int *h,int *s,int *v)
{
	FL rr,gg,bb;
	FL hh,ss,vv;
	FL cmax,cmin,cdes;

	rr=(FL)r/*/SCALE*/;
	gg=(FL)g/*/SCALE*/;
	bb=(FL)b/*/SCALE*/;

	if(rr>gg) cmax=rr;
	else cmax=gg;
	if(bb>cmax) cmax=bb;

	if(rr<gg) cmin=rr;
	else cmin=gg;
	if(bb<cmin) cmin=bb;

	cdes=cmax-cmin;
	vv=cmax;
	if(cdes!=0.0) {
		ss=cdes*SCALE/cmax;
		if(cmax==rr) {
			hh=(gg-bb)*SCALE/cdes;
		}
		else if(cmax==gg) {
			hh=(bb-rr)*SCALE/cdes+2.0*hSCALE;
		}
		else {
			hh=(rr-gg)*SCALE/cdes+4.0*hSCALE;
		}
	}
	else if(cmax!=0.0) {
		ss=cdes*SCALE/cmax;
		hh=0.0;
	}
	else {
		ss=0.0;
		hh=0.0;
	}
	if(hh<0.0) hh+=6.0*hSCALE;

	*h=CINT(hh*hGETA);
	*s=CINT(ss*GETA);
	*v=CINT(vv*GETA);
}

int HSVtoRGB(int h,int s,int v,int *r,int *g,int *b)
{
	FL hh,ss,vv;
	FL rr,gg,bb;

	if(h==6*hGETA*(hSCALE)) h=0;
	hh=(FL)h/hGETA/*/(SCALE)*/;
	ss=(FL)s/GETA/*/SCALE;*/;
	vv=(FL)v/GETA/*/SCALE*/;

	switch((int)(hh/hSCALE)) {
	case 0:
		rr=1.0*SCALE;
		gg=hh;
		bb=0.0;
		break;
	case 1:
		rr=2.0*hSCALE-hh;
		gg=1.0*SCALE;
		bb=0.0;
		break;
	case 2:
		rr=0.0;
		gg=1.0*SCALE;
		bb=hh-2.0*hSCALE;
		break;
	case 3:
		rr=0;
		gg=4.0*hSCALE-hh;
		bb=1.0*SCALE;
		break;
	case 4:
		rr=hh-4.0*hSCALE;
		gg=0;
		bb=1.0*SCALE;
		break;
	case 5:
		rr=1.0*SCALE;
		gg=0;
		bb=6.0*hSCALE-hh;
		break;
	}

	rr=(rr+(1.0*SCALE-rr)*(1.0*SCALE-ss)/(SCALE))*vv/SCALE;
	gg=(gg+(1.0*SCALE-gg)*(1.0*SCALE-ss)/(SCALE))*vv/SCALE;
	bb=(bb+(1.0*SCALE-bb)*(1.0*SCALE-ss)/(SCALE))*vv/SCALE;
	*r=CINT(rr);
	*g=CINT(gg);
	*b=CINT(bb);
	if(*r > 255) *r=255;
	if(*g > 255) *g=255;
	if(*b > 255) *b=255;
}
