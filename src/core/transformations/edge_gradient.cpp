#include "edge_gradient.h"
#include <memory>

EdgeGradient::EdgeGradient(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

EdgeGradient::EdgeGradient(PNM* img) :
    Convolution(img)
{
}

PNM* EdgeGradient::verticalDetection()
{
    return convolute(g_y, RepeatEdge);
}

PNM* EdgeGradient::horizontalDetection()
{
    return convolute(g_x, RepeatEdge);
}

PNM* EdgeGradient::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* image_x = horizontalDetection();
    PNM* image_y = verticalDetection();
    PNM* newImage = new PNM(width, height, image->format());

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            QRgb pixel_x = image_x->pixel(x, y);
            QRgb pixel_y = image_y->pixel(x, y);

            newImage->setPixel(x, y, qRgb(
                std::clamp(static_cast<int>(std::sqrt(std::pow(qRed(pixel_x), 2) + std::pow(qRed(pixel_y), 2))), 0, 255),
                std::clamp(static_cast<int>(std::sqrt(std::pow(qGreen(pixel_x), 2) + std::pow(qGreen(pixel_y), 2))), 0, 255),
                std::clamp(static_cast<int>(std::sqrt(std::pow(qBlue(pixel_x), 2) + std::pow(qBlue(pixel_y), 2))), 0, 255)
            ));
        }
    }

    delete image_x;
    delete image_y;

    return newImage;
}

