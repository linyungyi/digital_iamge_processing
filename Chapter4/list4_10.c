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
		printf("パ??ータが足りません");
		return;
	}


	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("画?が読めません");
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


int getOrg(double xa,double ya,double xb,double yb,
            double xc,double yc,double xd,double yd,
            double *x,double *y)
{
    *x=(xa+xb+xc+xd)/4.0;
    *y=(ya+yb+yc+yd)/4.0;
}

int setMatrix(double *mat,int mx,int y,double x1,double x2,double x3,double x4,double x5,double x6)
{
    mat[y*mx+0]=x1;
    mat[y*mx+1]=x2;
    mat[y*mx+2]=x3;
    mat[y*mx+3]=x4;
    mat[y*mx+4]=x5;
    mat[y*mx+5]=x6;
}

int getParam(double fx1,double fy1,double fx2,double fy2,double fx3,double fy3,
             double tx1,double ty1,double tx2,double ty2,double tx3,double ty3,
             double *pa,double *pb,double *pc,double *pd,double *pe,double *pf)
{
	int i;
	double mat[6*6];
	int x,y;
	double vec[6];
	double para[6];

    setMatrix(mat,6,0,tx1,ty1,1.0,0.0,0.0,0.0);
    setMatrix(mat,6,1,0.0,0.0,0.0,tx1,ty1,1.0);
    setMatrix(mat,6,2,tx2,ty2,1.0,0.0,0.0,0.0);
    setMatrix(mat,6,3,0.0,0.0,0.0,tx2,ty2,1.0);
    setMatrix(mat,6,4,tx3,ty3,1.0,0.0,0.0,0.0);
    setMatrix(mat,6,5,0.0,0.0,0.0,tx3,ty3,1.0);
	vec[0]=(double)fx1;
	vec[1]=(double)fy1;
	vec[2]=(double)fx2;
	vec[3]=(double)fy2;
	vec[4]=(double)fx3;
	vec[5]=(double)fy3;
	solveSystemOfLinearEquations(6,mat,vec,para);
	*pa=para[0];
	*pb=para[1];
	*pc=para[2];
	*pd=para[3];
	*pe=para[4];
	*pf=para[5];
}

int inRegion(double xa,double ya,double xb,double yb,double xc,double yc,
             double x,double y)
{
    double s,t;
    double det;
    double x1,y1,x2,y2,xx,yy;

    x1=xa-xb;
    y1=ya-yb;
    x2=xc-xb;
    y2=yc-yb;
    xx=x -xb;
    yy=y -yb;
    det=x1*y2-x2*y1;
    s=( y2*xx-x2*yy);
    t=(-y1*xx+x1*yy);
    if(s>=0.0 && t>=0.0 && (s+t)<=det) return TRUE;
    return FALSE;
}

int selectTriangle(double xa,double ya,double xb,double yb,
                   double xc,double yc,double xd,double yd,
                   double tox,double toy,double x,double y)
{
	if(inRegion(xa,ya,tox,toy,xd,yd,x,y)) return 0;
	if(inRegion(xd,yd,tox,toy,xc,yc,x,y)) return 1;
	if(inRegion(xc,yc,tox,toy,xb,yb,x,y)) return 2;
    if(inRegion(xb,yb,tox,toy,xa,ya,x,y)) return 3;
    return -1;
}


int transform(ImageData *img,ImageData **outimg_p,int *px,int *py)
{
	ImageData *outimg;
	int x,y;
	int wt,ht;
	int xa,ya,xb,yb;
	int xc,yc,xd,yd;
	double xx,yy;
	double ttx,tty;
	int tri;
	double pa[4],pb[4],pc[4],pd[4],pe[4],pf[4];
	int i;
	int x1,y1,x2,y2;
	Pixel col,bcol;
	int h1,w1;
	int h2,w2;
	double fox,foy;
    int nx,ny;
    double tox,toy;
    int rx1,ry1,rx2,ry2;

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
	// 変形先領域の頂点を取得
	xa=(double)px[0];
	ya=(double)py[0];
	xb=(double)px[3];
	yb=(double)py[3];
	xc=(double)px[2];
	yc=(double)py[2];
	xd=(double)px[1];
	yd=(double)py[1];

	getOrg(xa,ya,xb,yb,xc,yc,xd,yd,&tox,&toy);	
	fox=(double)(rx1+rx2)/2.0;					
	foy=(double)(ry1+ry2)/2.0;


	getParam((double)rx1,(double)ry1,fox,foy,(double)rx2,(double)ry1,
				xa,ya,tox,toy,xd,yd,
            	&pa[0],&pb[0],&pc[0],&pd[0],&pe[0],&pf[0]);
	getParam((double)rx2,(double)ry1,fox,foy,(double)rx2,(double)ry2,
				xd,yd,tox,toy,xc,yc,
            	&pa[1],&pb[1],&pc[1],&pd[1],&pe[1],&pf[1]);
	getParam((double)rx2,(double)ry2,fox,foy,(double)rx1,(double)ry2,
				xc,yc,tox,toy,xb,yb,
            	&pa[2],&pb[2],&pc[2],&pd[2],&pe[2],&pf[2]);
	getParam((double)rx1,(double)ry2,fox,foy,(double)rx1,(double)ry1,
				xb,yb,tox,toy,xa,ya,
            	&pa[3],&pb[3],&pc[3],&pd[3],&pe[3],&pf[3]);


	bcol.r=bcol.g=bcol.b=128;
	for(y=0;y<ht;y++)	{
		for(x=0;x<wt;x++)	{
			xx=(double)x;
			yy=(double)y;

			tri=selectTriangle(xa,ya,xb,yb,xc,yc,xd,yd,tox,toy,xx,yy);
			if(tri>=0) {	
				
				ttx=pa[tri]*xx + pb[tri]*yy	+ pc[tri];
				tty=pd[tri]*xx + pe[tri]*yy	+ pf[tri];
				
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
