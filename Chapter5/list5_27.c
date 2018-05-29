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
	int sum,mx,nerr;
	int endn;
	Pixel col;
	int *Rerr,*Gerr,*Berr;
	// ���F��̐F�W��(���F�Ȃ�16�F)
	int pR[16]={  0,  0,255,255,  0,  0,255,255,128,  0,128,128,  0,  0,128, 64};
	int pG[16]={  0,  0,  0,  0,255,255,255,255,128,  0,  0,  0,128,128,128, 64};
	int pB[16]={  0,255,  0,255,  0,255,  0,255,128,128,  0,128,  0,128,  0, 64};
	int ee,est,bst;

	mx=(img->width)+2;		// �o�b�t�@�̉��T�C�Y�i�����{���[�ɂQ��f���]�v�ɂƂ�j
	sum=mx*2;			// �Q�񕪂Ȃ̂łQ�{
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
	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&col);	//�摜��̉�f�����擾
			adr= x+1;
			// �����番�U���ꂽ�덷�����Z
			rr=col.r;
			rr+= Rerr[adr]/16;
			gg=col.g;
			gg+= Gerr[adr]/16;
			bb=col.b;
			bb+= Berr[adr]/16;
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
			// �E��
			Rerr[adr+1]+=re*7;
			Gerr[adr+1]+=ge*7;
			Berr[adr+1]+=be*7;

			// ������
			Rerr[adr+mx-1]+=re*3;
			Gerr[adr+mx-1]+=ge*3;
			Berr[adr+mx-1]+=be*3;

			// ����
			Rerr[adr+mx]+=re*5;
			Gerr[adr+mx]+=ge*5;
			Berr[adr+mx]+=be*5;

			// �E����
			Rerr[adr+mx+1]+=re*1;
			Gerr[adr+mx+1]+=ge*1;
			Berr[adr+mx+1]+=be*1;

			col.r = rss;
			col.g = gss;
			col.b = bss;
			setPixel(outimg,x,y,&col);	// �摜�ɒl���Z�b�g����
		}
		// �o�b�t�@�̂��炵����
		for(i=0;i<mx;i++) {
			Rerr[i]=Rerr[i+mx];
			Gerr[i]=Gerr[i+mx];
			Berr[i]=Berr[i+mx];
			Rerr[i+mx]=0;
			Gerr[i+mx]=0;
			Berr[i+mx]=0;
		}
	}
	free(Rerr);
	free(Gerr);
	free(Berr);
	return TRUE;
}
