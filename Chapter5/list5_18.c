#include "image.h"
#include <stdlib.h>

#define HM_HASH 6553
typedef struct HASHLIST {
	int r,g,b;				// RGB値
	int num;				// 画素数
	struct HASHLIST *next;	// ポインタチェーン
} HashList;

main(){}


// ハッシュ表を新規作成
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

// ハッシュ内のチェーンを検索する
//                    リスト内に画素値が存在する   リスト内に画素値が存在しない
//   return value      該当するデータへのポインタ   リストチェーンの末尾
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
	return tail;	// リスト内に画素値が存在しない場合はリストチェーンの末尾を返す
}

// ハッシュリストを求める
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
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			r=col.r;
			g=col.g;
			b=col.b;
			h_val=((r * 33023 + g * 30013 + b * 27011) & 0x7fffffff) % HM_HASH;	// ハッシュ関数
			if(rgbHash[h_val]==NULL) {
				// 未使用なら新規に割り当てる
				rgbHash[h_val]=allocHash(r,g,b);
				colors++;
			}
			else {
				fnd=findHashList(rgbHash[h_val],r,g,b,&res);
				if(res) { // リスト内に存在する場合
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
