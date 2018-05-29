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

double cPol(int c1,int c2,int c3,int c4,double xr)
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
	return f;
}

int to_int(double x)
{
	int y;

	y=(int)(x+0.5);
	if(y<0) y=0;
	else if(y>255) y=255;

	return y;
}

int interpolate(ImageData *img,double x1,double y1,Pixel *pix)
{
	int px,py;
	int val;
	double r[4],g[4],b[4],o[4];
	double rr,gg,bb,oo;
	double xrate,yrate;
	Pixel col1,col2,col3,col4;

	px=(int)x1;
	py=(int)y1;
	xrate=x1-(double)px;
	yrate=y1-(double)py;

	val = getPixel(img,px-1,py-1,&col1);	
	val = getPixel(img,px  ,py-1,&col2);	
	val = getPixel(img,px+1,py-1,&col3);	
	val = getPixel(img,px+2,py-1,&col4);	
	r[0]=cPol(col1.r,col2.r,col3.r,col4.r,xrate);
	g[0]=cPol(col1.g,col2.g,col3.g,col4.g,xrate);
	b[0]=cPol(col1.b,col2.b,col3.b,col4.b,xrate);

	val = getPixel(img,px-1,py,&col1);	
	val = getPixel(img,px  ,py,&col2);	
	val = getPixel(img,px+1,py,&col3);	
	val = getPixel(img,px+2,py,&col4);	
	r[1]=cPol(col1.r,col2.r,col3.r,col4.r,xrate);
	g[1]=cPol(col1.g,col2.g,col3.g,col4.g,xrate);
	b[1]=cPol(col1.b,col2.b,col3.b,col4.b,xrate);

	val = getPixel(img,px-1,py+1,&col1);	
	val = getPixel(img,px  ,py+1,&col2);	
	val = getPixel(img,px+1,py+1,&col3);	
	val = getPixel(img,px+2,py+1,&col4);	
	r[2]=cPol(col1.r,col2.r,col3.r,col4.r,xrate);
	g[2]=cPol(col1.g,col2.g,col3.g,col4.g,xrate);
	b[2]=cPol(col1.b,col2.b,col3.b,col4.b,xrate);

	val = getPixel(img,px-1,py+2,&col1);	
	val = getPixel(img,px  ,py+2,&col2);	
	val = getPixel(img,px+1,py+2,&col3);	
	val = getPixel(img,px+2,py+2,&col4);	
	r[0]=cPol(col1.r,col2.r,col3.r,col4.r,xrate);
	g[0]=cPol(col1.g,col2.g,col3.g,col4.g,xrate);
	b[0]=cPol(col1.b,col2.b,col3.b,col4.b,xrate);

	rr=cPol(r[0],r[1],r[2],r[3],yrate);
	gg=cPol(g[0],g[1],g[2],g[3],yrate);
	bb=cPol(b[0],b[1],b[2],b[3],yrate);

	pix->r=to_int(rr);
	pix->g=to_int(gg);
	pix->b=to_int(bb);
	return TRUE;
}
