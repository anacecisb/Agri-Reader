/*
 * videostreaming.cpp -- preview the camera buffer
 * Copyright © 2015  e-con Systems India Pvt. Limited
 *
 * This file is part of Qtcam.
 *
 * Qtcam is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Qtcam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Qtcam. If not, see <http://www.gnu.org/licenses/>.
 */

#include "videostreaming.h"
#include <QtCore/QCoreApplication>
#include <QtGui/QGuiApplication>
#include <QtWidgets>
#include <QDesktopWidget>
#include <QLabel>
#include <QMessageBox>
#include <QRectF>
#include <fcntl.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

/* MACRO FOR BAYER10 TO RGB24 */
#define R(x, y, w)	tempCu40DestBuffer[0 + 3 * ((x) + (w) * (y))]
#define G(x, y, w)	tempCu40DestBuffer[1 + 3 * ((x) + (w) * (y))]
#define B(x, y, w)	tempCu40DestBuffer[2 + 3 * ((x) + (w) * (y))]

#define Bay(x, y, w) tempCu40SrcBuffer[(x) + (w) * (y)]

//Modified by Nithyesh
/*
 * Previously it was
 * #define CLIP(x) ((x) < 0? 0 :((x) >= 255)? 255 : (x))
 */
#define CLIP(x) (((x) >= 255)? 255 : (x))

/* Jpeg-decode */
#define HEADERFRAME1 0xaf

#ifndef min
 #define min(a,b) ((a)<(b)?(a):(b))
#endif


QStringListModel Videostreaming::resolution;
QStringListModel Videostreaming::stillOutputFormat;
QStringListModel Videostreaming::videoOutputFormat;
QStringListModel Videostreaming::fpsList;
int Videostreaming::deviceNumber;
QString Videostreaming::camDeviceName;

Videostreaming::Videostreaming()
{
    openSuccess = false;
    updateOnce = true;
    m_snapShot = false;
    makeSnapShot = false;
    triggerShot = false;
    correctionDisplay = 0;
    m_VideoRecord = false;
    dotile = 0;
    pf = TJPF_RGB;
    warmup = 1;
    //Modified by Nithyesh
    /*
     * Previously was sf = {1, 1};
     * See http://stackoverflow.com/questions/7007591/syntax-in-assigning-to-map-of-structs
     */
    sf.denom = 1;
    sf.num = 1;
    flags = TJFLAG_NOREALLOC;
    yuvpad = 1;
}

Videostreaming::~Videostreaming()
{
}

void Videostreaming::setDevice(QString deviceName) {

    close();
    deviceName.append(QString::number(deviceNumber,10));
    if(open(deviceName,false)) {
        emit logDebugHandle("Success! Device Opened - "+deviceName);
        m_convertData = v4lconvert_create(fd());
        m_buftype= V4L2_BUF_TYPE_VIDEO_CAPTURE;
        openSuccess = true;
    } else {
        emit logCriticalHandle("Device Opening Failed - "+deviceName);
    }
}

void Videostreaming::getFirstDevice(int deviceNode) {
    deviceNumber = deviceNode;
}

void Videostreaming::getCameraName(QString deviceName){
    camDeviceName=deviceName;
}

void Videostreaming::paint(QPainter *painter)
{
    if(pixmapMutex.tryLock()) {
        if(!m_snapShot) {
            painter->drawPixmap(0,0,qImage);
            qStaticImage = qImage;
        } else {
            painter->drawPixmap(0,0, qStaticImage);
        }
        pixmapMutex.unlock();
    }
}

void Videostreaming::updateFrame(QImage img) {

    if(!m_snapShot && !updateStop) {
        qImage=QPixmap::fromImage(img);
//        update(); // quick window thing
    }
}

double Videostreaming::getTimeInSecs()
{
    struct timeval tv;
    if(gettimeofday(&tv, NULL)<0) return 0.0;
    else return (double)tv.tv_sec+((double)tv.tv_usec/1000000.);
}

