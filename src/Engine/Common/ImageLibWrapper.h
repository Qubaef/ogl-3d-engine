#pragma once

struct ImageLibWrapper
{
	static unsigned char* image_load(char const* filename, int* x, int* y, int* comp, int req_comp);
	static void image_free(void* retval_from_stbi_load);
	static int image_write_tga(char const* filename, int x, int y, int comp, const void* data);
};
