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
	int val;
	int x,y;
	int xx,yy;
	int i;
	int mx,my;
	int rmx,rmy;
	int d;
	int g,g2;
	Pixel col,scol;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	scol.r=0;
	scol.g=0;
	scol.b=0;
	
	for(x=x1;x<=x2;x++) {
		for(y=y1;y<=y2;y++) {
			setPixel(outimg,x,y,&scol);
		}
	}
	scol.r=255;
	scol.g=255;
	scol.b=255;	
	for(x=x1;x<=x2;x++) {
		for(y=y1;y<=y2;y++) {
			val = getPixel(img,x,y,&col);	//�摜��̉�f�����擾
			g=(col.r)/10-1;
			// �����l�������Ă���ꍇ�C�c������
			if(g>=0) {
				for(yy=-g;yy<=g;yy++) {
					for(xx=-g;xx<=g;xx++) {
						setPixel(outimg,x+xx,y+yy,&scol);
				    }
				}
			}
		}
	}
}
