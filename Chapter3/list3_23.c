#include "image.h"
#include <math.h>
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<6) {
		printf("パ??ータが足りません");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("画?が読めません");
		return;
	}


	outimg=createImage(img->width,img->height,24);
	
	effect(img,outimg,atoi(av[3]),atoi(av[4]),atoi(av[5]));

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

#define	FACTOR	256

int absi(int x)
{
	if(x<0) return -x;
	return x;
}

int getVal(int ed,int val,int con)
{
	int a,b;
	int v2;

	if(ed>255) ed=255;
	v2=(255-128)*val/255+128;
	a=255-v2;
	b=-a*ed/255+255;
	b=(b*con+val*(100-con))/100;
	return b;
}

int effect(ImageData *img,ImageData *outimg,int act,int st,int sm)
{
	int val;
	int x,y;
	int xx,yy;

	int f1,f2;

	int err,egg,ebb;
	int sr_x,sg_x,sb_x;
	int sr_y,sg_y,sb_y;
	int r1,g1,b1,dum1,dum2;
	int rr,gg,bb,ed;
	int ro,go,bo;
	int ff;
	int st2,con;
	int sum;
	int endn,endn3,endo;
	Pixel col,ncol;
	ImageData *buf;
	int smooth[9];
	int fil[9]={
		-1,-1,-1,
		-1, 8,-1,
		-1,-1,-1};
	int sobel1[9]={
		 1, 0,-1,
		 2, 0,-2,
		 1, 0,-1};
	int sobel2[9]={
		 1, 2, 1,
		 0, 0, 0,
		-1,-2,-1};
	double	factor[7] = { 1.25, 1.5, 1.75, 2.0, 2.5, 3.0, 4.0 };	
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;


	sum=0;
	for(ff=0;ff<9;ff++) {
		if(ff==4) {
			smooth[ff]=10;
			sum+=10;
		}
		else {
			smooth[ff]=sm;
			sum+=sm;
		}
	}

	f1 = (int)((double)FACTOR * factor[act]);
	f2 = (int)((double)FACTOR * (factor[act] - 1.0)/2.0);
	buf=createImage(img->width,img->height,24);

	// 色彩強調??
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			val = getPixel(img,x,y,&col);	
			rr= col.r;
			gg= col.g;
			bb= col.b;
			ro= ( rr*f1-gg*f2-bb*f2)/FACTOR;
			go= (-rr*f2+gg*f1-bb*f2)/FACTOR;
			bo= (-rr*f2-gg*f2+bb*f1)/FACTOR;
			col.r = ro;
			col.g = go;
			col.b = bo;
			setPixel(buf,x,y,&col);	
		}
	}

	st2=st/2;
	con=66;

	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			err=egg=ebb=0;
			sr_x=sg_x=sb_x=0;
			sr_y=sg_y=sb_y=0;
			ff=0;
			for(yy=-1;yy<=1;yy++) {
				for(xx=-1;xx<=1;xx++) {

					val = getPixel(img,x+xx,y+yy,&col);	得

					err-=col.r*fil[ff];
					egg-=col.g*fil[ff];
					ebb-=col.b*fil[ff];


					sr_x+= col.r*sobel1[ff];
					sg_x+= col.g*sobel1[ff];
					sb_x+= col.b*sobel1[ff];
					sr_y+= col.r*sobel2[ff];
					sg_y+= col.g*sobel2[ff];
					sb_y+= col.b*sobel2[ff];

					ff++;
				}
			}

			val = getPixel(buf,x,y,&col);	
			r1=col.r;
			g1=col.g;
			b1=col.b;

			err=err*st/100+(int)sqrt(sr_x*sr_x+sr_y*sr_y)*st2/100;
			egg=egg*st/100+(int)sqrt(sg_x*sg_x+sg_y*sg_y)*st2/100;
			ebb=ebb*st/100+(int)sqrt(sb_x*sb_x+sb_y*sb_y)*st2/100;
			rr=getVal(err,(int)col.r,con);
			gg=getVal(egg,(int)col.g,con);
			bb=getVal(ebb,(int)col.b,con);
			col.r = rr;
			col.g = gg;
			col.b = bb;
			setPixel(buf,x,y,&col);	// バッファに値をセットする
		}
	}

	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			err=egg=ebb=0;
			ff=0;
			for(yy=-1;yy<=1;yy++) {
				for(xx=-1;xx<=1;xx++) {
					val = getPixel(buf,x+xx,y+yy,&col);	得

					err+=col.r*smooth[ff];
					egg+=col.g*smooth[ff];
					ebb+=col.b*smooth[ff];

					ff++;
				}
			}
			col.r = err/sum;
			col.g = egg/sum;
			col.b = ebb/sum;
			setPixel(outimg,x,y,&col);	
		}
	}
	disposeImage(buf);
	return TRUE;
}
