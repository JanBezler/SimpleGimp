#include "histogram_stretching.h"
#include <iostream>

#include "../histogram.h"

HistogramStretching::HistogramStretching(PNM* img) :
    Transformation(img)
{
}

HistogramStretching::HistogramStretching(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HistogramStretching::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());
    Histogram* histogram = image->getHistogram();

    int leftR = histogram->minimumValue(Histogram::RChannel, true);
    int rightR = histogram->maximumValue(Histogram::RChannel, true);
    int leftG = histogram->minimumValue(Histogram::GChannel, true);
    int rightG = histogram->maximumValue(Histogram::GChannel, true);
    int leftB = histogram->minimumValue(Histogram::BChannel, true);
    int rightB = histogram->maximumValue(Histogram::BChannel, true);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            QRgb pixel = image->pixel(x, y);
            int r = ((qRed(pixel) - leftR) * 255 / (rightR - leftR));
            int g = ((qGreen(pixel) - leftG) * 255 / (rightG - leftG));
            int b = ((qBlue(pixel) - leftB) * 255 / (rightB - leftB));
            newImage->setPixel(x, y, qRgb(r, g, b));
        }
    }
    return newImage;
}

