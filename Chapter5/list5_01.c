#include "image.h"
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<7) {
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
	
	effect(img,outimg,atoi(av[3]),atoi(av[4]),atoi(av[5]),atoi(av[6]));

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}

// �G�t�F�N�g����
// *_flag�͂��̐��������������ۂ����w�肷��p�����[�^
// 0:�������Ȃ� 1:������
int effect(ImageData *img,ImageData *outimg,int c_flag,int m_flag,int y_flag,int k_flag)
{

	int x,y;
	int i;
	int val;
	int rr,gg,bb;
	int cc,mm,yy,kk;
	Pixel pix;

	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&pix);	//�摜��̉�f�����擾
			rr=pix.r;
			gg=pix.g;
			bb=pix.b;
			RGBtoCMYK(rr,gg,bb,&cc,&mm,&yy,&kk);
			// �c�������ȊO���O�ɂ���
			if(c_flag==0) cc=0;
			if(m_flag==0) mm=0;
			if(y_flag==0) yy=0;
			if(k_flag==0) kk=0;
			CMYKtoRGB(cc,mm,yy,kk,&rr,&gg,&bb);
			pix.r = rr;
			pix.g = gg;
			pix.b = bb;
			setPixel(outimg,x,y,&pix);	// �摜�ɒl���Z�b�g����
		}
	}

	return TRUE;
}

int RGBtoCMYK(int rr,int gg,int bb,int *cc,int *mm,int *yy,int *kk)
{
    int c,m,y,k;
    c=255-rr;
    m=255-gg;
    y=255-bb;
    k=min3(c,m,y);
    if(k==255) {
        *cc=0;
        *mm=0;
        *yy=0;
    }
    else {
        *cc=(c-k)*255/(255-k);
        *mm=(m-k)*255/(255-k);
        *yy=(y-k)*255/(255-k);
    }
        *kk=k;
}

int CMYKtoRGB(int cc,int mm,int yy,int kk,int *rr,int *gg,int *bb)
{
    int r,g,b;

    *rr=255-min2(255, (255*cc-kk*cc+255*kk)/255);
    *gg=255-min2(255, (255*mm-kk*mm+255*kk)/255);
    *bb=255-min2(255, (255*yy-kk*yy+255*kk)/255);
}

int min3(int x,int y,int z)
{
    int m=x;
    if(y<m) m=y;
    if(z<m) m=z;
    return m;
}

int min2(int x,int y)
{
    if(x<y) return x;
    return y;
}
