#include "ImageLibWrapper.h"

// STBImage
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

unsigned char* ImageLibWrapper::image_load(char const* filename, int* x, int* y, int* comp, int req_comp)
{
	return stbi_load(filename, x, y, comp, req_comp);
}

void ImageLibWrapper::image_free(void* retval_from_stbi_load)
{
	stbi_image_free(retval_from_stbi_load);
}

int ImageLibWrapper::image_write_tga(char const* filename, int x, int y, int comp, const void* data)
{
	return stbi_write_tga(filename, x, y, comp, data);
}
