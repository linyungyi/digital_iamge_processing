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
	int res;
	int nx,ny;

	if(ac<5) {
		printf("ƒp??[ƒ^‚ª‘«‚è‚Ü‚¹‚ñ");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("‰æ?‚ª“Ç‚ß‚Ü‚¹‚ñ");
		return;
	}
	nx=atoi(av[3]);
	ny=atoi(av[4]);


	outimg=createImage(nx,ny,24);
	
	transform(img,outimg);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}



int transform(ImageData *img,ImageData *outimg)
{
	int x,y;
	int x1,y1;
	int x2,y2;
	int rx1,ry1,rx2,ry2;
	Pixel col;
	int h1,w1;
	int h2,w2;
	double fx,fy;

	x1=0;
	y1=0;
	x2=outimg->width - 1;
	y2=outimg->height - 1;
	
	rx1=0;
	ry1=0;
	rx2=img->width - 1;
	ry2=img->height - 1;

	
	w1=rx2-rx1+1;
	h1=ry2-ry1+1;
	w2=x2-x1+1;
	h2=y2-y1+1;
	col.r=col.g=col.b=128;
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			fx=(x-x1)*w1/(double)w2+rx1;
			fy=(y-y1)*h1/(double)h2+ry1;
			if(interpolate(img,fx,fy,&col)) {	
				setPixel(outimg,x,y,&col);	
			}
		}
	}
}

double mat[16]=
{
	-1, 1,-1, 1,
	 0, 0, 0, 1,
	 1, 1, 1, 1,
	 8, 4, 2, 1
};

int cPol(int c1,int c2,int c3,int c4,double xr)
{
	double vec[4],res[4];
	double f;
	int c;

	vec[0]=c1;
	vec[1]=c2;
	vec[2]=c3;
	vec[3]=c4;
	solveSystemOfLinearEquations(4,mat,vec,res);

	f=res[0]*xr*xr*xr+res[1]*xr*xr+res[2]*xr+res[3];
	c=(int)(f+0.5);
	if(c<0) c=0;
	else if(c>255) c=255;
	return c;
}


int interpolate(ImageData *img,double x1,double y1,Pixel *pix)
{
	int px,py;
	int val;
	int r1,g1,b1,o1;
	int r2,g2,b2,o2;
	double xrate,yrate;
	Pixel col1,col2,col3,col4,col_n;

	px=(int)x1;
	py=(int)y1;
	xrate=x1-(double)px;
	yrate=y1-(double)py;

	py=(int)(y1+0.5);
	val = getPixel(img,px-1,py,&col1);	
	val = getPixel(img,px  ,py,&col2);	
	val = getPixel(img,px+1,py,&col3);	
	val = getPixel(img,px+2,py,&col4);	

	r1=cPol(col1.r,col2.r,col3.r,col4.r,xrate);
	g1=cPol(col1.g,col2.g,col3.g,col4.g,xrate);
	b1=cPol(col1.b,col2.b,col3.b,col4.b,xrate);

	px=(int)(x1+0.5);
	py=(int)y1;
	val = getPixel(img,px,py-1,&col1);	
	val = getPixel(img,px,py  ,&col2);	
	val = getPixel(img,px,py+1,&col3);	
	val = getPixel(img,px,py+2,&col4);	

	r2=cPol(col1.r,col2.r,col3.r,col4.r,xrate);
	g2=cPol(col1.g,col2.g,col3.g,col4.g,xrate);
	b2=cPol(col1.b,col2.b,col3.b,col4.b,xrate);

	pix->r=(r1+r2)/2;
	pix->g=(g1+g2)/2;
	pix->b=(b1+b2)/2;
	return TRUE;
}