void Videostreaming::capFrameOld()
{
//    emit logCriticalHandle("capFrame enter from emit");

    __u32 buftype = m_buftype;
    v4l2_plane planes[VIDEO_MAX_PLANES];
    v4l2_buffer buf;
    unsigned char *tempSrcBuffer = NULL, *tempDestBuffer = NULL, *copyDestBuffer = NULL;
    unsigned char *tempCu130DestBuffer = NULL, *tempCu130SrcBuffer = NULL;
    unsigned char *tempCu40DestBuffer = NULL, *irBuffer = NULL;
    unsigned char *tempLogtechSrcBuffer = NULL, *tempLogtechDestBuffer = NULL;
    unsigned char *displaybuf = NULL;
    unsigned short int *tempCu40SrcBuffer = NULL;
    //Modified by Nithyesh
    //Previously it was int err = 0, x, y;
    int err = 0;
    __u32 x, y;
    bool again, v4l2convert = false;



    memset(planes, 0, sizeof(planes));
    buf.length = VIDEO_MAX_PLANES;
    buf.m.planes = planes;
    if (!dqbuf_mmap(buf, buftype, again)) {
        closeDevice();
        unsigned char *m_data=NULL;
        QImage tempImage(m_data,320,240,QImage::Format_RGB888);
        qImage = QPixmap::fromImage(tempImage);
        //update(); // qt quick window thing
        emit deviceUnplugged("Disconnected","Device Not Found");
        emit logCriticalHandle("Device disconnected");
        return;
    }
    if (again) {
        return;
    }
    if (buf.flags & V4L2_BUF_FLAG_ERROR) {        
        qbuf(buf);
        return;
    }
#if 0
    switch(m_capSrcFormat.fmt.pix.pixelformat) {
        case V4L2_PIX_FMT_YUYV: {
            if((width*height*2) == buf.bytesused){
                validFrame = true;
            }

        }
        break;
        case V4L2_PIX_FMT_SGRBG8:{
            // if bayer - 8 bit camera
            // {
                if ((width*height) == buf.bytesused)
                    validFrame = true;
            // }
            // if bayer - 8 bit + pad camera
            // {
                if ((width*height*2) == buf.bytesused)
                    validFrame = true;
            // }
        }
        break;
        case V4L2_PIX_FMT_MJPEG:{
            validFrame = true;
            break;
        }
        default:
        // To do: for other color spaces
        break;

    }

    if (validFrame != true){
        qbuf(buf);
        qDebug()<<"validFrame != true";
     //   return;
    }
#endif

//    emit logCriticalHandle("capFrame checking camera device names");


//    if (camDeviceName == "e-con's CX3 RDK with M\nT9P031" || camDeviceName == "See3CAM_12CUNIR" || camDeviceName == "See3CAM_CU51")
    if ( camDeviceName == "See3CAM_12CUNIR" || camDeviceName == "See3CAM_CU51")
    {
//        emit logCriticalHandle("capFrame found 12CUNIR");

        tempSrcBuffer = (unsigned char *)malloc(width * height * 2);
        tempDestBuffer = (unsigned char *)malloc(width * height << 1);
        copyDestBuffer = tempDestBuffer;

        memcpy( tempSrcBuffer, m_buffers[buf.index].start[0], buf.bytesused );

        for(__u32 l=0; l<(width*height*2); l = l + 2 ) /* Y16 to YUYV conversion */
        {
            *tempDestBuffer++ = (( (tempSrcBuffer[l] & 0xF0) >> 4) | (tempSrcBuffer[l+1] & 0x0F) << 4);
            *tempDestBuffer++ = 0x80;
        }
        m_capSrcFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        err = v4lconvert_convert(m_convertData, &m_capSrcFormat, &m_capDestFormat,
                                 (unsigned char *)copyDestBuffer, buf.bytesused,
                                 m_capImage->bits(), m_capDestFormat.fmt.pix.sizeimage);
        v4l2convert = true;

    }else if (camDeviceName == "See3CAM_CU40")    {
        tempCu40SrcBuffer = (unsigned short int *)malloc(width * height * 2); // 10 bit bayer - short int used
        tempCu40DestBuffer = (unsigned char *)malloc(width * height * 3);
        memcpy(tempCu40SrcBuffer, m_buffers[buf.index].start[0], (width*height*2));

        for(x = 0; x < width; x += 2)  /* Nearest neighbour interpolation algorithm - y16 to RGB24 conversion */
        {
            for(y = 0; y < height; y += 2)
            {
               B(x, y, width) = B(x + 1, y, width) = B(x, y + 1, width) = B(x + 1, y + 1, width) = CLIP(Bay(x, y, width));
               G(x, y, width) = G(x + 1, y, width) = G(x, y + 1, width) = G(x + 1, y + 1, width) = CLIP(Bay(x + 1, y, width));
               R(x, y, width) = R(x + 1, y, width) = R(x, y + 1, width) = R(x + 1, y + 1, width) = CLIP(Bay(x + 1, y + 1, width));
            }

        }

    }else if(camDeviceName == "See3CAM_CU130" && m_capSrcFormat.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG &&  m_frame > 8) {
        if(buf.bytesused <= HEADERFRAME1) {
            emit logCriticalHandle("Ignoring empty buffer");
            return;
        }

        tempCu130SrcBuffer = (unsigned char *)malloc(width * height << 1);
        tempCu130DestBuffer = (unsigned char *)malloc(width * (height + 8) * 3);
        memcpy(tempCu130SrcBuffer, m_buffers[buf.index].start[0], buf.bytesused);

        if(tempCu130SrcBuffer[0] == 0xFF && tempCu130SrcBuffer[1] == 0xD8){ /* check for valid image */
            if(jpegDecode(&tempCu130DestBuffer, tempCu130SrcBuffer, buf.bytesused) < 0) {
                emit logCriticalHandle("jpeg decode error");
                return;
            }
        }
        else{
            qbuf(buf);
            return;
        }
    }else if(camDeviceName == "HD Pro Webcam C920" && m_capSrcFormat.fmt.pix.pixelformat == V4L2_PIX_FMT_H264)
    {
        tempLogtechSrcBuffer = (unsigned char *)malloc(width * height << 1);
        tempLogtechDestBuffer = (unsigned char *)malloc(width * (height + 8) * 2);
        memcpy(tempLogtechSrcBuffer, m_buffers[buf.index].start[0], buf.bytesused);

        /* decode h264 to yuv420p */
        h264Decode->decodeH264(tempLogtechDestBuffer, tempLogtechSrcBuffer, buf.bytesused);

        v4l2_format tmpSrcFormat = m_capSrcFormat;
        tmpSrcFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
        /* convert yuv420 to RGB */
        err = v4lconvert_convert(m_convertData, &tmpSrcFormat, &m_capDestFormat,
                                         (unsigned char *)tempLogtechDestBuffer, (width* height * 3)/2,
                                         m_capImage->bits(), m_capDestFormat.fmt.pix.sizeimage);        
       v4l2convert = true;
    }
    else{
        err = v4lconvert_convert(m_convertData, &m_capSrcFormat, &m_capDestFormat,
                                 (unsigned char *)m_buffers[buf.index].start[0], buf.bytesused,
                                 m_capImage->bits(), m_capDestFormat.fmt.pix.sizeimage);
        v4l2convert = true;
   }

    if (err != -1 && v4l2convert) {
        displaybuf = m_capImage->bits();        
    }else if (err == -1 && v4l2convert) {
        logCriticalHandle(v4lconvert_get_error_message(m_convertData));        
        qbuf(buf);
        return void();
    }else if(camDeviceName == "See3CAM_CU130" && m_capSrcFormat.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG){
        displaybuf = tempCu130DestBuffer;    
    }else if(camDeviceName == "See3CAM_CU40"){
        displaybuf = tempCu40DestBuffer;
    } 

    unsigned char* asil=(unsigned char*)malloc(m_capDestFormat.fmt.pix.sizeimage+qstrlen(header));    
    memmove(asil, displaybuf, m_capDestFormat.fmt.pix.sizeimage);    
    memmove(asil+qstrlen(header), asil, m_capDestFormat.fmt.pix.sizeimage);    
    memcpy(asil,header,qstrlen(header));        
    QImage *qq = new QImage();
    if(qq->loadFromData(asil,m_capDestFormat.fmt.pix.sizeimage+qstrlen(header),"PPM")) {
        updateFrame(*qq);
        if(m_VideoRecord) {
            if(videoEncoder!=NULL) {
                videoEncoder->encodeImage(*qq);
            }
        }
    } else {        
        logCriticalHandle("Unsupported Format...");        
    }
    free(asil);

    delete qq;
    int tmpRet;    
    if(m_frame > 1 && m_snapShot) {
        bool tmpValue;

        if(formatType == "raw") {
            QFile file(filename);
            if(file.open(QIODevice::WriteOnly)) {     
                tmpRet = file.write((const char*)m_buffers[buf.index].start[0], buf.bytesused);
                if(tmpRet != -1) {
                    tmpValue = true;
                } else {
                    tmpValue = false;
                }
                file.close();
            } else {
                tmpValue = false;
            }
        }else if(formatType == "IR data(8bit BMP)"){            
            irBuffer = (unsigned char *)malloc(width * height/4);
            if(extractIRImage(tempCu40SrcBuffer, irBuffer)){
                QImage qImage2(irBuffer, width/2, height/2, QImage::Format_Indexed8);
                bool tmpRet;
                QImageWriter writer(filename);

                /* For 8 bit bmp, We have to use Format_Indexed8 and set color table */
                QVector<QRgb>table;
                for(int i=0; i<256; i++)
                    table.push_back(qRgb(i,i,i));
                qImage2.setColorTable(table);

                if(!writer.write(qImage2)) {
                    emit logCriticalHandle("Error while saving image:"+writer.errorString());
                    tmpRet = false;
                }
                else {
                    tmpRet = true;
                }
                tmpValue = tmpRet;
            }else{                
                tmpValue = false;
            }
        }else {
            QImage qImage3(displaybuf, width, height,QImage::Format_RGB888);
            bool tmpRet;
            QImageWriter writer(filename);

            if(!writer.write(qImage3)) {
                emit logCriticalHandle("Error while saving image:"+writer.errorString());
                tmpRet = false;
            } else {
                tmpRet = true;
            }
            tmpValue = tmpRet;
        }        
        if(triggerShot) {
            captureSaveTime("Capture time: " +(QString::number((double)captureTime.elapsed()/1000)) + "seconds");
            makeSnapShot = false;
            m_snapShot = false;
            formatSaveSuccess(tmpValue);
        } else {
           captureSaveTime("Capture time: " +(QString::number((double)captureTime.elapsed()/1000)) + "seconds");
            makeSnapShot = false;
            m_snapShot = false;
            if (!((stillSize == lastPreviewSize) && (stillOutFormat == lastFormat)))
            {
                freeBuffer(tempSrcBuffer);
                freeBuffers(tempDestBuffer, copyDestBuffer);
                freeBuffer((unsigned char *)tempCu40SrcBuffer);
                freeBuffer(tempCu40DestBuffer);
                freeBuffer(irBuffer);
                freeBuffer(tempLogtechSrcBuffer);
                freeBuffer(tempLogtechDestBuffer);
                if(tempCu130DestBuffer){ /* To do: need to use freeBuffer call */
                free(tempCu130DestBuffer);  tempCu130DestBuffer = NULL;
                }
                if(tempCu130SrcBuffer){
                   free(tempCu130SrcBuffer); tempCu130SrcBuffer = NULL;
                }                
                stopCapture();
                vidCapFormatChanged(lastFormat);
                setResoultion(lastPreviewSize);
                startAgain();
                formatSaveSuccess(tmpValue);
                return void();
            }
            else{
                formatSaveSuccess(tmpValue);
            }
        }
    }
    getFrameRates();
    freeBuffer((unsigned char *)tempCu40SrcBuffer);
    freeBuffer(tempCu40DestBuffer);
    freeBuffer(irBuffer);
    freeBuffer(tempLogtechSrcBuffer);
    freeBuffer(tempLogtechDestBuffer);
    if(tempCu130DestBuffer){ /* To do: need to use freeBuffer call */
        free(tempCu130DestBuffer);  tempCu130DestBuffer = NULL;
    }
    if(tempCu130SrcBuffer){
       free(tempCu130SrcBuffer); tempCu130SrcBuffer = NULL;
    }   
    freeBuffer(tempSrcBuffer);
    freeBuffers(tempDestBuffer, copyDestBuffer);
    qbuf(buf);
}


