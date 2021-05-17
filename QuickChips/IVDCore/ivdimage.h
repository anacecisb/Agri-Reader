#ifndef IVDIMAGE_H
#define IVDIMAGE_H

class IVDImage
{
public:
    IVDImage();
    ~IVDImage();

    unsigned int width;
    unsigned int height;
    unsigned int bufferSize();
    const unsigned char* getBuffer();

    void setBuffer( const unsigned char* inBuffer, int width, int height );

private:
    unsigned char* buffer;

    void clearBuffer();
};

#endif // IVDIMAGE_H
