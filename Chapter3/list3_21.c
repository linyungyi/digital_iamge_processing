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

#define GAP 10

int absi(int i)
{
  if(i<0) return -i;
  return i;
}

int effect(ImageData *img,ImageData *outimg,int ol,int gammaint)
{
  int val;
  int x,y;
  int xx,yy;
  int hh;
  int du,dd,dl,dr;
  int endn;
  int c1,c2;
  double gamma,a;
  int rrx,ggx,bbx;
  int rry,ggy,bby;
  int rrr,ggg,bbb;
  int rr,gg,bb,gray;
  int rate;
  int res1,res2,res;
  int th1,th2,th3;
  int r1,g1,b1;
  Pixel col,ncol;
  int *sobel;
  int sadr;
  int sobel1[9]={
     1, 0,-1,
     2, 0,-2,
     1, 0,-1};
  int sobel2[9]={
     1, 2, 1,
     0, 0, 0,
    -1,-2,-1};
  int x1,y1,x2,y2;

  x1=0;
  y1=0;
  x2=img->width-1;
  y2=img->height-1;

  gamma=(double)gammaint/100.0;
  gamma=1.0/gamma;
  a=128.0/255.0;
  th1=(int)(pow(a,gamma)*255.0);
  a=96.0/255.0;
  th2=(int)(pow(a,gamma)*255.0);
  a=64.0/255.0;
  th3=(int)(pow(a,gamma)*255.0);
  for(y=y1;y<=y2;y++) {
    for(x=x1;x<=x2;x++) {
      rrx=ggx=bbx=0;
      rry=ggy=bby=0;
      sadr=0;
      for(yy=0;yy<3;yy++) {
        for(xx=0;xx<3;xx++) {
          val = getPixel(img,x+xx-1,y+yy-1,&col); 
          rr=col.r;
          gg=col.g;
          bb=col.b;
          rrx+= rr*sobel1[sadr];
          rry+= rr*sobel2[sadr];
          ggx+= gg*sobel1[sadr];
          ggy+= gg*sobel2[sadr];
          bbx+= bb*sobel1[sadr];
          bby+= bb*sobel2[sadr];
          sadr++;
        }
      }
      rrr=(int)(sqrt((double)(rrx*rrx+rry*rry))/8.0);
      ggg=(int)(sqrt((double)(ggx*ggx+ggy*ggy))/8.0);
      bbb=(int)(sqrt((double)(bbx*bbx+bby*bby))/8.0);
      if(rrr>ggg) rate=rrr;
      else rate=ggg;
      if(bbb>rate) rate=bbb;

      if(rate<ol) res1=255;
      else if(rate<ol+GAP) {
        res1=255-255*(rate-ol)/GAP;
      }
      else res1=0;

      val = getPixel(img,x,y,&col); 
      rr=col.r;
      gg=col.g;
      bb=col.b;
      gray=(bb*28+77*rr+gg*151)/256;

      if(gray>th1)     res2=255;
      else if(gray>th2) res2=128;
      else if(gray>th3) res2=64;
      else res2=0;

      res=res1*res2/256;
      col.r = res;
      col.g = res;
      col.b = res;
      setPixel(outimg,x,y,&col);  
    }
  }
  return TRUE;
}