bool Videostreaming::capRawY16Frame( unsigned char* returnBuffer )
{
    __u32 buftype = m_buftype;
    v4l2_plane planes[VIDEO_MAX_PLANES];
    v4l2_buffer buf;
    unsigned char *tempSrcBuffer = NULL, *tempDestBuffer = NULL, *copyDestBuffer = NULL;
    unsigned char *tempCu130DestBuffer = NULL, *tempCu130SrcBuffer = NULL;
    unsigned char *tempCu40DestBuffer = NULL, *irBuffer = NULL;
    unsigned char *tempLogtechSrcBuffer = NULL, *tempLogtechDestBuffer = NULL;
    unsigned char *displaybuf = NULL;
    unsigned short int *tempCu40SrcBuffer = NULL;
    //Modified by Nithyesh
    //Previously it was int err = 0, x, y;
    int err = 0;
    __u32 x, y;
    bool again, v4l2convert = false;


//    qDebug("buf.bytesused[%d]", buf.bytesused);
//    if ( buf.bytesused < 1 )
//    {
//        qDebug() << "Aborting capture: buf.bytesused < 1";
//        return false;
//    }


    memset(planes, 0, sizeof(planes));
    buf.length = VIDEO_MAX_PLANES;
    buf.m.planes = planes;
    if (!dqbuf_mmap(buf, buftype, again)) {

        closeDevice();
        unsigned char *m_data=NULL;
        QImage tempImage(m_data,320,240,QImage::Format_RGB888);
        qImage = QPixmap::fromImage(tempImage);
        //update(); // qt quick window thing
//        emit deviceUnplugged("Disconnected","Device Not Found");
//        emit logCriticalHandle("Device disconnected");
//        qDebug("Disconnected, Device not found");
        qDebug("Aborting capture: dqbuff error");
        return false;
    }

    if (buf.flags & V4L2_BUF_FLAG_ERROR) {
        qDebug("Aborting capture: V4L2 buff flag error");
        qbuf(buf);
        return false;
    }

//    if (camDeviceName == "e-con's CX3 RDK with M\nT9P031" || camDeviceName == "See3CAM_12CUNIR" || camDeviceName == "See3CAM_CU51")
//    if ( camDeviceName == "See3CAM_12CUNIR" || camDeviceName == "See3CAM_CU51")

    // raw Y16 image handling, and conversion...
    tempSrcBuffer = (unsigned char *)malloc(width * height * 2);
    tempDestBuffer = (unsigned char *)malloc(width * height << 1);
    copyDestBuffer = tempDestBuffer;

//    qDebug("prembuf.bytesused[%d]", buf.bytesused );
    memcpy( tempSrcBuffer, m_buffers[buf.index].start[0], buf.bytesused );
//    qDebug("postmbuf.bytesused[%d]", buf.bytesused );

    if ( returnBuffer != NULL )
    {
//        qDebug("buf.bytesused[%d]", buf.bytesused );
        memcpy( returnBuffer, tempSrcBuffer, buf.bytesused );
        for ( int i = 0; i < buf.bytesused; i += 345 )
        {
//            qDebug("%d <- %d", returnBuffer[i], tempSrcBuffer[i] );
        }
    }


    for( __u32 l=0; l < (width*height*2); l = l + 2 ) /* Y16 to YUYV conversion */
    {
        *tempDestBuffer++ = (( (tempSrcBuffer[l] & 0xF0) >> 4) | (tempSrcBuffer[l+1] & 0x0F) << 4);
        *tempDestBuffer++ = 0x80;
    }
    m_capSrcFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

    err = v4lconvert_convert(m_convertData, &m_capSrcFormat, &m_capDestFormat,
                             (unsigned char *)copyDestBuffer, buf.bytesused,
                             m_capImage->bits(), m_capDestFormat.fmt.pix.sizeimage);

    v4l2convert = true;


    if (err != -1 && v4l2convert)
    {
        displaybuf = m_capImage->bits();
    }
    else if (err == -1 && v4l2convert)
    {
        qDebug() << "Aborting capture: Error v4l2convert data";
        logCriticalHandle(v4lconvert_get_error_message(m_convertData));
        qbuf(buf);
        return false;
    }

    uint headerLength = qstrlen(header);
//    qDebug("headerLength[%d]", headerLength );
    unsigned char* asil=(unsigned char*)malloc( m_capDestFormat.fmt.pix.sizeimage + headerLength );
    memmove(asil, displaybuf, m_capDestFormat.fmt.pix.sizeimage);
    memmove(asil + headerLength, asil, m_capDestFormat.fmt.pix.sizeimage);
    memcpy(asil, header, headerLength);
    QImage *qq = new QImage();
    if ( qq->loadFromData( asil, m_capDestFormat.fmt.pix.sizeimage + qstrlen(header), "PPM" ) )
    {
        updateFrame(*qq);
//        if(m_VideoRecord)
//        {
//            if(videoEncoder!=NULL)
//            {
//                videoEncoder->encodeImage(*qq);
//            }
//        }
    } else
    {
        logCriticalHandle("Unsupported Format...");
    }
    free(asil);

    delete qq;

    getFrameRates();
    freeBuffer((unsigned char *)tempCu40SrcBuffer);
    freeBuffer(tempCu40DestBuffer);
    freeBuffer(irBuffer);
    freeBuffer(tempLogtechSrcBuffer);
    freeBuffer(tempLogtechDestBuffer);
    if(tempCu130DestBuffer){ /* To do: need to use freeBuffer call */
        free(tempCu130DestBuffer);  tempCu130DestBuffer = NULL;
    }
    if(tempCu130SrcBuffer){
       free(tempCu130SrcBuffer); tempCu130SrcBuffer = NULL;
    }
    freeBuffer(tempSrcBuffer);
    freeBuffers(tempDestBuffer, copyDestBuffer);
    qbuf(buf);

    return true;
}

