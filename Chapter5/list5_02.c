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
	
	effect(img,outimg,atoi(av[3]),atoi(av[4]),atoi(av[5]));

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

// �G�t�F�N�g����
// *_flag�͂��̐��������������ۂ����w�肷��p�����[�^
// 0:�������Ȃ� 1:������
int effect(ImageData *img,ImageData *outimg,int y_flag,int i_flag,int q_flag)
{

	int x,y;
	int i;
	int val;
	int rr,gg,bb;
	double yy,ii,qq;
	Pixel pix;

	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&pix);	//�摜��̉�f�����擾
			rr=pix.r;
			gg=pix.g;
			bb=pix.b;

			RGBtoYIQ(rr,gg,bb,&yy,&ii,&qq);
			// �c�������ȊO���O�ɂ���
			if(y_flag==0) yy=0;
			if(i_flag==0) ii=0;
			if(q_flag==0) qq=0;
			YIQtoRGB(yy,ii,qq,&rr,&gg,&bb);
			pix.r = rr;
			pix.g = gg;
			pix.b = bb;
			setPixel(outimg,x,y,&pix);	// �摜�ɒl���Z�b�g����
		}
	}

	return TRUE;
}

int RGBtoYIQ(int rr,int gg,int bb,double *yy,double *ii,double *qq)
{
    double r,g,b;

    r=((double)rr)/255.0;
    g=((double)gg)/255.0;
    b=((double)bb)/255.0;
    *yy= 0.2990*r+0.5870*g+0.1140*b;
    *ii= 0.5959*r-0.2750*g-0.3210*b;
    *qq= 0.2065*r-0.4969*g+0.2904*b;

}

int shusei(int x)
{
    if(x<0) return 0;
    if(x>255) return 255;
    return x;
}

int YIQtoRGB(double yy,double ii,double qq,int *rr,int *gg,int *bb)
{
    double r,g,b;

    r=yy+0.9489*ii+0.6561*qq;
    g=yy-0.2645*ii-0.6847*qq;
    b=yy-1.1270*ii+0.8050*qq;

    *rr=shusei((int)(r*255.0));
    *gg=shusei((int)(g*255.0));
    *bb=shusei((int)(b*255.0));
}
