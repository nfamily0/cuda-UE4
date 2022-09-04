#pragma once


#include "cuda_runtime.h"
#include <string>

__host__
void free();

__host__
void init(unsigned char* volumeSource, int tableRange[2], float aColor[3], float bColor[3], float eye[3], float dir[3], float cross[3], float up[3], std::string* errMessage);

__host__
void tick(unsigned char* MyTexture, int tableRange[2], float aColor[3], float bColor[3], float eye[3], float dir[3], float cross[3], float up[3], std::string* errMessage);

__host__
void volumeRender(unsigned char* MyTexture, std::string* errMessage);


cudaError_t addWithCuda(int* c, const int* a, const int* b, unsigned int size, std::string* error_message);