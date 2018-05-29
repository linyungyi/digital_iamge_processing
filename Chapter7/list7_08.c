#include "image.h"
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<4) {
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
	
	effect(img,outimg,atoi(av[3]));

	writeBMPfile(av[2],outimg);
	disposeImage(img);
	disposeImage(outimg);

}


// �G�t�F�N�g����
int effect(ImageData *img,ImageData *outimg,int area)
{
	int val;
	int i;
	int x,y;
	int xx,yy;
	int sam,nn;
	int first;
	int rr,gg,bb;
	int th;
	int hist[256];
	Pixel col;

	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	// �q�X�g�O�����v�Z
	for(i=0;i<256;i++) hist[i]=0;
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			val = getPixel(img,x,y,&col);	//�摜��̉�f�����擾
			rr=col.r;
			hist[rr]++;
		}
	}

	for(y=y1;y<=y2;y++) {
		sam=nn=0;
		for(yy=y-area;yy<y+area;yy++) {
			for(xx=x1-area;xx<x1+area;xx++) {
				val = getPixel(img,xx,yy,&col);	//�摜��̉�f�����擾
				sam+=(int)(col.r);
				nn++;
			}
		}
		first=TRUE;
		for(x=x1;x<=x2;x++) {
			// ���ς����߂�
			// �[�����ȊO�͑O�񋁂߂����ʂɕω��������𑝌�������
			if(!first) {
				for(yy=y-area;yy<y+area;yy++) {
					val = getPixel(img,x-area-1,yy,&col);	//�摜��̉�f�����擾
					sam-=(int)(col.r);
					val = getPixel(img,x+area-1,yy,&col);	//�摜��̉�f�����擾
					sam+=(int)(col.r);
				}
				first=FALSE;
			}
			th=sam/nn;
			val = getPixel(img,x,y,&col);	//�摜��̉�f�����擾
			rr=col.r;
			gg=col.g;
			bb=col.b;
			if(th<rr) rr=255;
			else rr=0;
			if(th<gg) gg=255;
			else gg=0;
			if(th<bb) bb=255;
			else bb=0;
			col.r = rr;
			col.g = gg;
			col.b = bb;
			setPixel(outimg,x,y,&col);	// �摜�ɒl���Z�b�g����
		}
	}
	return TRUE;
}