QPixmap* Videostreaming::convertRawY16Frame(unsigned char *rawBuffer)
{
//    v4l2_buffer buf;
    unsigned char *tempSrcBuffer = NULL, *tempDestBuffer = NULL, *copyDestBuffer = NULL;
    unsigned char *displaybuf = NULL;
    //Modified by Nithyesh
    //Previously it was int err = 0, x, y;
    int err = 0;
    __u32 x, y;
    bool v4l2convert, again = false;

//    qDebug() << "convertRawY16 etner";

    int bytesForImage = width * height * 2;

    tempDestBuffer = (unsigned char *)malloc(width * height << 1);
    copyDestBuffer = tempDestBuffer;

    for( __u32 l=0; l < bytesForImage; l = l + 2 ) /* Y16 to YUYV conversion */
    {
        *tempDestBuffer++ = (( (rawBuffer[l] & 0xF0) >> 4) | (rawBuffer[l+1] & 0x0F) << 4);
        *tempDestBuffer++ = 0x80;
    }
    m_capSrcFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

//    qDebug() << "v4lconvert";
    // convert to rgb24 here
    err = v4lconvert_convert(m_convertData, &m_capSrcFormat, &m_capDestFormat,
                             (unsigned char *)copyDestBuffer, bytesForImage,
                             m_capImage->bits(), m_capDestFormat.fmt.pix.sizeimage);

    v4l2convert = true;


    if (err != -1 && v4l2convert)
    {
        displaybuf = m_capImage->bits();
    }
    else if (err == -1 && v4l2convert)
    {
        qDebug() << "Aborting conversion: Error v4l2convert data";
//        logCriticalHandle(v4lconvert_get_error_message(m_convertData));
//        qbuf(buf);
        return false;
    }

    uint headerLength = qstrlen(header);
//    qDebug("headerLength[%d]", headerLength );
    unsigned char* asil=(unsigned char*)malloc( m_capDestFormat.fmt.pix.sizeimage + headerLength );
    memmove(asil, displaybuf, m_capDestFormat.fmt.pix.sizeimage);
    memmove(asil + headerLength, asil, m_capDestFormat.fmt.pix.sizeimage);
    memcpy(asil, header, headerLength);
    QImage *qq = new QImage();
    if ( qq->loadFromData( asil, m_capDestFormat.fmt.pix.sizeimage + qstrlen(header), "PPM" ) )
    {
//        qDebug() << "convertRawY16 doing conversion";
        qImage = QPixmap::fromImage(*qq);

    } else
    {
        logCriticalHandle("Unsupported Format...");
    }
//    qDebug() << "convertRawY16 freeing asil";
    free(asil);
//    qDebug() << "convertRawY16 freeing tempDestBuffer";
    freeBuffers(tempDestBuffer,copyDestBuffer);

    delete qq;

//    qDebug() << "convertRawY16 returning &qImage...";
    return &qImage;
}

void Videostreaming::freeBuffer(unsigned char *ptr)
{
    if(ptr) {
        free(ptr); ptr = NULL;
    }
}

//Modified by Nithyesh
//Changed type of imgHeight and imgWidth from int to __u32
bool Videostreaming::extractIRImage(unsigned short int *srcBuffer, unsigned char *irBuffer)
{
    bool ret = 1;
    unsigned int irBufferLocation = 0;

    if(srcBuffer != NULL && irBuffer != NULL)
    {
        for(__u32 imgHeight = 1; imgHeight < height; imgHeight += 2)
        {
            for(__u32 imgWidth = 0; imgWidth < width; imgWidth += 2)
            {
                irBuffer[irBufferLocation++] = srcBuffer[(imgHeight * width) + imgWidth] >> 2;
            }
        }        
    }
    else
    {
        ret = 0;
    }
    return ret;
}


int Videostreaming::jpegDecode(unsigned char **pic, unsigned char *buf, unsigned long bytesUsed)
{
    tjhandle handle = NULL;
    tjtransform *t = NULL;

    int w = 0, h = 0, subsamp = -1, _w, _h;
    int i, tilew, tileh, ntilesw = 1, ntilesh =1 , retval = 0;
    int _tilew, _tileh, xformopt=0;

    unsigned char **jpegbuf = NULL, *srcbuf = NULL;
    unsigned long *jpegsize = NULL, srcSize;

    srcSize = bytesUsed;

    if((srcbuf=(unsigned char *)malloc(srcSize))==NULL)
        logDebugHandle("allocating memory");

    memcpy(srcbuf,buf,srcSize);

    if((handle = tjInitTransform()) == NULL)
        logDebugHandle("executing tjInitTransform()");

    if(tjDecompressHeader2(handle, srcbuf, srcSize, &w, &h, &subsamp)==-1)
        logDebugHandle("tjDecompressHeader3()");

    for(tilew=dotile? 16:w, tileh=dotile? 16:h; ; tilew*=2, tileh*=2)
    {
        if(tilew>w)
            tilew = w;

        if(tileh>h)
            tileh = h;

        ntilesw = (w+tilew-1) / tilew;
        ntilesh = (h+tileh-1) / tileh;

        if((jpegbuf = (unsigned char **)malloc(sizeof(unsigned char *)
                                             *ntilesw*ntilesh)) == NULL)
            logDebugHandle("allocating JPEG tile array ");

        memset(jpegbuf, 0, sizeof(unsigned char *)*ntilesw*ntilesh);

        if((jpegsize = (unsigned long *)malloc(sizeof(unsigned long)
                                             *ntilesw*ntilesh)) == NULL)
            logDebugHandle("allocating JPEG size array");

        memset(jpegsize, 0, sizeof(unsigned long)*ntilesw*ntilesh);

        if((flags&TJFLAG_NOREALLOC)!=0 || !dotile)
            for(i=0; i<ntilesw*ntilesh; i++)
            {
                if((jpegbuf[i] = (unsigned char *)tjAlloc(tjBufSize(tilew, tileh,
                                                                  subsamp))) == NULL)
                    logDebugHandle("allocating JPEG tiles");
            }

        _tilew = tilew;
        _tileh = tileh;

        _w = w;
        _h = h;

        jpegsize[0] = srcSize;
        memcpy(jpegbuf[0], srcbuf, srcSize); /* Important Step */

        if(w == tilew)
            _tilew = _w;

        if(h == tileh)
            _tileh = _h;

        if(!(xformopt & TJXOPT_NOOUTPUT))
        {
            if(decomp(jpegbuf, jpegsize, NULL, _w, _h, 0,
                      _tilew, _tileh, pic)==-1)
                goto bailout;
        }        

        for(i=0; i<ntilesw*ntilesh; i++)
        {
            tjFree(jpegbuf[i]);
            jpegbuf[i] = NULL;
        }        
        free(jpegbuf);
        jpegbuf = NULL;

        if(jpegsize)
        {
            free(jpegsize);
            jpegsize = NULL;
        }
        if(tilew==w && tileh==h)
            break;
    }

bailout:    
    if(jpegbuf)
    {
        for(i=0; i<ntilesw*ntilesh; i++)
        {
            if(jpegbuf[i])
                tjFree(jpegbuf[i]);

            jpegbuf[i] = NULL;
        }
        free(jpegbuf);
        jpegbuf = NULL;
    }
    if(jpegsize) {
        free(jpegsize);
        jpegsize = NULL;
    }
    if(srcbuf) {
        free(srcbuf);
        srcbuf = NULL;
    }
    if(t) {
        free(t);
        t = NULL;
    }
    if(handle) {
        tjDestroy(handle);
        handle = NULL;
    }
    return retval;
}

