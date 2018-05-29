#include <windows.h>
#include "image.h"

#define CNUM 256

int RGBhist[CNUM][CNUM][CNUM]

int makeHistogram(ImageData *img)
{
  int mx,my;    // ‰æ‘œ‚Ì‘å‚«‚³
  int x,y;
  int i;
  Pixel col;
  int rr,gg,bb;

  getimginfo(img,&mx,&my);  // ‰æ‘œ‚Ì‘å‚«‚³‚ğ‹‚ß‚é

  // ƒqƒXƒgƒOƒ‰ƒ€‚Ì‰Šú‰»
  for(rr=0;rr<CNUM;rr++) {
    for(gg=0;gg<CNUM;gg++) {
      for(bb=0;bb<CNUM;bb++) {
        RGBhist[rr][gg][bb]=0;
      }
    }
  }

  for(y=0;y<my;y++) {
    for(x=0;x<mx;x++) {
      getPixel(img,x,y,&col); // ‰æ‘œã‚Ì‰æ‘fî•ñ‚ğæ“¾
      rr=col.r;
      gg=col.g;
      bb=col.b;
      RGBhist[rr][gg][bb]++;
      }
    }
  }
}
