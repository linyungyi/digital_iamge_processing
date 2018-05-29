#include <stdlib.h>
#include <math.h>

double lx= -1.0,ly= -1.0,lz= 1.0;
double mil=130;	
double env=30; 


void adjustLightSourse()
{
	double abl;

	abl=sqrt(lx*lx+ly*ly+lz*lz);
	lx=lx/abl;
	ly=ly/abl;
	lz=lz/abl;
}


int lighting(int g,double dx,double dy,double dz,double x,double y,double z)
{
	double cosT,cosT2;
	double rx,ry,rz;
	double abl;
	double env2;
	int res;


	abl=sqrt(dx*dx+dy*dy+dz*dz);
	dx=dx/abl;
	dy=dy/abl;
	dz=dz/abl;
	cosT=dx*lx+dy*ly+dz*lz;
	rz=2.0*cosT*dz-lz;
	if(rz>0.0)
		cosT2=rz*rz*rz*rz*rz*rz*rz*rz*rz*rz*rz*rz;// n=12;
	else
	    cosT2=0;
	if(cosT<0.0) cosT=0.0;
	env2=env*255.0/100.0;
	res=(int)((cosT*255.0+env2)*(double)g/255.0);
	if(res>g) res=g;
	res+=(mil*cosT2);
	if(res>255) res=255;
	if(res<0) res=0;
	return res;
}