/* Decompression test */
int Videostreaming::decomp(unsigned char **jpegbuf,
    unsigned long *jpegsize, unsigned char *dstbuf, int w, int h,
    int jpegqual, int tilew, int tileh,unsigned char **pic)
{
    tjhandle handle = NULL;
    char  qualstr[6] = "\0";

    double elapsed, elapsedDecode;    

    int ps = tjPixelSize[pf];
    int scaledw = TJSCALED(w, sf);
    int scaledh = TJSCALED(h, sf);
    int pitch = scaledw * ps;
    int ntilesw = (w + tilew - 1) / tilew;
    int ntilesh = (h + tileh - 1)/tileh;
    int row, col, iter = 0, retval = 0;

    unsigned char *dstptr, *dstptr2;

    if(jpegqual>0)
    {
        snprintf(qualstr, 6, "_Q%d", jpegqual);
        qualstr[5]=0;
    }

    if((handle = tjInitDecompress()) == NULL){
        emit logCriticalHandle("tjInitDecompress() failed");
        retval = -1;
        goto bailout;
    }

    /* Benchmark */
    iter = -warmup;
    elapsed = elapsedDecode = 0.;

    if(1)
    {
        int tile = 0;
        double start = getTimeInSecs();
        for(row=0, dstptr=dstbuf; row<ntilesh; row++, dstptr+=pitch*tileh)
        {
            for(col=0, dstptr2=dstptr; col<ntilesw; col++, tile++, dstptr2+=ps*tilew)
            {
                int width = dotile? min(tilew, w-col*tilew):scaledw;
                int height = dotile? min(tileh, h-row*tileh):scaledh;

                if(tjDecompress2(handle, jpegbuf[tile], jpegsize[tile], *pic,
                                    width, pitch, height, pf, flags) == -1){
                    emit logCriticalHandle("tjDecompress2() failed");
                    retval = -1;
                    goto bailout;
                }
            }
        }
        iter++;
        if(iter >= 1)
            elapsed += getTimeInSecs()-start;
    }

    if(1) elapsed -= elapsedDecode;

    if(tjDestroy(handle) == -1){
        emit logCriticalHandle("tjDestroy() failed");
        retval = -1;
        goto bailout;
    }

    handle = NULL;

    bailout:    
    if(handle)
        tjDestroy(handle);

    return retval;
}

void Videostreaming::freeBuffers(unsigned char *destBuffer, unsigned char *copyBuffer)
{     
    if(copyBuffer || destBuffer)
    {
        free(copyBuffer);
        copyBuffer = NULL;
        destBuffer = NULL;
    }

}

void Videostreaming::getFrameRates() {
    struct timeval tv, res;
    if (m_frame == 0)
        gettimeofday(&m_tv, NULL);
    gettimeofday(&tv, NULL);
    timersub(&tv, &m_tv, &res);
    if (res.tv_sec) {
        m_fps = (100 * (m_frame - m_lastFrame)) / (res.tv_sec * 100 + res.tv_usec / 10000);
        m_lastFrame = m_frame;
        m_tv = tv;
    }
    ++m_frame;
    emit averageFPS(m_fps);
}

bool Videostreaming::startCapture()
{
    __u32 buftype = m_buftype;
    v4l2_requestbuffers req;
    unsigned int i;

    memset(&req, 0, sizeof(req));

    if (!reqbufs_mmap(req, buftype, 3)) {
        emit logCriticalHandle("Cannot capture");
        return false;
    }

    if (req.count < 2) {
        emit logCriticalHandle("Too few buffers");
        reqbufs_mmap(req, buftype);
        return false;
    }

    m_buffers = (buffer *)calloc(req.count, sizeof(*m_buffers));

    if (!m_buffers) {
        emit logCriticalHandle("Out of memory");
        reqbufs_mmap(req, buftype);
        return false;
    }

    for (m_nbuffers = 0; m_nbuffers < req.count; ++m_nbuffers) {
        v4l2_plane planes[VIDEO_MAX_PLANES];
        v4l2_buffer buf;

        memset(&buf, 0, sizeof(buf));

        buf.type        = buftype;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = m_nbuffers;
        buf.length      = VIDEO_MAX_PLANES;
        buf.m.planes    = planes;

        if (-1 == ioctl(VIDIOC_QUERYBUF, &buf)) {
            perror("VIDIOC_QUERYBUF");
            return false;
        }

        m_buffers[m_nbuffers].planes = 1;
        m_buffers[m_nbuffers].length[0] = buf.length;
        m_buffers[m_nbuffers].start[0] = mmap(buf.length, buf.m.offset);
        if (MAP_FAILED == m_buffers[m_nbuffers].start[0]) {
            perror("mmap");
            return false;
        }
    }
    for (i = 0; i < m_nbuffers; ++i) {
        if (!qbuf_mmap(i, buftype)) {
            emit logCriticalHandle("Querying Buffer failed");
            perror("VIDIOC_QBUF");
            return false;
        }
    }
    if (!streamon(buftype)) {
        emit logCriticalHandle("Stream ON failed");
        perror("VIDIOC_STREAMON");
        return false;
    }
    return true;
}

int Videostreaming::findMax(QList<int> *list) {

    int array[list->count()];
    for(int j=0;j<list->count();j++)
        array[j] = list->at(j);
    int index_of_min = 0;
    for(int x=0; x<list->count(); x++)
    {
        index_of_min = x;
        for(int y=x; y<list->count(); y++)

        {
            if(array[index_of_min] > array[y])
            {
                index_of_min = y;
            }
        }
        int temp = array[x];
        array[x] = array[index_of_min];
        array[index_of_min] = temp;
    }
    return array[index_of_min];
}



void Videostreaming::makeShot(QString filePath,QString imgFormatType) {

    captureTime.start();
    m_snapShot = true;
    QDateTime dateTime = QDateTime::currentDateTime();
    QDir tmpDir;

    /* cu40 - IR image in bmp format */
    if(imgFormatType == "IR data(8bit BMP)"){
        formatType = imgFormatType;
        imgFormatType = "bmp";
    }else{ // other image formats or other cameras
        formatType = imgFormatType;
    }

    if(tmpDir.cd(filePath)) {
        QStringList filters,list;
        filters << "Qtcam-" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"-*"+imgFormatType;
        tmpDir.setNameFilters(filters);
        list << tmpDir.entryList(filters,QDir::Files);
        if(!list.isEmpty()) {
            QList<int> tmpStr;
            for(int i=0;i<list.count();i++) {
                tmpStr.append(list.at(i).split(".").at(0).split("-").at(1).toInt());
            }
            int lastNumber = findMax(&tmpStr);
            lastNumber++;
            filename = filePath +"/Qtcam-" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"-" +QString::number(lastNumber,10)+"."+ imgFormatType;
        } else {
            filename = filePath +"/Qtcam-" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"-" +QString::number(1,10)+"."+ imgFormatType;
        }
    } else {
        QStringList filters,list;
        filters << "Qtcam-" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"-*"+imgFormatType;
        tmpDir.cd(tmpDir.currentPath());
        tmpDir.setNameFilters(filters);
        list << tmpDir.entryList(filters,QDir::Files);
        if(!list.isEmpty()) {
            QList<int> tmpStr;
            for(int i=0;i<list.count();i++) {
                tmpStr.append(list.at(i).split(".").at(0).split("-").at(1).toInt());
            }
            int lastNumber = findMax(&tmpStr);
            lastNumber++;
            filename = "Qtcam-" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"-" +QString::number(lastNumber,10)+"."+ imgFormatType;
        } else {
            filename = "Qtcam-" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"-" +QString::number(1,10)+"."+ imgFormatType;
        }
    }
    makeSnapShot = true;
    triggerShot = false;
   // formatType = imgFormatType;

    if (!((stillSize == lastPreviewSize) && (stillOutFormat == lastFormat)))
    {
        stopCapture();
        vidCapFormatChanged(stillOutFormat);
        setResoultion(stillSize);
        startAgain();
    }
}

