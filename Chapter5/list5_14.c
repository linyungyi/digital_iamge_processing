#include "image.h"

// �F���Ƃ̉�f�����L�^���邽�߂̃��X�g
typedef struct CINFO
{
  int r;
  int g;
  int b;
  int num;
  struct CINFO *next;
} ColInfo;

ColInfo* getMaxNumColor(ColInfo *clist);

int decreaseColor(ImageData *img,int *r,int *g,int *b)
{
  int mx,my;    // �摜�̑傫��
  int x,y;
  int i;
  Pixel col;
  ColInfo clist,*dum;
  int rr,gg,bb;

  getimginfo(img,&mx,&my);  // �摜�̑傫�������߂�

  initCinfo(&clist);
  for(y=0;y<my;y++) {
    for(x=0;x<mx;x++) {
      GetPixel(img,x,y,&col); // �摜��̉�f�����擾
      rr=col.r;
      gg=col.g;
      bb=col.b;
      if(!existInList(&clist,rr,gg,bb)) { // ���X�g�ɐF�����݂��Ȃ�������
        insColor(&clist,rr,gg,bb);    // ���X�g�ɒǉ�
      }
    }
  }
  for(i=0;i<256;i++) {    // �p���b�g������
    r[i]=0;
    g[i]=0;
    b[i]=0;
  }

  for(i=0;i<256;i++) {
    dum=getMaxNumColor(&clist);
    if(dum==NULL) break;
    r[i]=dum->r;
    g[i]=dum->g;
    b[i]=dum->b;
    dum->num= -1;
  }
  return i;
}

int initCinfo(ColInfo *clist)
{
  clist->r=0;
  clist->g=0;
  clist->b=0;
  clist->num=0;
  clist->next=NULL;
}

int existInList(ColInfo *clist,int rr,int gg,int bb)
{
  ColInfo *i;

  for(i=clist;i!=NULL; i=i->next) {
    if(clist->r == rr &&
       clist->g == gg &&
       clist->b == bb ) {
      return TRUE;
    }
  }
  return FALSE;
}

ColInfo* lastColInfo(ColInfo *clist)
{
  ColInfo *i;

  i=clist;
  while(i!=NULL) {
    if(i->next ==NULL) return i;
  }
  return NULL;
}

int insColor(ColInfo *clist,int rr,int gg,int bb)
{
  ColInfo *dum,*last;

  dum=malloc(sizeof(ColInfo));
  if(dum==NULL) return FALSE;
  last=lastColInfo(clist);
  initCinfo(dum);
  last->next=dum;
  return TRUE;
}

ColInfo* getMaxNumColor(ColInfo *clist)
{
  ColInfo *res,*i;
  int max;

  max=-10000;
  res=NULL;
  for(i=clist;i!=NULL;i=i->next) {
    if(i->num > max ) {
      max=i->num;
      res=i;
    }
  }
  return res;
}
