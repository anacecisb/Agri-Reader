#include "ivdimage.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

IVDImage::IVDImage()
{
    buffer = NULL;
}

IVDImage::~IVDImage()
{
    clearBuffer();
}


unsigned int IVDImage::bufferSize()
{
    return width * height * 2;
}


const unsigned char* IVDImage::getBuffer()
{
    return buffer;
}


void IVDImage::setBuffer(const unsigned char *inBuffer, int width, int height)
{
    clearBuffer();
    unsigned int bufferSize = width * height * 2;

    buffer = (unsigned char*) malloc(bufferSize);
    memcpy( buffer, inBuffer, bufferSize );

    this->width = width;
    this->height = height;
}


void IVDImage::clearBuffer()
{
    if ( buffer != NULL )
    {
        free(buffer);
        buffer = NULL;
    }
}
