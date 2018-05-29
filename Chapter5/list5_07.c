#include "image.h"
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<6) {
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
	
	effect(img,outimg,atoi(av[3]),atoi(av[4]),atoi(av[5]) );

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

int getMax(int x,int y,int z)
{
	int mx;

	if(x>y) mx=x;
	else mx=y;
	if(z>mx) mx=z;
	return mx;
}

// �G�t�F�N�g����
int effect(ImageData *img,ImageData *outimg,int rtR,int rtG,int rtB)
{
	int val;
	int x,y;
	int rr,gg,bb;
	int v1,v2;
	Pixel col;

	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&col);	//�摜��̉�f�����擾
			rr=col.r;
			gg=col.g;
			bb=col.b;
			v1=getMax(rr,gg,bb);
			rr=rr*rtR;
			gg=gg*rtG;
			bb=bb*rtB;
			v2=getMax(rr,gg,bb);
			if(v2==0) {
				rr=0;
				gg=0;
				bb=0;
			}
			else {
				rr=rr*v1/(v2);
				gg=gg*v1/(v2);
				bb=bb*v1/(v2);
				if(rr>255) rr=255;
				if(gg>255) gg=255;
				if(bb>255) bb=255;
			}
			col.r = rr;
			col.g = gg;
			col.b = bb;
			setPixel(outimg,x,y,&col);	// �摜�ɒl���Z�b�g����
		}
	}

	return TRUE;
}
