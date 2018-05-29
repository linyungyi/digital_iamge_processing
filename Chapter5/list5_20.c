#include "image.h"
#include <stdlib.h>

#define HM_HASH 6553

typedef	struct {
	int	r;					/* �Ԑ��� */
	int	g;					/* �ΐ��� */
	int	b;					/* ���� */
} Colors;

typedef struct HASHLIST {
	int r,g,b;				// RGB�l
	int num;				// ��f��
	struct HASHLIST *next;	// �|�C���^�`�F�[��
} HashList;

typedef struct RGBARRY {
	int r,g,b;				// RGB�l
	int num;				// ��f��
} RGBArray;

typedef struct BOX_DATA {
	int start;				// �{�b�N�X�̎n�܂�
	int colors;				// �{�b�N�X���̐F��
	int pixels;				// �{�b�N�X���̉�f��
} Box;

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

int get_color(Pixel *col,Colors *pal,int cols)
{
	int i,v;
	int rr,gg,bb;
	int min,minV;

	minV=2100000000;
	rr=col->r;
	gg=col->g;
	bb=col->b;
	for(i=0;i<cols;i++) {
		v=(rr-pal[i].r)*(rr-pal[i].r)+(gg-pal[i].g)*(gg-pal[i].g)+(bb-pal[i].b)*(bb-pal[i].b);
		if(minV>v) {
			min=i;
			minV=v;
		}
	}
	return min;
}

int effect(ImageData *img,ImageData *outimg,int c)
{
	int val;
	int x,y;
	Colors *pal;
	int idx;
	int i;
	Pixel col;

	pal=malloc(sizeof(Colors)*c);
	medianCut(img,0,0,img->width-1,img->height-1,pal,c);
	for(i=0;i<c;i++) {
		printf("%d:(r:%3d g:%3d b:%3d)\n",i,pal[i].r,pal[i].g,pal[i].b);
	}

	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&col);	//�摜��̉�f�����擾
			idx=get_color(&col,pal,c);
			col.r=pal[idx].r;
			col.g=pal[idx].g;
			setPixel(outimg,x,y,&col);	// �摜�ɒl���Z�b�g����
		}
	}
	free(pal);
	return TRUE;
}

int redCompare(const void *v1,const void *v2)
{
	RGBArray *s1,*s2;

	s1=(RGBArray *)v1;
	s2=(RGBArray *)v2;
	return (s1->r)-(s2->r);
}

int greenCompare(const void *v1,const void *v2)
{
	RGBArray *s1,*s2;

	s1=(RGBArray *)v1;
	s2=(RGBArray *)v2;
	return (s1->g)-(s2->g);
}
int blueCompare(const void *v1,const void *v2)
{
	RGBArray *s1,*s2;

	s1=(RGBArray *)v1;
	s2=(RGBArray *)v2;
	return (s1->b)-(s2->b);
}

int initHashList(HashList **h)
{
	int i;

	for(i=0;i<HM_HASH;i++) {
		h[i]=NULL;
	}
}

int medianCut(ImageData *img,int x1,int y1,int x2,int y2,Colors *pal,int colors)
{
	HashList *rgbHash[HM_HASH];
	RGBArray *rgbArray;
	Box *box;
	int num,pix;
	int hm_box,i;
	int st,en;
	int sam,hf;
	int minR,maxR,difR;
	int minG,maxG,difG;
	int minB,maxB,difB;
	int max;
	int idx;
	int lcsam;

	// �F�W��������
	for(i=0;i<colors;i++) {
		pal[i].r=0;
		pal[i].g=0;
		pal[i].b=0;
	}
	box=malloc(sizeof(Box)*colors);
	initHashList(rgbHash);
	num=computeHash(img,x1,y1,x2,y2,rgbHash);
	rgbArray=malloc(sizeof(RGBArray)*num);		// RGB�f�[�^�z��̊m��
	pix=transHashList2Array(rgbHash,rgbArray);

	disposeHash(rgbHash);

	// �{�b�N�X�̏������
	box[0].start=0;
	box[0].colors=num;
	box[0].pixels=pix;
	hm_box=1;
	while(hm_box<colors) {
		// �ő��f�������{�b�N�X��I���i�����Ώہj
		max= -100;
		for(i=0;i<hm_box;i++) {
			if(max<box[i].pixels && box[i].colors>1) {
				idx=i;
				max=box[i].pixels;
			}
		}
		if(max<0) break;
		st=box[idx].start;
		en=box[idx].colors;
		sam=box[idx].pixels;
		// �{�b�N�X����RGB�ŏ��l�ő�l�����߂�
		minR=rgbArray[st].r;
		maxR=rgbArray[st].r;
		minG=rgbArray[st].g;
		maxG=rgbArray[st].g;
		minB=rgbArray[st].b;
		maxB=rgbArray[st].b;
		for(i=1;i<en;i++) {
			int val;

			val=rgbArray[st+i].r;
			if(minR>val) minR=val;
			if(maxR<val) maxR=val;
			val=rgbArray[st+i].g;
			if(minG>val) minG=val;
			if(maxG<val) maxG=val;
			val=rgbArray[st+i].b;
			if(minB>val) minB=val;
			if(maxB<val) maxB=val;
		}
		// �Œ��̕ӂŃ{�b�N�X�𕪊�
		// �i��f���������𒴂���_�����߂�j
		difR=maxR-minR;
		difG=maxG-minG;
		difB=maxB-minB;
		if(difR>=difG && difR>=difB) {	// R�̕ӂ��Œ�
			qsort((char*)&rgbArray[st],en,sizeof(RGBArray),redCompare);
		}
		else if(difG>=difB) {			// G�̕ӂ��Œ�
			qsort((char*)&rgbArray[st],en,sizeof(RGBArray),greenCompare);
		}
		else {							// B�̕ӂ��Œ�
			qsort((char*)&rgbArray[st],en,sizeof(RGBArray),blueCompare);
		}
		hf=sam/2;
		lcsam=rgbArray[st].num;
		for(i=1;i<en-1;i++) {
			if(lcsam>=hf) break;
			lcsam+=rgbArray[st+i].num;
		}
		// �����������ʂ��L�^
		box[idx].colors=i;
		box[idx].pixels=lcsam;
		box[hm_box].start=st+i;
		box[hm_box].colors=en-i;
		box[hm_box].pixels=sam-lcsam;
		hm_box++;
	}
	// �{�b�N�X�����\�l��I��
	// ��\�l=�{�b�N�X���̐F�̕��ϒl
	for (i=0; i<hm_box; i++) {
		int indx;
		int clrs;
		int maxval=255;
		long r, g, b, sum;
		int j;

		indx = box[i].start;
		clrs = box[i].colors;
		r = 0; g = 0; b = 0; sum = 0;
		for (j=0; j<clrs; j++) {
			r += rgbArray[indx + j].r  * rgbArray[indx + j].num;
			g += rgbArray[indx + j].g  * rgbArray[indx + j].num;
			b += rgbArray[indx + j].b  * rgbArray[indx + j].num;
			sum += rgbArray[indx + j].num;
		}

		r = r / sum;	if (r>maxval) r = maxval;
		g = g / sum;	if (g>maxval) g = maxval;
		b = b / sum;	if (b>maxval) b = maxval;
		pal[i].r=r;
		pal[i].g=g;
		pal[i].b=b;
	}
	free(box);
	free(rgbArray);
}

