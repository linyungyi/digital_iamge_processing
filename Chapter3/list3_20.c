#include "image.h"
#include <math.h>
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<5) {
		printf("ƒp??[ƒ^‚ª‘«‚è‚Ü‚¹‚ñ");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("‰æ?‚ª“Ç‚ß‚Ü‚¹‚ñ");
		return;
	}


	outimg=createImage(img->width,img->height,24);

	effect(img,outimg,atoi(av[3]),atoi(av[4]));

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}


int effect(ImageData *img,ImageData *outimg,int msx,int msy)
{
	int val;
	int x,y;
	int xx,yy;
	int sx,sy;
	int ex,ey;
	int bx,by;
	int rr,gg,bb;
	int dum,dum2;
	int endn;
	int mm;
	int mlx,mly,zzx;
	int my1,my2;
	Pixel col;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	if(msx==0) msx=1;
	if(msy==0) msy=1;
	mlx=(x2-x1+1+msx-1)/msx;
	mly=(y2-y1+1+msy-1)/msy;
	zzx=msx/2;
	for(y=0;y<mly;y++) {
	    bx=0;
		for(x=bx;x<x2;x+=msx) {
			rr=gg=bb=0;
			sx=x;
			if(x<0) sx=0;
			sy=y1+y*msy;
			ex=x+msx-1;
			ey=sy+msy-1;
			if(ex>x2) ex=x2;
			if(ey>y2) ey=y2;
			for(yy=sy;yy<=ey;yy++) {
				for(xx=sx;xx<=ex;xx++) {
					val = getPixel(img,xx,yy,&col);	
					rr+=col.r;
					gg+=col.g;
					bb+=col.b;
				}
			}
			mm=(ex-sx+1)*(ey-sy+1);
			if(mm) {
				rr/=mm;
				gg/=mm;
				bb/=mm;
				col.r = rr;
				col.g = gg;
				col.b = bb;
				for(yy=sy;yy<=ey;yy++) {
					for(xx=sx;xx<=ex;xx++) {
						setPixel(outimg,xx,yy,&col);	
					}
				}
			}
		}
	}
	return TRUE;
}
