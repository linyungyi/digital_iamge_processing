#include <windows.h>
#include "image.h"

#define CNUM 256

int RGBhist[CNUM][CNUM][CNUM]

int makeHistogram(ImageData *img)
{
  int mx,my;    // �摜�̑傫��
  int x,y;
  int i;
  Pixel col;
  int rr,gg,bb;

  getimginfo(img,&mx,&my);  // �摜�̑傫�������߂�

  // �q�X�g�O�����̏�����
  for(rr=0;rr<CNUM;rr++) {
    for(gg=0;gg<CNUM;gg++) {
      for(bb=0;bb<CNUM;bb++) {
        RGBhist[rr][gg][bb]=0;
      }
    }
  }

  for(y=0;y<my;y++) {
    for(x=0;x<mx;x++) {
      getPixel(img,x,y,&col); // �摜��̉�f�����擾
      rr=col.r;
      gg=col.g;
      bb=col.b;
      RGBhist[rr][gg][bb]++;
      }
    }
  }
}
