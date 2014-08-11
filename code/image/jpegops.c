#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include "../../../external/jpeg-9/jpeglib.h"
#ifndef IMAGELIB_NO_PNG
#include "../../../external/lpng/png.h"
#include "../../../external/lpng/pngstruct.h"
#include <assert.h>
#endif

struct cDataManagerJPEGError
{
	struct jpeg_error_mgr	pub;
	jmp_buf			setjmp_buffer;
};

struct MyPNGMemStream
{
	char* buff;
	int offset;
	int size;
};

void JPEGErrorExit(j_common_ptr cinfo)
{
	struct cDataManagerJPEGError* myerr = (struct cDataManagerJPEGError*)cinfo->err;
	//(*cinfo->err->output_message)(cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}


unsigned char* myJpegLoadFunc(unsigned int* outWidth, unsigned int* outHeight, unsigned int* outChannels, const unsigned char* dataPtr, unsigned int dataSize, unsigned int yUp)
{
	struct jpeg_decompress_struct cinfo;
	struct cDataManagerJPEGError	jerr;
	int row_stride;
	JSAMPARRAY buffer;
	unsigned char* img;
	int y = 0;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = JPEGErrorExit;
	
	if(setjmp(jerr.setjmp_buffer) ) {
		jpeg_destroy_decompress(&cinfo);
		return 0;
	}

	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, (unsigned char*)dataPtr, dataSize);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)( (j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1 );
	memset(*buffer, 0, sizeof(JSAMPLE) * row_stride);

	if(cinfo.output_components != 1 && cinfo.output_components != 3 && cinfo.output_components != 4){
		jpeg_destroy_decompress(&cinfo);
		return 0;
	}
	img = (unsigned char*)malloc(cinfo.output_components * cinfo.image_width * cinfo.image_height);
	*outChannels = cinfo.output_components;
	*outWidth = cinfo.image_width;
	*outHeight = cinfo.image_height;

	while ( cinfo.output_scanline < cinfo.output_height ) 
	{
		jpeg_read_scanlines(&cinfo, buffer, 1);
		memcpy(&img[(yUp ? cinfo.output_height - 1 - y : y) * row_stride], buffer[0],  row_stride);
		y++;
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	return img;
}

unsigned int myJpegLoadDescFunc(unsigned int* outWidth, unsigned int* outHeight, unsigned int* outChannels, const unsigned char* dataPtr, unsigned int dataSize)
{
	struct jpeg_decompress_struct cinfo;
	struct cDataManagerJPEGError	jerr;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = JPEGErrorExit;

	if(setjmp(jerr.setjmp_buffer) ) {
		jpeg_destroy_decompress(&cinfo);
		return 0;
	}

	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, (unsigned char*)dataPtr, dataSize);
	jpeg_read_header(&cinfo, TRUE);
	*outChannels = cinfo.num_components;
	*outWidth = cinfo.image_width;
	*outHeight = cinfo.image_height;
	jpeg_destroy_decompress(&cinfo);
	return 1;
}


#ifndef IMAGELIB_NO_PNG
void MyPngReadDataFromMem(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead)
{
	struct MyPNGMemStream* reader;

	if(png_ptr->io_ptr == 0)
		return;

	reader = (struct MyPNGMemStream*)png_get_io_ptr(png_ptr);
	assert(reader->offset + byteCountToRead <= reader->size);
	memcpy(outBytes, reader->buff + reader->offset, byteCountToRead);
	reader->offset += byteCountToRead;
}

unsigned char* myPNGLoadFunc(unsigned int* outWidth, unsigned int* outHeight, unsigned int* outChannels, unsigned int* outChannelBits, const unsigned char* dataPtr, unsigned int dataSize, unsigned int yUp)
{
	png_uint_32 width = 0, height = 0;
	int bit_depth = 0;
	int color_type = 0; 
	png_structp png_ptr;
	png_infop info_ptr;
	png_byte* data;
	png_uint_32 row_bytes;
	png_byte **row_pointers;
	struct MyPNGMemStream reader;
	unsigned int i;

	reader.buff = dataPtr;
	reader.size = dataSize;
	reader.offset = 8;

	if(!png_check_sig(dataPtr, 8))
		return 0;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	info_ptr = png_create_info_struct(png_ptr);

	png_set_read_fn(png_ptr, &reader, MyPngReadDataFromMem);

	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);
	
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);
	
	if(color_type == PNG_COLOR_TYPE_PALETTE && bit_depth <= 8) png_set_palette_to_rgb(png_ptr);
	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);
	if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
	
	png_read_update_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);
	*outWidth = width;
	*outHeight = height;
	*outChannelBits = bit_depth;
	if(bit_depth != 8 && bit_depth != 16){
		png_destroy_read_struct(&png_ptr, 0, 0);
		return 0;
	}

	switch(color_type)
	{
	case PNG_COLOR_TYPE_GRAY:
		*outChannels = 1;
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		*outChannels = 2;
		break;
	case PNG_COLOR_TYPE_RGB:
		*outChannels = 3;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		*outChannels = 4;
		break;
	default:
		png_destroy_read_struct(&png_ptr, 0, 0);
		return 0;
	}
	
	row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	data = (png_byte*)malloc(row_bytes * height);
	row_pointers = (png_byte**)malloc(height * sizeof(png_byte *));
	for(i = 0; i < height; i++)
		row_pointers[yUp ? height - i - 1 : i] = data + i * row_bytes;

	png_read_image(png_ptr, row_pointers);

	free(row_pointers);
	png_destroy_read_struct(&png_ptr, 0, 0);
	return data;
}

unsigned int myPNGLoadDescFunc(unsigned int* outWidth, unsigned int* outHeight, unsigned int* outChannels, unsigned int* outChannelBits, const unsigned char* dataPtr, unsigned int dataSize)
{
	png_uint_32 width = 0, height = 0;
	int bit_depth = 0;
	int color_type = 0;
	png_structp png_ptr;
	png_infop info_ptr;
	struct MyPNGMemStream reader;

	reader.buff = dataPtr;
	reader.size = dataSize;
	reader.offset = 8;

	if(!png_check_sig(dataPtr, 8))
		return 0;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	info_ptr = png_create_info_struct(png_ptr);

	png_set_read_fn(png_ptr, &reader, MyPngReadDataFromMem);

	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);
	*outWidth = width;
	*outHeight = height;
	*outChannelBits = bit_depth < 8 ? 8 : bit_depth;

	switch(color_type)
	{
	case PNG_COLOR_TYPE_GRAY:
		*outChannels = 1;
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		*outChannels = 2;
		break;
	case PNG_COLOR_TYPE_RGB:
		*outChannels = 3;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		*outChannels = 4;
		break;
	case PNG_COLOR_TYPE_PALETTE:
		*outChannels = 3;
		break;
	default:
		return 0;
	}

	return 1;
}

#endif