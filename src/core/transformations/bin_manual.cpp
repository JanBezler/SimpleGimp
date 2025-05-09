#include "bin_manual.h"

BinarizationManual::BinarizationManual(PNM* img) :
    Transformation(img)
{
}

BinarizationManual::BinarizationManual(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationManual::transform()
{
    const int threshold = getParameter("threshold").toInt();

    const int width  = image->width();
    const int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
            newImage->setPixel(x, y, qGray(image->pixel(x, y)) >= threshold ? Qt::color1 : Qt::color0);
        
    return newImage;
}




