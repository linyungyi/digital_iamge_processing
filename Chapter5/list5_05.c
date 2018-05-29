#include "image.h"
#include <stdlib.h>
#include <math.h>

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

// �G�t�F�N�g����
int effect(ImageData *img,ImageData *outimg,int rri,int ggi,int bbi)
{
	int val;
	int x,y;
	int rr,gg,bb;
	int Rtbl[256],Gtbl[256],Btbl[256],i;
	double rrt,grt,brt;
	double fr,fg,fb;
	Pixel col;

	rrt=(double)rri /100.0;
	grt=(double)ggi /100.0;
	brt=(double)bbi /100.0;
	for(i=0;i<256;i++) {
		fr=(double)i/255.0;
		fg=(double)i/255.0;
		fb=(double)i/255.0;
		fr=pow(fr,rrt);
		fg=pow(fg,grt);
		fb=pow(fb,brt);
		if(fr>1.0) fr=1.0;
		if(fg>1.0) fg=1.0;
		if(fb>1.0) fb=1.0;
		Rtbl[i]=(int)(fr*255.0+0.5);
		Gtbl[i]=(int)(fg*255.0+0.5);
		Btbl[i]=(int)(fb*255.0+0.5);
	}
	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&col);	//�摜��̉�f�����擾
			rr=col.r;
			gg=col.g;
			bb=col.b;
			col.r = Rtbl[rr];
			col.g = Gtbl[gg];
			col.b = Btbl[bb];
			setPixel(outimg,x,y,&col);	// �摜�ɒl���Z�b�g����
		}
	}
	return TRUE;
}
