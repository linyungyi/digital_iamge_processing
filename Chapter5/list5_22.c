#include "image.h"
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<3) {
		printf("�p�����[�^������܂���");
		return;
	}

	// �t�@�C�����摜�f�[�^�̓ǂݍ���
	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("�摜���ǂ߂܂���");
		return;
	}

	// ���ʊi�[�p�摜�f�[�^�쐬
	outimg=createImage(img->width,img->height,24);
	
	effect(img,outimg);

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

// �G�t�F�N�g����
int effect(ImageData *img,ImageData *outimg)
{
	int x,y;
	int val;
	int rr,gg,bb;
	int adr;
	Pixel col;
	int pat[16]={
		 10, 9, 8, 7,
		 11, 2, 1, 6,
		 12, 3, 4, 5,
		 13,14,15,16
	};

	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&col);	//�摜��̉�f�����擾
			rr=col.r;
			gg=col.g;
			bb=col.b;
			adr=(x%4)+(y%4)*4;
			rr/=16;
			gg/=16;
			bb/=16;
			if(rr<pat[adr]) rr=0;
			else rr=255;
			if(gg<pat[adr]) gg=0;
			else gg=255;
			if(bb<pat[adr]) bb=0;
			else bb=255;
			col.r = rr;
			col.g = gg;
			col.b = bb;
			setPixel(outimg,x,y,&col);	// �摜�ɒl���Z�b�g����
		}
	}
	return TRUE;
}
