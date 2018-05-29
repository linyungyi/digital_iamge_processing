#include "image.h"
#include <math.h>
#include <stdlib.h>

int swapval(int *v1,int *v2)
{
	int dum;

	dum= *v1;
	*v1 = *v2;
	*v2 = dum;
}

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int i,adr;
	int px[4],py[4];
	int res;

	if(ac<(3+4*2)) {
		printf("ƒp??[ƒ^‚ª‘«‚è‚Ü‚¹‚ñ");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("‰æ?‚ª“Ç‚ß‚Ü‚¹‚ñ");
		return;
	}
	adr=3;
	for(i=0;i<4;i++) {
		px[i]=atoi(av[adr]);
		adr++;
		py[i]=atoi(av[adr]);
		adr++;
	}

	transform(img,&outimg,px,py);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}


int calcPara(int rx1,int ry1,int rx2,int ry2,int *vx,int *vy,double *pa,double *pb)
{
	int i;
	double mat[4*4];
	int x,y;
	double vec[4];

	for(i=0;i<4;i++) {
		mat[i*4  ]=(double)(vx[i]*vy[i]);
		mat[i*4+1]=(double)vx[i];
		mat[i*4+2]=(double)vy[i];
		mat[i*4+3]=1.0;
	}
	vec[0]=(double)rx1;
	vec[1]=(double)rx2;
	vec[2]=(double)rx2;
	vec[3]=(double)rx1;

	solveSystemOfLinearEquations(4,mat,vec,pa);
	vec[0]=(double)ry1;
	vec[1]=(double)ry1;
	vec[2]=(double)ry2;
	vec[3]=(double)ry2;

	solveSystemOfLinearEquations(4,mat,vec,pb);
}



int transform(ImageData *img,ImageData **outimg_p,int *px,int *py)
{
	ImageData *outimg;
	int x,y;
	int wt,ht;

	int i;
	Pixel col,bcol;
	int h1,w1;
	int h2,w2;
    int rx1,ry1,rx2,ry2;
	double pa[4],pb[4];

	rx1=0;
	ry1=0;
	rx2=img->width-1;
	ry2=img->height-1;

	wt=px[0];	ht=py[0];
	for(i=1;i<4;i++) {
		x=px[i];
		y=py[i];
		if(x>wt) wt=x;
		if(y>ht) ht=y;
	}
	wt++;
	ht++;
	*outimg_p=createImage(wt,ht,24);
	outimg=(*outimg_p);



	calcPara(rx1,ry1,rx2,ry2,px,py,pa,pb);
	bcol.r=bcol.g=bcol.b=128;
	for(y=0;y<ht;y++)	{
		for(x=0;x<wt;x++)	{
			double xx,yy;
			double ttx,tty;
			int tox,toy;

			xx=(double)x;	yy=(double)y;

			ttx=pa[0]*xx*yy+pa[1]*xx+pa[2]*yy+pa[3];
			tty=pb[0]*xx*yy+pb[1]*xx+pb[2]*yy+pb[3];
			tox=(int)(ttx+.5);
			toy=(int)(tty+.5);
			if(tox>=rx1 && tox<=rx2 && toy>=ry1 && toy<=ry2) {
				if(interpolate(img,ttx,tty,&col)) {
					setPixel(outimg,x,y,&col);	
				}
				else {
					setPixel(outimg,x,y,&bcol);	
				}
			}
			else {
				setPixel(outimg,x,y,&bcol);	
			}
		}
	}
}


int interpolate(ImageData *img,double x1,double y1,Pixel *pix)
{
	int x,y;
	
	x=(int)(x1+0.5);
	y=(int)(y1+0.5);
	return getPixel(img,x,y,pix);
}