void Videostreaming::triggerModeShot(QString filePath,QString imgFormatType) {

    captureTime.restart();
    m_snapShot = true;
    QDateTime dateTime = QDateTime::currentDateTime();
    QDir tmpDir;

    /* cu40 - IR image in bmp format */
    if(imgFormatType == "IR data(8bit BMP)"){
        formatType = imgFormatType;
        imgFormatType = "bmp";
    }else{ // other image formats or other cameras
        formatType = imgFormatType;
    }

    if(tmpDir.cd(filePath)) {
        filename = filePath +"/Qtcam-" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"."+ imgFormatType;
    } else {
        filename = "Qtcam" + dateTime.toString("yy_MM_dd:hh_mm_ss")+"." + imgFormatType;
    }
    makeSnapShot = true;
    triggerShot = true;
    m_frame = 3;
}

void Videostreaming::formatSaveSuccess(bool success) {
    if(success) {
        _title = "Captured";
        _text = "Image saved in the location:" + filename;
        emit logDebugHandle("Still image saved successfully in " + filename);
        emit titleTextChanged(_title,_text);
    } else {
        _title = "Failure";
        _text = "Image not saved in the selected location";
        emit logCriticalHandle("Still image not saved successfully");
        emit titleTextChanged(_title,_text);
    }

//    if(success) {
//        emit logDebugHandle("Still image saved successfully in " + filename);
//        qDebug("able to save image");
//    } else {
//        emit logCriticalHandle("Still image not saved successfully");
//        qDebug("Not able to save image");
//    }
//    emit enableCaptureAndRecord();
}

bool Videostreaming::getInterval(struct v4l2_fract &interval)
{
    if (m_has_interval)
        interval = m_interval;
    return m_has_interval;
}

void Videostreaming::displayFrame() {
    emit logDebugHandle("Videostreaming::displayFrame - enter");
    m_frame = m_lastFrame = m_fps = 0;    
    emit averageFPS(m_fps);

    __u32 buftype = m_buftype;
    g_fmt_cap(buftype, m_capSrcFormat);

    // if (try_fmt(m_capSrcFormat)) {
    if(!s_fmt(m_capSrcFormat)) {        
        emit titleTextChanged("Error", "Device or Resource is busy");
        emit logCriticalHandle("Device or Resource is busy");
        if (fd() >= 0) {
            v4lconvert_destroy(m_convertData);
            close();
        }
        return void();
    }
    //} else {
    //return void();
    //}

    if (getInterval(interval))
        set_interval(buftype, interval);

    emit logDebugHandle("Preview settings completed");

    m_capDestFormat = m_capSrcFormat;
    m_capDestFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;

    //m_capDestFormat.fmt.pix.sizeimage = (m_capDestFormat.fmt.pix.width * m_capDestFormat.fmt.pix.height * 3);
    v4l2_format copy = m_capSrcFormat;
    v4lconvert_try_format(m_convertData, &m_capDestFormat, &m_capSrcFormat);
    // v4lconvert_try_format sometimes modifies the source format if it thinks
    // that there is a better format available. Restore our selected source
    // format since we do not want that happening.

    m_capSrcFormat = copy;
    m_capDestFormat.fmt.pix.width  = m_capSrcFormat.fmt.pix.width;
    m_capDestFormat.fmt.pix.height = m_capSrcFormat.fmt.pix.height;
    width = m_capDestFormat.fmt.pix.width;
    height = m_capDestFormat.fmt.pix.height;
    pixfmt = m_capDestFormat.fmt.pix.pixelformat;
    m_capDestFormat.fmt.pix.sizeimage = width*height*3;

    m_capImage = new QImage(width, height, QImage::Format_RGB888);

    if(camDeviceName == "e-con's 1MP Bayer RGB \nCamera") {
        m_capSrcFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_SGRBG8;
    }
    h264Decode = new H264Decoder();
    h264Decode->initH264Decoder(width, height);

    if (startCapture()) {
        sprintf(header,"P6\n%d %d 255\n",width,height);
        m_capNotifier = new QSocketNotifier(fd(), QSocketNotifier::Read);
        connect(m_capNotifier, SIGNAL(activated(int)), this, SLOT(capFrame()));
    }
}

void Videostreaming::stopCapture() {
    if (fd() >= 0) {
        emit logDebugHandle("Stop Previewing...");
        const char *m_data;
        m_data=NULL;
        QPixmap tempImage(m_data);
        qImage = tempImage;
        v4l2_requestbuffers reqbufs;
        if (m_buffers == NULL)
            return;
        if (!streamoff(V4L2_BUF_TYPE_VIDEO_CAPTURE)) {
            perror("VIDIOC_STREAMOFF");
            emit logCriticalHandle("Stream OFF failed");
        }
        for (uint i = 0; i < m_nbuffers; ++i)
            for (unsigned p = 0; p < m_buffers[i].planes; p++)
                if (-1 == munmap(m_buffers[i].start[p], m_buffers[i].length[p]))
                    perror("munmap");
        // Free all buffers.
        reqbufs_mmap(reqbufs, V4L2_BUF_TYPE_VIDEO_CAPTURE, 1);  // videobuf workaround
        reqbufs_mmap(reqbufs, V4L2_BUF_TYPE_VIDEO_CAPTURE, 0);
        emit logDebugHandle("Value of FD is:"+ QString::number(fd(),10));
        if (m_capNotifier) {
            delete m_capNotifier;
            delete m_capImage;
            m_capNotifier = NULL;
            m_capImage = NULL;
        }
    }
}

void Videostreaming::closeDevice() {
    emit logDebugHandle("Closing the current camera device");
    if (fd() >= 0) {
        if (m_capNotifier) {
            delete m_capNotifier;
            delete m_capImage;
            m_capNotifier = NULL;
            m_capImage = NULL;
        }
        v4lconvert_destroy(m_convertData);
        close();
    }
}

void Videostreaming::startAgain() {
    if(openSuccess) {
        displayFrame();
    }
}

void Videostreaming::lastPreviewResolution(QString resolution,QString format) {
    lastPreviewSize = resolution;
    lastFormat = format;
    emit logDebugHandle("Last Resolution displayed at::"+resolution);
}

