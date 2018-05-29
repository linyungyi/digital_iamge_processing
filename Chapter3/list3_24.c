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

int absi(int x)
{
	if(x<0) return -x;
	return x;
}

#define BRT 0

// ef<10
int effect(ImageData *img,ImageData *outimg,int ef,int rate)
{
	int val;
	int x,y;
	int xx,yy;
	int rr,gg,bb,opa,gray;
	int r0,g0,b0;
	int endn;
	Pixel col,pixb;
	int sumr[20],sumg[20],sumb[20];
	int smx;
	int sq;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;


	sq=(2*ef+1)*(2*ef+1);
	for(y=y1;y<=y2;y++) {
		x=x1;
		for(xx=-ef;xx<ef;xx++) {
			smx=xx+ef;
			sumr[smx]=sumg[smx]=sumb[smx]=0;
			for(yy=-ef;yy<=ef;yy++) {
				val = getPixel(img,x+xx,y+yy,&pixb);	
				gray=(pixb.b*28+77*pixb.r+pixb.g*151)/256;
				if(gray>rate) {
					sumr[smx]+=(int)pixb.r+BRT;
					sumg[smx]+=(int)pixb.g+BRT;
					sumb[smx]+=(int)pixb.b+BRT;
			    }
			}
		}
		for(x=x1;x<=x2;x++) {
			rr=gg=bb=0;
			smx=ef+ef;
			sumr[smx]=sumg[smx]=sumb[smx]=0;
			for(yy=-ef;yy<=ef;yy++) {
				val = getPixel(img,x+ef,y+yy,&pixb);	
				gray=(pixb.b*28+77*pixb.r+pixb.g*151)/256;
				if(gray>rate) {
					sumr[smx]+=(int)pixb.r+BRT;
					sumg[smx]+=(int)pixb.g+BRT;
					sumb[smx]+=(int)pixb.b+BRT;
			    }
			}
			for(xx=0;xx<=smx;xx++) {
				rr+=sumr[xx];
				gg+=sumg[xx];
				bb+=sumb[xx];
				sumr[xx]=sumr[xx+1];
				sumg[xx]=sumg[xx+1];
				sumb[xx]=sumb[xx+1];
			}
			val = getPixel(img,x,y,&pixb);	
			r0=(int)pixb.r;
			g0=(int)pixb.g;
			b0=(int)pixb.b;
			rr= (rr)/(sq);
			gg= (gg)/(sq);
			bb= (bb)/(sq);
			rr= rr*255/(3*r0+255);
			gg= gg*255/(3*g0+255);
			bb= bb*255/(3*b0+255);

			rr= rr + (255 - rr ) *  r0 / 255;
			gg= gg + (255 - gg ) *  g0 / 255;
			bb= bb + (255 - bb ) *  b0 / 255;
			col.r = rr;
			col.g = gg;
			col.b = bb;
			setPixel(outimg,x,y,&col);	
		}
	}
	return TRUE;
}
