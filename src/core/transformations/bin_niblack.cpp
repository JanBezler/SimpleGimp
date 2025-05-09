#include "bin_niblack.h"

BinarizationNiblack::BinarizationNiblack(PNM* img) :
    Transformation(img)
{
}

BinarizationNiblack::BinarizationNiblack(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationNiblack::transform()
{
    int width = image->width();
    int height = image->height();

    int    r = getParameter("r").toInt();
    double a = getParameter("a").toDouble();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            math::matrix<float> window = getWindow(x, y, r, Transformation::Channel::LChannel, Transformation::Mode::RepeatEdge);

            double sum = 0.0;
            double sumSq = 0.0;
            int windowSize = window.rowno() * window.colno();

            for (int i = 0; i < window.rowno(); i++)
            {
                for (int j = 0; j < window.colno(); j++)
                {
                    double value = window(i, j);
                    sum += value;
                    sumSq += value * value;
                }
            }

            double mean = sum / windowSize;
            double stddev = std::sqrt((sumSq / windowSize) - (mean * mean));
            double threshold = mean + a * stddev;
            newImage->setPixel(x, y, qGray(image->pixel(x, y)) > threshold ? Qt::color1 : Qt::color0);
        }
    }

    return newImage;
}
