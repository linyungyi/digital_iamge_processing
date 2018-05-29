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

int absi(int i)
{
	if(i<0) return -i;
	return i;
}

#define BUF_MX 20


int effect(ImageData *img,ImageData *outimg,int th,int nn)
{
	int val;
	int x,y;
	int xx,yy;
	int hh;
	int tot,count;
	int dr,dg,db;
	int endn;
	int c1,c2;
	int rr,gg,bb;
	Pixel col,ncol;
	Pixel ans;
	int bufR[400],bufG[400],bufB[400];
	int ovx;
	int n2;
	int adr;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	n2=nn*2+1;
	for(y=y1;y<=y2;y++) {
		x=x1;
		for(yy=(-nn);yy<=nn;yy++) {
			adr=(yy+nn)*BUF_MX;
			for(xx=(-nn);xx<nn;xx++) {
				val = getPixel(img,x+xx,y+yy,&ncol);	
				bufR[adr]=ncol.r;
				bufG[adr]=ncol.g;
				bufB[adr]=ncol.b;
				adr++;
			}
		}
		ovx=n2-1;
		for(x=x1;x<=x2;x++) {
			xx=nn;
			for(yy=(-nn);yy<=nn;yy++) {
				adr=(yy+nn)*BUF_MX+ovx;
				val = getPixel(img,x+xx,y+yy,&ncol);	
				bufR[adr]=ncol.r;
				bufG[adr]=ncol.g;
				bufB[adr]=ncol.b;
				adr++;
			}
			ovx=(ovx+1)%n2;
			rr=gg=bb=0;
			count=0;
			val = getPixel(img,x,y,&col);	
			for(yy=0;yy<n2;yy++) {
				adr=yy*BUF_MX;
				for(xx=0;xx<n2;xx++) {
					dr=absi((int)(col.r) - bufR[adr]);
					dg=absi((int)(col.g) - bufG[adr]);
					db=absi((int)(col.b) - bufB[adr]);
					if(dr<th && dg<th && db<th) {
						rr+= bufR[adr];
						gg+= bufG[adr];
						bb+= bufB[adr];
						count++;
					}
					adr++;
				}
			}
			if(count) {
				hh=rr/count;
				ans.r = hh;
				hh=gg/count;
				ans.g = hh;
				hh=bb/count;
				ans.b = hh;
			}
			else {
				ans=col;
			}
			setPixel(outimg,x,y,&ans);	
		}
	}
	return TRUE;
}
