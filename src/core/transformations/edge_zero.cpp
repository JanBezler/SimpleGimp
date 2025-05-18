#include "edge_zero.h"

#include "edge_laplacian_of_gauss.h"

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img) :
    Convolution(img)
{
}

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* EdgeZeroCrossing::transform()
{
    int width = image->width();
    int height = image->height();

    int size = getParameter("size").toInt();
    double sigma = getParameter("sigma").toDouble();
    int t = getParameter("threshold").toInt();

    EdgeLaplaceOfGauss laplacian(image);
    laplacian.setParameter("size", size);
    laplacian.setParameter("sigma", sigma);
    PNM* logImage = laplacian.transform();

    PNM* newImage = new PNM(width, height, QImage::Format_Grayscale8);
    int v0 = 128;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            math::matrix<float> window = getWindow(x, y, size, LChannel, RepeatEdge);
            float minVal = 255;
            float maxVal = -255;
            for (int i = 0; i < window.rowno(); i++)
            {
                for (int j = 0; j < window.colno(); j++)
                {
                    float value = window(i, j);
                    minVal = std::min(minVal, value);
                    maxVal = std::max(maxVal, value);
                }
            }

            if (minVal < v0 - t && maxVal > v0 + t)
            {
                int pixelValue = qGray(logImage->pixel(x, y));
                newImage->setPixel(x, y, qRgb(pixelValue, pixelValue, pixelValue));
            }
            else newImage->setPixel(x, y, qRgb(0, 0, 0));
        }
    }
    delete logImage;
    return newImage;
}
