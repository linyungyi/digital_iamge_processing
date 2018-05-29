#include "image.h"
#include <math.h>
#include <stdlib.h>

int fil_x[11*11]={
	-14, 	-16, 	-17, 	-19, 	-20, 	0, 	20, 	19, 	17, 	16,	14,
	-16, 	-18, 	-20, 	-22, 	-24, 	0, 	24, 	22, 	20, 	18,	16,
	-17, 	-20, 	-24, 	-28, 	-32, 	0, 	32, 	28, 	24, 	20,	17,
	-19, 	-22, 	-28, 	-35, 	-45, 	0, 	45, 	35, 	28, 	22,	19,
	-20, 	-24, 	-32, 	-45, 	-71, 	0, 	71, 	45, 	32, 	24,	20,
	-20, 	-25, 	-33, 	-50, 	-100, 	0, 	100,	50, 	33, 	25,	20,
	-20, 	-24, 	-32, 	-45, 	-71, 	0, 	71, 	45, 	32, 	24,	20,
	-19, 	-22, 	-28, 	-35, 	-45, 	0, 	45, 	35, 	28, 	22,	19,
	-17, 	-20, 	-24, 	-28, 	-32, 	0, 	32, 	28, 	24, 	20,	17,
	-16, 	-18, 	-20, 	-22, 	-24, 	0, 	24, 	22, 	20, 	18,	16,
	-14, 	-16, 	-17, 	-19, 	-20, 	0, 	20, 	19, 	17, 	16,	14
};

int fil_y[11*11]={
	-14, 	-16, 	-17, 	-19, 	-20,  -20, 	-20, 	-19, 	-17, 	-16,	-14,
	-16, 	-18, 	-20, 	-22, 	-24,  -25, 	-24, 	-22, 	-20, 	-18,	-16,
	-17, 	-20, 	-24, 	-28, 	-32,  -33, 	-32, 	-28, 	-24, 	-20,	-17,
	-19, 	-22, 	-28, 	-35, 	-45,  -50, 	-45, 	-35, 	-28, 	-22,	-19,
	-20, 	-24, 	-32, 	-45, 	-71, -100, 	-71, 	-45, 	-32, 	-24,	-20,
	  0, 	  0, 	  0, 	  0, 	  0, 	0, 	0,		  0, 	  0, 	  0,	  0,
	20, 	24, 	32, 	45, 	71,   100, 	71, 	45, 	 32, 	 24,	20,
	19, 	22, 	28, 	35, 	45,    50, 	45, 	35, 	 28, 	 22,	19,
	17, 	20, 	24, 	28, 	32,    33, 	32, 	28, 	 24, 	 20,	17,
	16, 	18, 	20, 	22, 	24,    25, 	24, 	22, 	 20, 	 18,	16,
	14, 	16, 	17, 	19, 	20,    20, 	20, 	19, 	 17, 	 16,	14
};

int absi(int i)
{
	if(i<0) return -i;
	return i;
}

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



int effect(ImageData *img,ImageData *outimg,int som)
{
	int x,y;
	int i;
	int val;
	int xx,yy;
	int rrx,ggx,bbx;
	int rry,ggy,bby;
	int rr,gg,bb;
	Pixel col;
	int sadr;
	int x1,y1,x2,y2;
	int area,sum;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	area=som*2+1;
	sum=0;
	for(yy=0;yy<area;yy++) {
		sadr=(yy+(5-som))*11+(5-som);
		for(xx=0;xx<area;xx++) {
			sum+=absi(fil_x[sadr]);
			sadr++;
		}
	}
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			rrx=ggx=bbx=0;
			rry=ggy=bby=0;
			for(yy=0;yy<area;yy++) {
				sadr=(yy+(5-som))*11+(5-som);
				for(xx=0;xx<area;xx++) {
					val = getPixel(img,x+xx-1,y+yy-1,&col);	“¾

					rrx+= (int)col.r*fil_x[sadr];
					ggx+= (int)col.g*fil_x[sadr];
					bbx+= (int)col.b*fil_x[sadr];
					rry+= (int)col.r*fil_y[sadr];
					ggy+= (int)col.g*fil_y[sadr];
					bby+= (int)col.b*fil_y[sadr];
					sadr++;
				}
			}
			rr=(int)(sqrt(rrx*rrx+rry*rry)/(double)sum);
			gg=(int)(sqrt(ggx*ggx+ggy*ggy)/(double)sum);
			bb=(int)(sqrt(bbx*bbx+bby*bby)/(double)sum);
			col.r = rr;
			col.g = gg;
			col.b = bb;
			setPixel(outimg,x,y,&col);	
		}
	}
	return TRUE;
}
