#include "image.h"
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<3) {
		printf("ƒp??[ƒ^‚ª‘«‚è‚Ü‚¹‚ñ");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("‰æ?‚ª“Ç‚ß‚Ü‚¹‚ñ");
		return;
	}


	outimg=createImage(img->width,img->height,24);
	
	effect(img,outimg);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}


int effect(ImageData *img,ImageData *outimg)
{
	int val;
	int x,y;
	int xx,yy;
	int rr,gg,bb;
	int endn;
	Pixel col;
	int rd;
	int sortr[9],sortg[9],sortb[9];
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			rr=gg=bb=0;
			rd=0;
			for(yy=-1;yy<=1;yy++) {
				for(xx=-1;xx<=1;xx++) {
					val = getPixel(img,x+xx,y+yy,&col);	“¾

					sortr[rd]=col.r;
					sortg[rd]=col.g;
					sortb[rd]=col.b;
					rd++;
				}
			}
			col.r = median(sortr);
			col.g = median(sortg);
			col.b = median(sortb);
			setPixel(outimg,x,y,&col);	
		}
	}
	return TRUE;
}

int median(int *sort)
{
	int i,j,n;
	int max;

	for( i=0; i<5; i++ ){
		for( j=1, n=0, max=sort[0]; j<9; j++ ){
			if( max<sort[j] ){
				max = sort[j];
				n = j;
			}
		}
		sort[n] = -1;
	}
	return max;
}
