#include "image.h"
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<3) {
		printf("パ??ータが足りません");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("画?が読めません");
		return;
	}


	outimg=createImage(img->width,img->height,24);
	
	effect(img,outimg);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

#define RG_SIZE 3

int reg[9][9];
int rgox[9];
int rgoy[9];

int imgR[25],imgG[25],imgB[25];


int setRegInfo()
{
	reg[0][0]=1;	reg[0][1]=1;	reg[0][2]=0;
	reg[0][3]=1;	reg[0][4]=1;	reg[0][5]=1;
	reg[0][6]=0;	reg[0][7]=1;	reg[0][8]=1;
	rgox[0]=-2;
	rgoy[0]=-2;

	reg[1][0]=1;	reg[1][1]=1;	reg[1][2]=1;
	reg[1][3]=1;	reg[1][4]=1;	reg[1][5]=1;
	reg[1][6]=0;	reg[1][7]=1;	reg[1][8]=0;
	rgox[1]=-1;
	rgoy[1]=-2;

	reg[2][0]=0;	reg[2][1]=1;	reg[2][2]=1;
	reg[2][3]=1;	reg[2][4]=1;	reg[2][5]=1;
	reg[2][6]=1;	reg[2][7]=1;	reg[2][8]=0;
	rgox[2]= 0;
	rgoy[2]=-2;

	reg[3][0]=0;	reg[3][1]=1;	reg[3][2]=1;
	reg[3][3]=1;	reg[3][4]=1;	reg[3][5]=1;
	reg[3][6]=0;	reg[3][7]=1;	reg[3][8]=1;
	rgox[3]= 0;
	rgoy[3]=-1;

	reg[4][0]=1;	reg[4][1]=1;	reg[4][2]=0;
	reg[4][3]=1;	reg[4][4]=1;	reg[4][5]=1;
	reg[4][6]=0;	reg[4][7]=1;	reg[4][8]=1;
	rgox[4]= 0;
	rgoy[4]= 0;

	reg[5][0]=0;	reg[5][1]=1;	reg[5][2]=0;
	reg[5][3]=1;	reg[5][4]=1;	reg[5][5]=1;
	reg[5][6]=1;	reg[5][7]=1;	reg[5][8]=1;
	rgox[5]=-1;
	rgoy[5]= 0;

	reg[6][0]=0;	reg[6][1]=1;	reg[6][2]=1;
	reg[6][3]=1;	reg[6][4]=1;	reg[6][5]=1;
	reg[6][6]=1;	reg[6][7]=1;	reg[6][8]=0;
	rgox[6]=-2;
	rgoy[6]= 0;

	reg[7][0]=1;	reg[7][1]=1;	reg[7][2]=0;
	reg[7][3]=1;	reg[7][4]=1;	reg[7][5]=1;
	reg[7][6]=1;	reg[7][7]=1;	reg[7][8]=0;
	rgox[7]=-2;
	rgoy[7]=-1;

	reg[8][0]=1;	reg[8][1]=1;	reg[8][2]=1;
	reg[8][3]=1;	reg[8][4]=1;	reg[8][5]=1;
	reg[8][6]=1;	reg[8][7]=1;	reg[8][8]=1;
	rgox[8]=-1;
	rgoy[8]=-1;
}

int getRegVal(int *img,int num,int *ave)
{
	int xx,yy;
	int ix,iy;
	int sx,sy;
	int ex,ey;
	int tmp,cnt;
	int max,min;
	int adr;
	int val;

	sx=2+rgox[num];
	sy=2+rgoy[num];
	ex=sx+RG_SIZE;
	ey=sy+RG_SIZE;
	tmp=0;
	cnt=0;
	adr=0;
	max= -1;
	min= 256;
	iy=0;
	for(yy=sy;yy<ey;yy++) {
		for(xx=sx;xx<ex;xx++) {
			if(reg[num][adr]) {
				val=img[xx+yy*5];
				tmp+=val;
				cnt++;
				if(max<val) max=val;
				if(min>val) min=val;
			}
			adr++;
		}
	}
	*ave=(tmp/cnt);
	if(*ave>255) *ave=255;
	return max-min;
}

int absi(int i)
{
	if(i<0) return -i;
	return i;
}

int effect(ImageData *img,ImageData *outimg)
{
	int val;
	int x,y;
	int xx,yy;
	int hh;
	int endn;
	int vv,vr,vg,vb;
	int ave,rr,gg,bb;
	Pixel col,ncol;
	Pixel ans;
	int *sobel;
	int sadr;
	int rate,nn,i;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;


	setRegInfo();
	nn=2;
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			rr=gg=bb=0;
			sadr=0;
			for(yy=(-nn);yy<=nn;yy++) {
				for(xx=(-nn);xx<=nn;xx++) {
					val = getPixel(img,x+xx,y+yy,&ncol);	得
					imgR[sadr]=ncol.r;
					imgG[sadr]=ncol.g;
					imgB[sadr]=ncol.b;
					sadr++;
				}
			}
			vr=vg=vb= 1000;
			for(i=0;i<=8;i++) {			
なるものを選ぶ
				vv=getRegVal(imgR,i,&ave);
				if(vr>vv) {
					vr=vv;
					rr=ave;
				}
				vv=getRegVal(imgG,i,&ave);
				if(vg>vv) {
					vg=vv;
					gg=ave;
				}
				vv=getRegVal(imgB,i,&ave);
				if(vb>vv) {
					vb=vv;
					bb=ave;
				}
			}
			ans.r = rr;
			ans.g = gg;
			ans.b = bb;
			setPixel(outimg,x,y,&ans);	
		}
	}
	return TRUE;
}
