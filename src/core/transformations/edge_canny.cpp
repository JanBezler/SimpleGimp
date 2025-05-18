#include "edge_canny.h"

#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"

EdgeCanny::EdgeCanny(PNM* img) :
    Convolution(img)
{
}

EdgeCanny::EdgeCanny(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* EdgeCanny::transform()
{
    int width = image->width();
    int height = image->height();

    int upper_thresh = getParameter("upper_threshold").toInt();
    int lower_thresh = getParameter("lower_threshold").toInt();

    ConversionGrayscale grayscale(image);
    PNM* grayImage = grayscale.transform();

    BlurGaussian gaussianBlur(grayImage);
    gaussianBlur.setParameter("size", 3);
    gaussianBlur.setParameter("sigma", 1.6);
    PNM* blurredImage = gaussianBlur.transform();

    EdgeSobel sobel(blurredImage);
    math::matrix<float>* Gx = sobel.rawHorizontalDetection();
    math::matrix<float>* Gy = sobel.rawVerticalDetection();

    math::matrix<float> magnitude(width, height);
    math::matrix<float> direction(width, height);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float gx = (*Gx)(x, y);
            float gy = (*Gy)(x, y);

            magnitude(x, y) = std::sqrt(gx * gx + gy * gy);
            direction(x, y) = std::atan2(gy, gx) * 180.0 / M_PI;
            if (direction(x, y) < 0)
                direction(x, y) += 180;
        }
    }

    math::matrix<float> suppressed(width, height);
    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            float angle = direction(x, y);
            float mag = magnitude(x, y);
            float neighbor1 = 0, neighbor2 = 0;

            if ((angle >= 0 && angle < 22.5) || (angle >= 157.5 && angle <= 180))
            {
                neighbor1 = magnitude(x - 1, y);
                neighbor2 = magnitude(x + 1, y);
            }
            else if (angle >= 22.5 && angle < 67.5)
            {
                neighbor1 = magnitude(x - 1, y - 1);
                neighbor2 = magnitude(x + 1, y + 1);
            }
            else if (angle >= 67.5 && angle < 112.5)
            {
                neighbor1 = magnitude(x, y - 1);
                neighbor2 = magnitude(x, y + 1);
            }
            else if (angle >= 112.5 && angle < 157.5)
            {
                neighbor1 = magnitude(x - 1, y + 1);
                neighbor2 = magnitude(x + 1, y - 1);
            }

            if (mag >= neighbor1 && mag >= neighbor2) suppressed(x, y) = mag;
            else suppressed(x, y) = 0;
        }
    }

    PNM* newImage = new PNM(width, height, QImage::Format_Grayscale8);
    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            float value = suppressed(x, y);

            if (value >= upper_thresh)
            {
                newImage->setPixel(x, y, qRgb(255, 255, 255));
            }
            else if (value >= lower_thresh)
            {
                bool connected = false;
                for (int dy = -1; dy <= 1; dy++)
                {
                    for (int dx = -1; dx <= 1; dx++)
                    {
                        if (suppressed(x + dx, y + dy) >= upper_thresh)
                        {
                            connected = true;
                            break;
                        }
                    }
                    if (connected) break;
                }

                if (connected) newImage->setPixel(x, y, qRgb(255, 255, 255));
                else newImage->setPixel(x, y, qRgb(0, 0, 0));
            }
            else newImage->setPixel(x, y, qRgb(0, 0, 0));
        }
    }

    delete grayImage;
    delete blurredImage;
    delete Gx;
    delete Gy;
    return newImage;
}