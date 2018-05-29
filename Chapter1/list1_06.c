// List1-6

int readBMPfile(char *fname,ImageData **img)
{
	int i,c;
	int errcode=0;
	BITMAPFILEHEADER BMPFile;
	int	fsize;
	BITMAPINFOHEADER BMPInfo;
	BITMAPCOREHEADER BMPCore;
	int	colors;
	int	colorTableSize;
	int	bitsSize;
	int	BISize;
	int x,y;
	int mx,my,depth;
	int pad;
	int mxb,myb;
	int isPM=FALSE;	
	FILE *fp;

    WORD    HEAD_bfType;
    DWORD   HEAD_bfSize;
    WORD    HEAD_bfReserved1;
    WORD    HEAD_bfReserved2;
    DWORD   HEAD_bfOffBits;
    DWORD   INFO_bfSize;
    Pixel palet[MAXCOLORS];
    Pixel setcolor;

	if((fp=fopen(fname,"rb"))==NULL) {
		return -1;
	}


	if(!freadWORD(&HEAD_bfType,fp)) {
		errcode=-2;
		goto $ABORT;
	}
	if (HEAD_bfType != 0x4d42) {
		errcode=-10;
		goto $ABORT;
	}

	if(!freadDWORD(&HEAD_bfSize,fp)) {
		errcode=-10;
		goto $ABORT;
	}

	if(!freadWORD(&HEAD_bfReserved1,fp)) {
		errcode=-10;
		goto $ABORT;
	}

	if(!freadWORD(&HEAD_bfReserved2,fp)) {
		errcode=-10;
		goto $ABORT;
	}

	if(!freadDWORD(&HEAD_bfOffBits,fp)) {
		errcode=-10;
		goto $ABORT;
	}

	if(!freadDWORD(&INFO_bfSize,fp)) {
		errcode=-10;
		goto $ABORT;
	}


	if (INFO_bfSize == 40/*sizeof(BITMAPINFOHEADER)*/ || INFO_bfSize == 12/*sizeof(BITMAPCOREHEADER)*/) {
		BMPInfo.biSize =	INFO_bfSize;

		if(INFO_bfSize == sizeof(BITMAPCOREHEADER)) {
			WORD tmp;
			isPM =	TRUE;

			if(!freadWORD(&tmp,fp)) {
				errcode=-10;
				goto $ABORT;
			}
			BMPInfo.biWidth=tmp;

			if(!freadWORD(&tmp,fp)) {
				errcode=-10;
				goto $ABORT;
			}
			BMPInfo.biHeight=tmp;

			if(!freadWORD(&(BMPInfo.biPlanes),fp)) {
				errcode=-10;
				goto $ABORT;
			}

			if(!freadWORD(&(BMPInfo.biBitCount),fp)) {
				errcode=-10;
				goto $ABORT;
			}
		}
		else {		

			if(!freadDWORD(&(BMPInfo.biWidth),fp)) {
				errcode=-10;
				goto $ABORT;
			}

			if(!freadDWORD(&(BMPInfo.biHeight),fp)) {
				errcode=-10;
				goto $ABORT;
			}

			if(!freadWORD(&(BMPInfo.biPlanes),fp)) {
				errcode=-10;
				goto $ABORT;
			}

			if(!freadWORD(&(BMPInfo.biBitCount),fp)) {
				errcode=-10;
				goto $ABORT;
			}
		}

		if(!isPM) {

			if(!freadDWORD(&(BMPInfo.biCompression),fp)) {
				errcode=-10;
				goto $ABORT;
			}

			if(!freadDWORD(&(BMPInfo.biSizeImage),fp)) {
				errcode=-10;
				goto $ABORT;
			}

			if(!freadDWORD(&(BMPInfo.biXPelsPerMeter),fp)) {
				errcode=-10;
				goto $ABORT;
			}

			if(!freadDWORD(&(BMPInfo.biYPelsPerMeter),fp)) {
				errcode=-10;
				goto $ABORT;
			}

			if(!freadDWORD(&(BMPInfo.biClrUsed),fp)) {
				errcode=-10;
				goto $ABORT;
			}

			if(!freadDWORD(&(BMPInfo.biClrImportant),fp)) {
				errcode=-10;
				goto $ABORT;
			}
		}
	}
	else {
		errcode=-10;
		goto $ABORT;
	}
	mx=BMPInfo.biWidth;
	my=BMPInfo.biHeight;
	depth=BMPInfo.biBitCount;

	if(depth!=8 && depth!=24) {
		errcode=-3;
		goto $ABORT;
	}

	if(BMPInfo.biCompression!=BI_RGB) {
		errcode=-20;
		goto $ABORT;
	}

	if(BMPInfo.biClrUsed==0) {
		colors	= countOfDIBColorEntries(BMPInfo.biBitCount);
	}
	else {
		colors	= BMPInfo.biClrUsed;
	}



	if (!isPM)	{
		for(i=0;i<colors;i++) {
			// Blue¬•ª
			c=fgetc(fp);
			if(c==EOF) {
				errcode=-10;
				goto $ABORT;
			}
			palet[i].b=c;
			// Green¬•ª
			c=fgetc(fp);
			if(c==EOF) {
				errcode=-10;
				goto $ABORT;
			}
			palet[i].g=c;
			// Red¬•ª
			c=fgetc(fp);
			if(c==EOF) {
				errcode=-10;
				goto $ABORT;
			}
			palet[i].r=c;
			// ?‚Ü‚è
			c=fgetc(fp);
			if(c==EOF) {
				errcode=-10;
				goto $ABORT;
			}
		}
	} else {
		for(i=0;i<colors;i++) {
			c=fgetc(fp);
			if(c==EOF) {
				errcode=-10;
				goto $ABORT;
			}
			palet[i].b=c;
			c=fgetc(fp);
			if(c==EOF) {
				errcode=-10;
				goto $ABORT;
			}
			palet[i].g=c;
			c=fgetc(fp);
			if(c==EOF) {
				errcode=-10;
				goto $ABORT;
			}
			palet[i].r=c;
		}
	}

	*img=createImage(mx,my,24);
	mxb=getDIBxmax(mx,depth);
	pad=mxb-mx*depth/8;

	for(y=my-1;y>=0;y--) {
		for(x=0;x<mx;x++) {
			if(depth==8) {	
				c=fgetc(fp);
				if(c==EOF) {
					errcode=-20;
					goto $ABORT;
				}
				setcolor.r=palet[c].r;
				setcolor.g=palet[c].g;
				setcolor.b=palet[c].b;
			}
			else if(depth==24) {
				c=fgetc(fp);
				if(c==EOF) {
					errcode=-20;
					goto $ABORT;
				}
				setcolor.b=c;
				c=fgetc(fp);
				if(c==EOF) {
					errcode=-20;
					goto $ABORT;
				}
				setcolor.g=c;
				c=fgetc(fp);
				if(c==EOF) {
					errcode=-20;
					goto $ABORT;
				}
				setcolor.r=c;
			}
			setPixel(*img,x,y,&setcolor);
		}
	
		for(i=0;i<pad;i++) {
			c=fgetc(fp);
			if(c==EOF) {
				errcode=-20;
				goto $ABORT;
			}
		}
	}
$ABORT:	
	fclose(fp);
	return errcode;
}

