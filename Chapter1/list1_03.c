/* List1-3
   �摜�f�[�^�̔p��
 */
void disposeImage(ImageData *img)
{
	if(img->pixels!=NULL) free(img->pixels);
	free(img);
	return;
}
