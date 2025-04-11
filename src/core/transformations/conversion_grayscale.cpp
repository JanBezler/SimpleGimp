#include "conversion_grayscale.h"

ConversionGrayscale::ConversionGrayscale(PNM* img) :
    Transformation(img)
{
}

ConversionGrayscale::ConversionGrayscale(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* ConversionGrayscale::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Grayscale8);
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
        {
            QRgb pixel = image->pixel(x, y);
            int mean = (qRed(pixel) * 0.3 + qGreen(pixel) * 0.6 + qBlue(pixel) * 0.1);
            QColor newPixel = QColor(mean, mean, mean);
            newImage->setPixel(x, y, newPixel.rgb());
        }
    
    return newImage;
}
