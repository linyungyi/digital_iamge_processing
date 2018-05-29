/* List1-4
  ��?�f�[�^��̉�f�l���擾
  x,y ��f�̍��W
  pix ��f�l���i�[����
 */
int getPixel(ImageData *img,int x,int y,Pixel *pix)
{
  int ret=1;
  int adr;  
  int dep,val;
  BYTE *pixels;

  if(img==NULL) return -1;
  if(img->pixels==NULL) return -1;

  if(x<0) {
    x=0;
    ret=0;
  }
  if(x >= img->width ) {
    x=img->width -1;
    ret=0;
  }
  if(y<0) {
    y=0;
    ret=0;
  }
  if(y >= img->height ) {
    y=img->height -1;
    ret=0;
  }
  dep=img->depth;
  adr=x + y*img->width;
  pixels=img->pixels;
  if(dep==8) {  
    val=pixels[adr];
    pix->r=val;
    pix->g=val;
    pix->b=val;
  }
  else if(dep==24) {
    pixels+=(adr*3);
    pix->r=(*pixels);
    pixels++;
    pix->g=(*pixels);
    pixels++;
    pix->b=(*pixels);
  }
  else {
    return -1;
  }
  return ret;
}

/*
  ��f�l�̕␳�i�͈͊O�̒l��͈͓��Ɏ��߂�j
*/
int correctValue(int val,int max)
{
  if(val<0) return 0;
  if(val>max) return max;
  return val;
}
