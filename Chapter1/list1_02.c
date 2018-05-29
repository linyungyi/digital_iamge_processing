#include "image.h"
/* List1-2
  ��?�f�[�^�쐬
   width :��?�̉�?
   height:��?�̏c?
   depth :�P��f?����̃r�b�g?(8 or 24)
 */
ImageData* createImage(int width,int height,int depth)
{
	ImageData *newimg;
	int byte_per_pixel;

	if(width<0 || height<0) return NULL;
	if(depth!=8 && depth!=24) return NULL;	
	
	newimg=malloc(sizeof(ImageData));
	if(newimg==NULL) return NULL;

	byte_per_pixel=depth/8;

	newimg->pixels=malloc(sizeof(BYTE)*byte_per_pixel*width*height);
	if(newimg->pixels==NULL) {
		free(newimg);
		return NULL;
	}

	newimg->width=width;
	newimg->height=height;
	newimg->depth=depth;
	return newimg;
}