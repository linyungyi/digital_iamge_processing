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

int plsi(int x)
{
	if(x<0) return 0;
	return x;
}


int crgb(int x)
{
	if(x<0) return 0;
	if(x>255) return 255;
	return x;
}

int linarPol(int c1,int c2,int c3,int c4,double xr,double yr)
{
	double d,e,f;
	int res;

	d=(double)c1*(1.0-xr)+(double)c3*xr;
	e=(double)c2*(1.0-xr)+(double)c4*xr;
	f=d*(1.0-yr)+e*yr;
	res=(f+0.5);
	if(res>255) res=255;
	else if(res<0) res=0;
	return res;
}

int getPatVal(int *mat,int n,int x,int y)
{
	if(x<0 || y<0 || x>=n || y>=n) return 0;

	return mat[x+y*n];
}

int linearInterpolate(int *mat,int n,double x1,double y1)
{
	int lx,ly;
	int adr;
	int mx,my,dep;
	int px,py;
	int nx,ny;
	double xrate,yrate;
	int col1,col2,col3,col4,col_n;

	nx=(int)(x1);
	ny=(int)(y1);
	px=(int)x1;
	py=(int)y1;
	xrate=x1-(double)px;
	yrate=y1-(double)py;
	col1=getPatVal(mat,n,px,py);
	col2=getPatVal(mat,n,px,py+1);
	col3=getPatVal(mat,n,px+1,py);
	col4=getPatVal(mat,n,px+1,py+1);
	return linarPol(col1,col2,col3,col4,xrate,yrate);
}

int makeBlurPat(int n,int ang,int **x,int **y,int **val,int *sum)
{
	int *tmpx,*tmpy,*tmpv;
	int *mat;
	int msum,tsum;
	int xx,yy,i;
	int vv;
	int hf;
	double rad;
	double fx,fy,tx,ty;
	double sinT,cosT;

	hf=n/2;

	tmpx=malloc(sizeof(int)*n*n);
	tmpy=malloc(sizeof(int)*n*n);
	tmpv=malloc(sizeof(int)*n*n);
	mat=malloc(sizeof(int)*n*n);
	for(yy=0;yy<n;yy++) {
		for(xx=0;xx<n;xx++) {
			mat[xx+yy*n]=0;
		}
	}
	msum=0;
	for(xx=0;xx<n;xx++) {
		vv=xx+1;
		if(vv>hf+1) vv=0;
		mat[xx+hf*n]=vv;
		msum+=vv;
	}

	rad=(double)ang*(3.14159265)/180.0;
	sinT=sin(rad);
	cosT=cos(rad);

	i=0;
	tsum=0;
	for(yy=0;yy<n;yy++) {
		for(xx=0;xx<n;xx++) {
			tx=(double)(xx-hf);
			ty=(double)(yy-hf);
			fx=  cosT*tx+sinT*ty + (double)hf;
			fy= -sinT*tx+cosT*ty + (double)hf;
			vv=linearInterpolate(mat,n,fx,fy);
			if(vv) {
				tmpx[i]=xx-hf;
				tmpy[i]=yy-hf;
				tmpv[i]=vv;
				tsum+=tmpv[i];
				i++;
			}
		}
	}
	free(mat);
	*x=tmpx;
	*y=tmpy;
	*val=tmpv;
	*sum=tsum;
	return i;
}

int effect(ImageData *img,ImageData *outimg,int ef,int ang)
{
	int val;
	int i;
	int x,y;
	int *px,*py,*pat,nn;
	int xx,yy;
	int rr,gg,bb,oo;
	int endn;
	Pixel col,pixb,ncol;
	int mx;
	int pat_sum;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	mx=2*ef+1;

	nn=makeBlurPat(mx,ang,&px,&py,&pat,&pat_sum);
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			rr=gg=bb=oo=0;
			for(i=0;i<nn;i++) {
				val = getPixel(img,x+px[i],y+py[i],&ncol);	
				rr+=ncol.r  * pat[i];
				gg+=ncol.g  * pat[i];
				bb+=ncol.b  * pat[i];
				oo+=pat[i];
			}
			col.r = rr/(oo);
			col.g = gg/(oo);
			col.b = bb/(oo);
			setPixel(outimg,x,y,&col);	
		}
	}
	free(px);
	free(py);
	free(pat);
	return TRUE;
}
