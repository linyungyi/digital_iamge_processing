#include "image.h"
#include <stdlib.h>
#include <math.h>

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

double distance(int x1,int y1,int x2,int y2)
{
	double dx,dy;

	dx=(double)(x2-x1);
	dy=(double)(y2-y1);
	return sqrt(dx*dx+dy*dy);
}

// �G�t�F�N�g����
int effect(ImageData *img,ImageData *outimg)
{
	int val;
	int x,y;
	int i;
	int mx,my;
	int rmx,rmy;
	int d;
	int prev,tmp;
	short *dist,*res;
	double kyori,min;
	Pixel col;
	int x1,y1,x2,y2;

	x1=0;
	y1=0;
	x2=img->width-1;
	y2=img->height-1;

	rmx=(x2-x1)+1;
	rmy=(y2-y1)+1;
	/* �����o�b�t�@�͎��͂P��f���]���ɂƂ� */
	mx=rmx+2;
	my=rmy+2;

	dist=malloc(sizeof(short)*mx*my);
	res=malloc(sizeof(short)*mx*my);	// �����摜���i�[
	for(y=0;y<my;y++) {
		for(x=0;x<mx;x++) {
			dist[x+y*mx]=0;
		}
	}
	/* Step 1 */
	d=0;
	for(x=x1;x<=x2;x++) {
		for(y=y1;y<=y2;y++) {
			val = getPixel(img,x,y,&col);	//�摜��̉�f�����擾
			if(col.r>0) {
				d++;
				setDist(dist,x-x1,y-y1,mx,d);
			}
			else {
				d=0;
			}
		}
		for(y=y2;y>=y1;y--) {
			val = getPixel(img,x,y,&col);	//�摜��̉�f�����擾
			if(col.r>0) {
				d++;
				if(getDist(dist,x-x1,y-y1,mx)>d)
					setDist(dist,x-x1,y-y1,mx,d);
			}
			else {
				d=0;
			}
		}
	}
	for(y=0;y<rmy;y++) {
		prev=-1;
		for(x=0;x<rmx;x++) {
			min=100000000.0;
			if(getDist(dist,x,y,mx)==0) {
				setDist(res,x,y,mx,0);
			}
			else {
				for(i=prev;i<=x;i++) {
					kyori=distance(x,y,i,y+getDist(dist,i,y,mx));
					if(kyori<min) {
						min=kyori;
						tmp=i;
					}
					prev=tmp;
					setDist(res,x,y,mx,(int)(min*10+0.5));
				}
			}
		}
		prev=rmx;
		for(x=rmx-1;x>=0;x--) {
			min=100000000.0;
			if(getDist(dist,x,y,mx)==0) {
				setDist(res,x,y,mx,0);
			}
			else {
				for(i=prev;i>=x;i--) {
					kyori=distance(x,y,i,y+getDist(dist,i,y,mx));
					if(kyori<min) {
						min=kyori;
						tmp=i;
					}
					prev=tmp;
					d=(int)(min*10+0.5);
					if(getDist(res,x,y,mx)>d)
						setDist(res,x,y,mx,d);
				}
			}
		}
	}
	// �����摜�����߂鏈���͂����܂�
	// �ȉ��͌��ʂ��������邽�߂̏���
	for(x=x1;x<=x2;x++) {
		for(y=y1;y<=y2;y++) {
			d=getDist(res,x-x1,y-y1,mx);
			col.r=d;
			col.g=d;
			col.b=d;
			setPixel(outimg,x,y,&col);	// �摜�ɒl���Z�b�g����
		}
	}
	free(dist);
	free(res);
}

int setDist(short *dist,int x,int y,int mx,int d)
{
	dist[x+1+(y+1)*mx]=d;
}

int getDist(short *dist,int x,int y,int mx)
{
	return dist[(x+1)+(y+1)*mx];
}
