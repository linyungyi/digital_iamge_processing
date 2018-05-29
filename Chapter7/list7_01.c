#include "image.h"
#include <math.h>
#include <stdlib.h>

#define TSIZE 32

int swapval(int *v1,int *v2)
{
	int dum;

	dum= *v1;
	*v1 = *v2;
	*v2 = dum;
}

main(int ac,char *av[])
{
	ImageData *img1,*img2;
	ImageData *timg1,*timg2;
	int i;
	int D,minD,id;
	int res;
	int x,y;
	int nx,ny;

	if(ac<3) {
		printf("�p�����[�^������܂���");
		return;
	}

	// �t�@�C�����摜�f�[�^�̓ǂݍ���
	res=readBMPfile(av[1],&img1);
	if(res<0) {
		printf("�摜���ǂ߂܂���");
		return;
	}

	// ���ʊi�[�p�摜�f�[�^�쐬
	timg1=createImage(TSIZE,TSIZE,24);
	timg2=createImage(TSIZE,TSIZE,24);
	
	transform(img1,timg1);
	id=-1;
	minD=0;
	for(i=2;i<ac;i++) {
		res=readBMPfile(av[i],&img2);
		if(res<0) {
			printf("�摜���ǂ߂܂���(%s)",av[i]);
			return;
		}
		transform(img2,timg2);
		D=getDistance(timg1,timg2);
		if(id<0 || minD>D) {
			id=i;
			minD=D;
		}
		disposeImage(img2);
	}
	// �ł��߂��摜�̏o��
	printf("**** �ł��߂��摜��(%s) ... ��v�x[%d]\n",av[id],minD);

	disposeImage(img1);
	disposeImage(timg1);
	disposeImage(timg2);

}

int getDistance(ImageData *img1,ImageData *img2)
{
	int x,y;
	int mx,my;
	int dr,dg,db;
	Pixel col1,col2;
	int D=0,d;

	mx=img1->width;
	my=img1->height;
	if(mx!=img2->width || mx!=img2->height) return -1;
	for(y=0;y<my;y++) {
		for(x=0;x<mx;x++) {
			getPixel(img1,x,y,&col1);
			getPixel(img2,x,y,&col2);
			dr= col1.r - col2.r;
			dg= col1.g - col2.g;
			db= col1.b - col2.b;
			D+=(dr*dr+dg*dg+db+db);
		}
	}
	return D;
}

// �ό`�����̖{��
int transform(ImageData *img,ImageData *outimg)
{
	int x,y;
	int x1,y1;
	int x2,y2;
	int rx1,ry1,rx2,ry2;
	Pixel col;
	int h1,w1;
	int h2,w2;
	double fx,fy;

	x1=0;
	y1=0;
	x2=outimg->width - 1;
	y2=outimg->height - 1;
	
	rx1=0;
	ry1=0;
	rx2=img->width - 1;
	ry2=img->height - 1;

	
	w1=rx2-rx1+1;
	h1=ry2-ry1+1;
	w2=x2-x1+1;
	h2=y2-y1+1;
	col.r=col.g=col.b=128;
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			fx=(x-x1)*w1/(double)w2+rx1;//���W�ϊ�
			fy=(y-y1)*h1/(double)h2+ry1;
			if(interpolate(img,fx,fy,&col)) {	//�F���擾(�g��k���ł�FALSE���Ԃ邱�Ƃ͂Ȃ���)
				setPixel(outimg,x,y,&col);	// �摜�ɒl���Z�b�g����
			}
		}
	}
}

// ��Ԃ����l�����߂�
int interpolate(ImageData *img,double x1,double y1,Pixel *pix)
{
	int x,y;
	
	x=(int)(x1+0.5);
	y=(int)(y1+0.5);
	return getPixel(img,x,y,pix);
}
