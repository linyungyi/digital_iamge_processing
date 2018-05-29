/* List1-3
   ‰æ‘œƒf[ƒ^‚Ì”pŠü
 */
void disposeImage(ImageData *img)
{
	if(img->pixels!=NULL) free(img->pixels);
	free(img);
	return;
}
