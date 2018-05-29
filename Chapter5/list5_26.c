#include "image.h"
#include <stdlib.h>

main(int ac,char *av[])
{
	ImageData *img,*outimg;
	int res;
	int x,y,mx,my;

	if(ac<3) {
		printf("パラメータが足りません");
		return;
	}

	// ファイルより画像データの読み込み
	res=readBMPfile(av[1],&img);
	if(res<0) {
		printf("画像が読めません");
		return;
	}

	// 結果格納用画像データ作成
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

// エフェクト処理
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
	Pixel col;
	int Rerr,Gerr,Berr;
	int pR[16]={  0,  0,255,255,  0,  0,255,255,128,  0,128,128,  0,  0,128, 64};
	int pG[16]={  0,  0,  0,  0,255,255,255,255,128,  0,  0,  0,128,128,128, 64};
	int pB[16]={  0,255,  0,255,  0,255,  0,255,128,128,  0,128,  0,128,  0, 64};
	int ee,est,bst;


	Rerr=0;
	Gerr=0;
	Berr=0;
	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			rr=col.r;
			rr+= Rerr;
			gg=col.g;
			gg+= Gerr;
			bb=col.b;
			bb+= Berr;
			rrr=rr;
			ggg=gg;
			bbb=bb;
			est=999999L;
			bst=0;
			// 徹底検索法で減色
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
            // 誤差を計算
			Rerr= rr-rss;
			Gerr= gg-gss;
			Berr= bb-bss;
			col.r = rss;
			col.g = gss;
			col.b = bss;
			setPixel(outimg,x,y,&col);	// 画像に値をセットする
		}
	}
	return TRUE;
}
