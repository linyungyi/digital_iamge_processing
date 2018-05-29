#include "image.h"
#include <stdlib.h>

#define HM_HASH 6553
typedef struct HASHLIST {
	int r,g,b;				// RGB�l
	int num;				// ��f��
	struct HASHLIST *next;	// �|�C���^�`�F�[��
} HashList;

main(){}


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
	HashList *fnd;
	int mx,my,dep;
	int val;
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
