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

int pow2(int x)
{
	//if(x<0) return -x;
	return x*x;
}

// �G�t�F�N�g����
int effect(ImageData *img,ImageData *outimg)
{
	int val;
	int x,y,i;
	int adr;
	int rr,gg,bb;
	int rrr,ggg,bbb;
	int rss,gss,bss;
	int re,ge,be;
	int sum,mx;
	int endn;
	Pixel col;
	int *Rerr,*Gerr,*Berr;
	// �덷���U�p�^�[��
	int err_pat[5*3]= {
		0,0,0,7,5,
		3,5,7,5,3,
		1,3,5,3,1};
	// �ȉ��R�͌덷���U�p�^�[���ɂ���Č��܂�萔
	int d_area=2;	//�덷���U���s����f�͈̔�
	int patx=5;		//�z��err_pat�̉���
	int paty=3;		//�z��err_pat�̏c��

	// ���F��̐F�W��(���F�Ȃ�16�F)
	int pR[16]={  0,  0,255,255,  0,  0,255,255,128,  0,128,128,  0,  0,128, 64};
	int pG[16]={  0,  0,  0,  0,255,255,255,255,128,  0,  0,  0,128,128,128, 64};
	int pB[16]={  0,255,  0,255,  0,255,  0,255,128,128,  0,128,  0,128,  0, 64};

	int pat_sum;
	int ix,iy;
	int ee,est,bst;

	mx=(img->width)+d_area*2;		// �o�b�t�@�̉��T�C�Y�i�����{���[�ɗ]�v�ɂƂ�j
	sum=mx*paty;				// �o�b�t�@�T�C�Y
	Rerr=malloc(sizeof(int)*sum);
	Gerr=malloc(sizeof(int)*sum);
	Berr=malloc(sizeof(int)*sum);
	if(!Rerr || !Gerr || !Berr) return FALSE;	// �o�b�t�@���m�ۂł��Ȃ���ΏI��
	// �o�b�t�@������
	for(x=0;x<sum;x++) {
		Rerr[x]=0;
		Gerr[x]=0;
		Berr[x]=0;
	}
	// ���U�p�^�[���̍��v�l
	pat_sum=0;
	for(i=0;i<(patx*paty);i++) {
		pat_sum+=err_pat[i];
	}
	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&col);	//�摜��̉�f�����擾
			adr=(x+d_area);
			// �����番�U���ꂽ�덷�����Z
			rr=col.r;
			rr+= (Rerr[adr]/pat_sum);
			gg=col.g;
			gg+= (Gerr[adr]/pat_sum);
			bb=col.b;
			bb+= (Berr[adr]/pat_sum);
			rrr=rr;
			ggg=gg;
			bbb=bb;
			// �O�ꌟ���@�Ō��F
			est=999999L; // �傫�Ȑ������Z�b�g
			bst=0;
			for(i=0;i<16;i++) {
				ee=pow2(rrr-pR[i])+pow2(ggg-pG[i])+pow2(bbb-pB[i]);
				if(est>ee) {
					bst=i;
					est=ee;
				}
            }
            rss=pR[bst];
            gss=pG[bst];
            bss=pB[bst];
            // ������f�Ő������덷���v�Z
			re= rr-rss;
			ge= gg-gss;
			be= bb-bss;
			// �덷���U
			adr-=d_area;
			for(iy=0;iy<paty;iy++) {
				for(ix=0;ix<patx;ix++) {
					Rerr[adr+ix]+=(re * err_pat[ix+iy*patx]);
					Gerr[adr+ix]+=(ge * err_pat[ix+iy*patx]);
					Berr[adr+ix]+=(be * err_pat[ix+iy*patx]);
				}
				adr+=mx;
			}
			col.r = rss;
			col.g = gss;
			col.b = bss;
			setPixel(outimg,x,y,&col);	// �摜�ɒl���Z�b�g����
		}
		// �o�b�t�@�̂��炵����
		for(i=0;i<mx;i++) {
			for(iy=0;iy<paty-1;iy++) {
				Rerr[i+iy*mx]=Rerr[i+(iy+1)*mx];
				Gerr[i+iy*mx]=Gerr[i+(iy+1)*mx];
				Berr[i+iy*mx]=Berr[i+(iy+1)*mx];
			}
			Rerr[i+(paty-1)*mx]=0;
			Gerr[i+(paty-1)*mx]=0;
			Berr[i+(paty-1)*mx]=0;
		}
	}
	free(Rerr);
	free(Gerr);
	free(Berr);
	return TRUE;
}
