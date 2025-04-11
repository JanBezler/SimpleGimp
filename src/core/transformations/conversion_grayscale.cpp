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
    if (image->format() == QImage::Format_RGB32)
    {
        // Iterate over image space
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                QRgb pixel = image->pixel(x, y); // Getting the pixel(x,y) value
                int mean = (qRed(pixel) + qGreen(pixel) + qBlue(pixel)) / 3;
                QColor newPixel = QColor(mean * 0.3, mean * 0.6, mean * 0.1);
                newImage->setPixel(x, y, newPixel.rgb());
            }
    }

    return newImage;
}
