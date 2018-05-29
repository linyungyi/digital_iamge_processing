
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

int setMatrix(int i,double *mat,double *vec,int *X,int *Y,int *x,int *y)
{
	int j;

	j=i*2;
	mat[j*8+0]=(double)X[i];
	mat[j*8+1]=(double)Y[i];
	mat[j*8+2]=1.0;
	mat[j*8+3]=0.0;
	mat[j*8+4]=0.0;
	mat[j*8+5]=0.0;
	mat[j*8+6]= -(double)(X[i]*x[i]);
	mat[j*8+7]= -(double)(Y[i]*x[i]);
	vec[j]=(double)(x[i]);
	j++;
	mat[j*8+0]=0.0;
	mat[j*8+1]=0.0;
	mat[j*8+2]=0.0;
	mat[j*8+3]=(double)X[i];
	mat[j*8+4]=(double)Y[i];
	mat[j*8+5]=1.0;
	mat[j*8+6]= -(double)(X[i]*y[i]);
	mat[j*8+7]= -(double)(Y[i]*y[i]);
	vec[j]=(double)(y[i]);
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
	int x1,y1,x2,y2;
	int tox,toy;
	int vx[4],vy[4];
	int fx[4],fy[4];
	double mat[8*8],vec[8];
	double pa[8];

	rx1=0;
	ry1=0;
	rx2=img->width-1;
	ry2=img->height-1;

	fx[0]=rx1;	fy[0]=ry1;
	fx[1]=rx2;	fy[1]=ry1;
	fx[2]=rx2;	fy[2]=ry2;
	fx[3]=rx1;	fy[3]=ry2;
	for(i=0;i<4;i++) {
		vx[i]=px[i];
		vy[i]=py[i];
	}
	x1=x2=vx[0];	y1=y2=vy[0];
	for(i=0;i<4;i++) {
		setMatrix(i,mat,vec,vx,vy,fx,fy);
		if(x1>vx[i]) x1=vx[i];
		if(x2<vx[i]) x2=vx[i];
		if(y1>vy[i]) y1=vy[i];
		if(y2<vy[i]) y2=vy[i];
	}
	if(!solveSystemOfLinearEquations(8,mat,vec,pa)) {
		// •û’öŽ®‚ª‰ð‚¯‚È‚¢‚Æ‚«‚Í??‚µ‚È‚¢
		printf("Can't transform\n");
		return FALSE;
	}

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


	bcol.r=bcol.g=bcol.b=128;
	for(y=0;y<ht;y++)	{
		for(x=0;x<wt;x++)	{
			double xx,yy;
			double ttx,tty;
			double bs;
			//int tox,toy;

			bs=pa[6]*(double)x+pa[7]*(double)y+1;
			if(bs) {
				xx=(double)x;	yy=(double)y;
				ttx=(pa[0]*xx+pa[1]*yy+pa[2])/bs;
				tty=(pa[3]*xx+pa[4]*yy+pa[5])/bs;
				tox=(int)(ttx+.5);
				toy=(int)(tty+.5);
				if(tox>=rx1 && tox<=rx2 && toy>=ry1 && toy<=ry2) {
					if(interpolate(img,ttx,tty,&col)) {
						setPixel(outimg,x,y,&col);	
					}
					else {
						setPixel(outimg,x,y,&bcol);	‚·‚é
					}
				}
				else {
					setPixel(outimg,x,y,&bcol);	
				}
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