void Videostreaming::setResoultion(QString resolution)
{

    emit logDebugHandle("Resolution set at::"+resolution);
    v4l2_format fmt;
    unsigned int width, height;
    QStringList tempResList = resolution.split('x');
    width = tempResList.value(0).toInt();
    height = tempResList.value(1).toInt();
    g_fmt_cap(V4L2_BUF_TYPE_VIDEO_CAPTURE, fmt);
    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;
    m_width = width;
    m_height = height;
    try_fmt(fmt);
    s_fmt(fmt);
}

void Videostreaming::displayStillResolution() {
    g_fmt_cap(V4L2_BUF_TYPE_VIDEO_CAPTURE, fmt);

    QStringList dispStillRes;

    m_pixelformat = fmt.fmt.pix.pixelformat;
    m_width       = fmt.fmt.pix.width;
    m_height      = fmt.fmt.pix.height;

    unsigned int indexCount = -1;
    v4l2_frmsizeenum frmsize;
    bool ok = false;
    ok = enum_framesizes(frmsize, m_pixelformat);
    if (ok && frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
        do {
            indexCount++;
            dispStillRes.append(QString("%1x%2").arg(frmsize.discrete.width).arg(frmsize.discrete.height));
            if (frmsize.discrete.width == m_width && frmsize.discrete.height == m_height) {                
                emit defaultStillFrameSize(indexCount);
            }
        } while (enum_framesizes(frmsize));
    }    
    stillOutputFormat.setStringList(dispStillRes);
    emit logDebugHandle("Supported still Resolution: " +dispStillRes.join(", "));
}

void Videostreaming::displayVideoResolution() {
    g_fmt_cap(V4L2_BUF_TYPE_VIDEO_CAPTURE, fmt);

    QStringList dispVideoRes;

    m_pixelformat = fmt.fmt.pix.pixelformat;
    m_width       = fmt.fmt.pix.width;
    m_height      = fmt.fmt.pix.height;

    unsigned int indexCount = -1;
    unsigned int defaultWidth;
    unsigned int defaultHeight;

    v4l2_frmsizeenum frmsize;
    bool ok = false;
    ok = enum_framesizes(frmsize, m_pixelformat);
    if (ok && frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
        do {
            indexCount++;
            dispVideoRes.append(QString("%1x%2").arg(frmsize.discrete.width).arg(frmsize.discrete.height));
            if (frmsize.discrete.width == m_width && frmsize.discrete.height == m_height) {
                defaultWidth = m_width;
                defaultHeight = m_height;
                emit defaultFrameSize(indexCount, defaultWidth, defaultHeight);                
            }
        } while (enum_framesizes(frmsize));
    }

    videoOutputFormat.setStringList(dispVideoRes);    
    emit logDebugHandle("Supported video Resolution: " +dispVideoRes.join(", "));
}

void Videostreaming::vidCapFormatChanged(QString idx)
{
    v4l2_fmtdesc desc;
    enum_fmt_cap(desc, m_buftype, true, idx.toInt());
    v4l2_format fmt;
    g_fmt_cap(m_buftype, fmt);
    fmt.fmt.pix.pixelformat = desc.pixelformat;
    try_fmt(fmt);
    s_fmt(fmt);
    if(!makeSnapShot)
        updateVidOutFormat();
}

void Videostreaming::updateVidOutFormat()
{
    v4l2_fmtdesc desc;
    v4l2_format fmt;
    g_fmt_cap(m_buftype, fmt);
    m_pixelformat = fmt.fmt.pix.pixelformat;
    m_width       = fmt.fmt.pix.width;
    m_height      = fmt.fmt.pix.height;
    if (enum_fmt_cap(desc, m_buftype, true)) {
        do {
            if (desc.pixelformat == fmt.fmt.pix.pixelformat)
                break;
        } while (enum_fmt_cap(desc, m_buftype));
    }
    if (desc.pixelformat != fmt.fmt.pix.pixelformat)
        return;
    emit defaultOutputFormat(desc.index);
    emit logDebugHandle("Color Space set to: "+pixfmt2s(m_pixelformat));
}

void Videostreaming::displayOutputFormat() {
    QStringList dispOutFormat;
    v4l2_fmtdesc fmt;
    pixFormat.clear();
    if (enum_fmt_cap(fmt, V4L2_BUF_TYPE_VIDEO_CAPTURE, true)) {
        do {
            QString s(pixfmt2s(fmt.pixelformat) + " (");
            pixFormat.insert(pixfmt2s(fmt.pixelformat), QString::number(fmt.pixelformat));
            if (fmt.flags & V4L2_FMT_FLAG_EMULATED)
                dispOutFormat.append(s + "Emulated)");
            else {
                dispOutFormat.append(s + (const char *)fmt.description + ")");
            }
        } while (enum_fmt_cap(fmt, V4L2_BUF_TYPE_VIDEO_CAPTURE));

    }
    emit logDebugHandle("Output format supported: " +dispOutFormat.join(", "));
    //dispOutFormat.sort();
    resolution.setStringList(dispOutFormat);
    updateVidOutFormat();
}

void Videostreaming::updateFrameInterval(QString pixelFormat, QString frameSize)
{
    v4l2_frmivalenum frmival;
    v4l2_fract curr;
    bool curr_ok, ok;
    QStringList tempResList = frameSize.split('x');
    width = tempResList.value(0).toInt();
    height = tempResList.value(1).toInt();
    QStringList tempPixFmt = pixelFormat.split(' ');
    QString pixFmtValue = tempPixFmt.value(0);

    /* Actual Format of "Y16" is "Y16 " [Y16 with space]. So append space char */
    if (0 == QString::compare(pixFmtValue, "Y16")){
        pixFmtValue.append(" ");
    }

    ok = enum_frameintervals(frmival,pixFormat.value(pixFmtValue).toInt(), width, height);
    m_has_interval = ok && frmival.type == V4L2_FRMIVAL_TYPE_DISCRETE;
    QStringList availableFPS;
    availableFPS.clear();
    if (m_has_interval) {
        m_interval = frmival.discrete;
        curr_ok = v4l2::get_interval(m_buftype, curr);
        do {
            availableFPS.append(QString::number((double)frmival.discrete.denominator / frmival.discrete.numerator).append(" FPS"));
            if (curr_ok && frmival.discrete.numerator == curr.numerator && frmival.discrete.denominator == curr.denominator) {                
                emit defaultFrameInterval(frmival.index);
                m_interval = frmival.discrete;
            }
        } while (enum_frameintervals(frmival));
    }
    emit logDebugHandle("Available FPS:"+ availableFPS.join(", "));
    fpsList.setStringList(availableFPS);
}

void Videostreaming::frameIntervalChanged(int idx)
{
    v4l2_frmivalenum frmival;
    emit logDebugHandle("Pixel Format:"+ QString::number(m_pixelformat));
    emit logDebugHandle("Width:"+ QString::number(m_width));
    emit logDebugHandle("Height:"+ QString::number(m_height));
    emit logDebugHandle("IDX Value:"+QString::number(idx));
    if (enum_frameintervals(frmival, m_pixelformat, m_width, m_height, idx)
            && frmival.type == V4L2_FRMIVAL_TYPE_DISCRETE) {
        if (set_interval(m_buftype, frmival.discrete)) {
            m_interval = frmival.discrete;
        }
    }
}

