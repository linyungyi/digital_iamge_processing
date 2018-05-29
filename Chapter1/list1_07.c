// List1-7

int writeBMPfile(char *fname,ImageData *img)
{
	FILE *fp;
	BITMAPFILEHEADER bfn;
	int w,h,rw;
	int mxb,pad;
	int depth;	
	int pbyte;	
	int palsize;	
	int x,y,i;
	int saveloop,saverest;
	int	iBytes;
	unsigned int wsize;
	Pixel pix;

	w=img->width;
	h=img->height;
	depth=img->depth;

	if(depth!=24) {
		//errcode=-3;
		goto $abort1;
	}

	if(depth==24) {
		pbyte=1;
	}
	else {
		pbyte=depth/8;
	}
	if(depth>=24) {
		palsize=0;
	}
	else {
		palsize=256;
	}
	// -- end --

	rw=getDIBxmax(w,depth);

	bfn.bfType=0x4d42;	
	bfn.bfSize=14+40+/*sizeof(BITMAPFILEHEADER) +sizeof(BITMAPINFOHEADER) +*/
			   palsize*4/*sizeof(RGBQUAD)*/ +
			   rw*h*pbyte;
	bfn.bfReserved1=0;
	bfn.bfReserved2=0;
	bfn.bfOffBits=14+40/*sizeof(BITMAPFILEHEADER) +sizeof(BITMAPINFOHEADER)*/ +
			      palsize*4/*sizeof(RGBQUAD)*/;

	if((fp=fopen(fname,"wb"))==NULL) {
		goto $abort1;
	}

	fwriteWORD(bfn.bfType,fp);
	fwriteDWORD(bfn.bfSize,fp);
	fwriteWORD(bfn.bfReserved1,fp);
	fwriteWORD(bfn.bfReserved2,fp);
	fwriteDWORD(bfn.bfOffBits,fp);
	fwriteDWORD(40/*sizeof(BITMAPINFOHEADER)*/,fp);	//biSize
	fwriteDWORD(w,fp);		// biWidth
	fwriteDWORD(h,fp);		// biHeight
	fwriteWORD(1,fp);		// biPlanes
	fwriteWORD(depth,fp);	// biBitCount
	fwriteDWORD(BI_RGB,fp);	// biCompression
	fwriteDWORD(0,fp);	// biSizeImage
	fwriteDWORD(300,fp);	// biXPelsPerMeter
	fwriteDWORD(300,fp);	// biYPelsPerMeter
	fwriteDWORD(0,fp);		// biClrUsed
	fwriteDWORD(0,fp);		// biClrImportant


	pad=rw-w*depth/8;

	for(y=h-1;y>=0;y--) {
		for(x=0;x<w;x++) {
			getPixel(img,x,y,&pix);
			fputc(pix.b,fp);
			fputc(pix.g,fp);
			fputc(pix.r,fp);
		}

		for(i=0;i<pad;i++) {
			fputc(0,fp);
		}
	}
	return 0;
$abort1:
	return 1;
$abort2:
	fclose(fp);
	return 1;
}
