/* List1-5
  ��?�f�[�^��̉�f�l��ύX����
  x,y ��f�̍��W
  pix �Z�b�g�����f�l
 */
int setPixel(ImageData *img,int x,int y,Pixel *pix)
{
	int adr;	
	int dep,val;
	BYTE *pixels;

	if(img==NULL) return -1;
	if(img->pixels==NULL) return -1;

	if(x<0 || x >= img->width || y<0 || y >= img->height ) {
		return 0;
	}
	dep=img->depth;
	adr=x + y*img->width;
	pixels=img->pixels;
	if(dep==8) {
		pixels[adr]=correctValue(pix->r,PIXELMAX);
	}
	else if(dep==24) {
		pixels+=(adr*3);
		(*pixels)=correctValue(pix->r,PIXELMAX);
		pixels++;
		(*pixels)=correctValue(pix->g,PIXELMAX);
		pixels++;
		(*pixels)=correctValue(pix->b,PIXELMAX);
	}
	else {
		return -1;
	}
	return 1;
}
