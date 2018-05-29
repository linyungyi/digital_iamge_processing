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
	//if(x<0) return -x;
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
	int sum,mx;
	int endn;
	Pixel col;
	int *Rerr,*Gerr,*Berr;
	// 誤差分散パターン
	int err_pat[5*3]= {
		0,0,0,7,5,
		3,5,7,5,3,
		1,3,5,3,1};
	// 以下３つは誤差分散パターンによって決まる定数
	int d_area=2;	//誤差分散を行う画素の範囲
	int patx=5;		//配列err_patの横幅
	int paty=3;		//配列err_patの縦幅

	// 減色後の色集合(原色など16色)
	int pR[16]={  0,  0,255,255,  0,  0,255,255,128,  0,128,128,  0,  0,128, 64};
	int pG[16]={  0,  0,  0,  0,255,255,255,255,128,  0,  0,  0,128,128,128, 64};
	int pB[16]={  0,255,  0,255,  0,255,  0,255,128,128,  0,128,  0,128,  0, 64};

	int pat_sum;
	int ix,iy;
	int ee,est,bst;

	mx=(img->width)+d_area*2;		// バッファの横サイズ（横幅＋両端に余計にとる）
	sum=mx*paty;				// バッファサイズ
	Rerr=malloc(sizeof(int)*sum);
	Gerr=malloc(sizeof(int)*sum);
	Berr=malloc(sizeof(int)*sum);
	if(!Rerr || !Gerr || !Berr) return FALSE;	// バッファが確保できなければ終了
	// バッファ初期化
	for(x=0;x<sum;x++) {
		Rerr[x]=0;
		Gerr[x]=0;
		Berr[x]=0;
	}
	// 分散パターンの合計値
	pat_sum=0;
	for(i=0;i<(patx*paty);i++) {
		pat_sum+=err_pat[i];
	}
	for(y=0;y<img->height;y++) {
		for(x=0;x<img->width;x++) {
			val = getPixel(img,x,y,&col);	//画像上の画素情報を取得
			adr=(x+d_area);
			// 他から分散された誤差を加算
			rr=col.r;
			rr+= (Rerr[adr]/pat_sum);
			gg=col.g;
			gg+= (Gerr[adr]/pat_sum);
			bb=col.b;
			bb+= (Berr[adr]/pat_sum);
			rrr=rr;
			ggg=gg;
			bbb=bb;
			// 徹底検索法で減色
			est=999999L; // 大きな数字をセット
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
            // 処理画素で生じた誤差を計算
			re= rr-rss;
			ge= gg-gss;
			be= bb-bss;
			// 誤差分散
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
			setPixel(outimg,x,y,&col);	// 画像に値をセットする
		}
		// バッファのずらし処理
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