void Videostreaming::cameraFilterControls(bool actualValue) {
    v4l2_queryctrl qctrl;
    v4l2_querymenu qmenu;
    int indexValue;
    qctrl.id = V4L2_CTRL_FLAG_NEXT_CTRL;
    emit logDebugHandle("Available Controls:");    
    while(queryctrl(qctrl)) {
        emit logDebugHandle((char*)qctrl.name);
        switch (qctrl.type) {
        case V4L2_CTRL_TYPE_BOOLEAN:
            ctrlName = (char*)qctrl.name;
            ctrlType = QString::number(qctrl.type,10);
            ctrlID = QString::number(qctrl.id,10);
            if(actualValue) {
                emit newControlAdded(ctrlName,ctrlType,ctrlID,QString::number(0,10),QString::number(1,10),getSettings(qctrl.id));
            } else {
                emit newControlAdded(ctrlName,ctrlType,ctrlID,QString::number(0,10),QString::number(1,10),QString::number(qctrl.default_value,10));
            }
            break;
        case V4L2_CTRL_TYPE_INTEGER:
            ctrlName = (char*)qctrl.name;
            ctrlType = QString::number(qctrl.type,10);
            ctrlID = QString::number(qctrl.id,10);
            ctrlMaxValue = QString::number(qctrl.maximum,10);
            ctrlMinValue = QString::number(qctrl.minimum,10);
            ctrlDefaultValue = QString::number(qctrl.default_value,10);
            if(actualValue) {
                emit newControlAdded(ctrlName,ctrlType,ctrlID,ctrlMinValue,ctrlMaxValue,getSettings(qctrl.id));
            } else {
                emit newControlAdded(ctrlName,ctrlType,ctrlID,ctrlMinValue,ctrlMaxValue,QString::number(qctrl.default_value,10));
            }
            break;
        case V4L2_CTRL_TYPE_MENU:
        case V4L2_CTRL_TYPE_INTEGER_MENU:
            ctrlName = (char*)qctrl.name;
            ctrlType = QString::number(qctrl.type,10);
            ctrlID = QString::number(qctrl.id,10);
            for (int i = qctrl.minimum; i <= qctrl.maximum; i++) {
                qmenu.id = qctrl.id;
                qmenu.index = i;
                if (!querymenu(qmenu))
                    continue;
                if (qctrl.type == V4L2_CTRL_TYPE_MENU) {
                    emit newControlAdded((char *)qmenu.name,ctrlType,ctrlID);
                }
                else {
                    //combo->addItem(QString("%1").arg(qmenu.value));
                    //menuItems.append(qmenu.value);
                }
            }
            indexValue =  getSettings(qctrl.id).toInt();
            if(actualValue) {
                emit newControlAdded(ctrlName,ctrlType,ctrlID,ctrlMinValue,ctrlMaxValue,QString::number(getMenuIndex(qctrl.id,indexValue),10));
            } else {
                emit newControlAdded(ctrlName,ctrlType,ctrlID,ctrlMinValue,ctrlMaxValue,QString::number(getMenuIndex(qctrl.id,qctrl.default_value),10));
            }
            break;
        case V4L2_CTRL_TYPE_BITMASK:
            break;
        case V4L2_CTRL_TYPE_BUTTON:
            break;
        case V4L2_CTRL_TYPE_INTEGER64:
            break;
        case V4L2_CTRL_TYPE_STRING:
            break;
        case V4L2_CTRL_TYPE_CTRL_CLASS:
            break;
        }
        qctrl.id |= V4L2_CTRL_FLAG_NEXT_CTRL;
    }
}

QString Videostreaming::getSettings(unsigned int id) {
    struct v4l2_control c;
    c.id = id;
    //Modified by Nithyesh
    /*
     * Previosuly it was
     * if (ioctl(VIDIOC_G_CTRL, &c)) {
     *      v4l2_queryctrl qctrl;
     *      qctrl.id = id;
     *      emit logCriticalHandle("Unable to get the Value, setting the Default value: "+ QString::number(c.value,10));
     *      return QString::number(c.value,10);
     *  }
     *  QString value = QString::number(c.value,10);
     *  return value;
     */
    c.value = 0;
    if (ioctl(VIDIOC_G_CTRL, &c)) {
        //v4l2_queryctrl qctrl;
        //qctrl.id = id;
        emit logCriticalHandle("Unable to get the Value, setting the Default value: "+ QString::number(c.value,10));
        return QString::number(c.value,10);
    }
    QString value = QString::number(c.value,10);
    return value;
}

void Videostreaming::changeSettings(unsigned int id, QString value) {
    struct v4l2_control c;
    c.id = id;
    c.value = value.toInt();    
    if (ioctl(VIDIOC_S_CTRL, &c)) {
        emit logCriticalHandle("Error in setting the Value");        
    }
}


void Videostreaming::selectMenuIndex(unsigned int id, int value) {
    v4l2_queryctrl qctrl;
    v4l2_querymenu qmenu;
    qctrl.id = id;
    queryctrl(qctrl);
    int i;
    for (i = qctrl.minimum; i <= qctrl.maximum; i++) {
        qmenu.id = qctrl.id;
        qmenu.index = i;
        if (!querymenu(qmenu))
            continue;
        if (value-- == 0)
            break;
    }
    changeSettings(id,QString::number(i,10));
}

int Videostreaming::getMenuIndex(unsigned int id,int value) {
    v4l2_queryctrl qctrl;
    v4l2_querymenu qmenu;
    qctrl.id = id;
    queryctrl(qctrl);
    int i, j = 0;
    for (i = qctrl.minimum; i <= qctrl.maximum; i++) {
        qmenu.id = qctrl.id;
        qmenu.index = i;
        if (!querymenu(qmenu)) {
            value--;
            continue;
        }
        if (value-- == 0)
            break;
        j++;
    }
    return j;
}

void Videostreaming::setStillVideoSize(QString stillValue, QString stillFormat) {
    stillSize = stillValue;
    stillOutFormat = stillFormat;
}

void Videostreaming::recordBegin(int videoEncoderType, QString videoFormatType, QString fileLocation) {
    m_VideoRecord = true;
    videoEncoder=new VideoEncoder();
    if(videoFormatType.isEmpty()) {
        videoFormatType = "avi";        //Application never enters in this condition
    }
    switch(videoEncoderType) {
    case 0:
        videoEncoderType = CODEC_ID_RAWVIDEO;
        break;
    case 1:
        videoEncoderType = CODEC_ID_MJPEG;
        break;
    case 2:
        videoEncoderType = CODEC_ID_H264;
        break;
    case 3:
        videoEncoderType = CODEC_ID_VP8;
        break;
    }

    fileName = fileLocation +"/Qtcam-" + QDateTime::currentDateTime().toString("yy_MM_dd:hh_mm_ss")+"."+ videoFormatType;
    v4l2_frmivalenum frmival;
    enum_frameintervals(frmival, m_pixelformat, m_width, m_height);
    bool tempRet = videoEncoder->createFile(fileName,(CodecID)videoEncoderType, m_capDestFormat.fmt.pix.width,m_capDestFormat.fmt.pix.height,frmival.discrete.denominator,frmival.discrete.numerator,10000000);
    if(!tempRet){
        emit rcdStop("Unable to record the video");
    }
}

void Videostreaming::recordStop() {

    emit videoRecord(fileName);
    m_VideoRecord = false;
    if(videoEncoder!=NULL){
        videoEncoder->closeFile();
        delete videoEncoder;
        videoEncoder=NULL;
    }
}

void Videostreaming::triggerModeEnabled() {
    updateStop = true;
}

void Videostreaming::masterModeEnabled() {
    updateStop = false;
}
