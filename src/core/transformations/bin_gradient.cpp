#include "bin_gradient.h"

BinarizationGradient::BinarizationGradient(PNM* img) :
    Transformation(img)
{
}

BinarizationGradient::BinarizationGradient(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationGradient::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    double numerator = 0.0;
    double denominator = 0.0;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int gx = 0;
            int gy = 0;

            if (x > 0 && x < width - 1) gx = qGray(image->pixel(x + 1, y)) - qGray(image->pixel(x - 1, y));
            if (y > 0 && y < height - 1) gy = qGray(image->pixel(x, y + 1)) - qGray(image->pixel(x, y - 1));

            int gMax = std::max(std::abs(gx), std::abs(gy));
            numerator += gMax * qGray(image->pixel(x, y));
            denominator += gMax;
        }
    }

    double threshold = (denominator != 0) ? (numerator / denominator) : 0;
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            newImage->setPixel(x, y, qGray(image->pixel(x, y)) > threshold ? Qt::color1 : Qt::color0);

    return newImage;
}