// �n�b�V���\��V�K�쐬
HashList* allocHash(int r,int g,int b)
{
	HashList *n;

	n=malloc(sizeof(HashList));
	if(n==NULL) return n;
	n->r=r;
	n->g=g;
	n->b=b;
	n->num=1;
	n->next=NULL;
	return n;
}

// �n�b�V�����̃`�F�[������������

//                    ���X�g���ɉ�f�l�����݂���   ���X�g���ɉ�f�l�����݂��Ȃ�
//   return value      �Y������f�[�^�ւ̃|�C���^   ���X�g�`�F�[���̖���
//   res               TRUE                         FALSE

HashList* findHashList(HashList *rgbHash,int r,int g,int b,int *res)
{
	HashList *list,*tail;

	tail=rgbHash;
	for(list=rgbHash;list!=NULL;list=list->next) {
		if(list->r == r && list->g == g && list->b == b) {
			*res=TRUE;
			return list;
		}
		tail=list;
	}
	*res=FALSE;
	return tail;	// ���X�g���ɉ�f�l�����݂��Ȃ��ꍇ�̓��X�g�`�F�[���̖�����Ԃ�
}

// �n�b�V�����X�g�����߂�
int computeHash(ImageData *img,int x1,int y1,int x2,int y2,HashList **rgbHash)
{
	int val;
	HashList *fnd;
	int mx,my,dep;
	int x,y;
	int r,g,b;
	int h_val;
	int res;
	int colors;
	Pixel col;

	colors=0;
	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			val = getPixel(img,x,y,&col);	//�摜��̉�f�����擾
			r=col.r;
			g=col.g;
			b=col.b;
			h_val=((r * 33023 + g * 30013 + b * 27011) & 0x7fffffff) % HM_HASH;	// �n�b�V���֐�
			if(rgbHash[h_val]==NULL) {
				// ���g�p�Ȃ�V�K�Ɋ��蓖�Ă�
				rgbHash[h_val]=allocHash(r,g,b);
				colors++;
			}
			else {
				fnd=findHashList(rgbHash[h_val],r,g,b,&res);
				if(res) { // ���X�g���ɑ��݂���ꍇ
					(fnd->num)++;
				}
				else {
					fnd->next=allocHash(r,g,b);
					colors++;
				}
			}
		}
	}
	return colors;
}

// �n�b�V������z��ɕϊ�����
int transHashList2Array(HashList **hash,RGBArray *ary)
{
	int n,pix;
	int i;
	HashList *chn;

	n=0;
	pix=0;
	for(i=0;i<HM_HASH;i++) {
		for(chn=hash[i];chn!=NULL;chn=chn->next) {
			ary[n].r   = chn->r;
			ary[n].g   = chn->g;
			ary[n].b   = chn->b;
			ary[n].num = chn->num;
			pix+=chn->num;
			n++;
		}
	}
	return pix;
}

int disposeHash(HashList **rgbHash)
{
	int i;
	HashList *chn,*nxt;

	for(i=0;i<HM_HASH;i++) {
		for(chn=rgbHash[i];chn!=NULL;chn=nxt) {
			nxt=chn->next;
			free(chn);
		}
	}
}
