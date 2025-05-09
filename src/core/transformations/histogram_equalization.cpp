#include "histogram_equalization.h"

#include "../histogram.h"

HistogramEqualization::HistogramEqualization(PNM* img) :
    Transformation(img)
{
}

HistogramEqualization::HistogramEqualization(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HistogramEqualization::transform()
{
    const int width = image->width();
    const int height = image->height();
    const int totalPixels = width * height;

    PNM* newImage = new PNM(width, height, image->format());
    Histogram* histogram = image->getHistogram();

    QVector<int> cdfR(256, 0), cdfG(256, 0), cdfB(256, 0);

    auto calculateCDF = [&](Histogram::Channel channel, QVector<int>& cdf) {
        QHash<int, int>* channelHistogram = histogram->get(channel);
        QVector<float> probabilities(256, 0.0f);
        float cumulative = 0.0f;

        for (int i = 0; i < 256; i++)
        {
            probabilities[i] = static_cast<float>(channelHistogram->value(i, 0)) / totalPixels;
            cumulative += probabilities[i];
            cdf[i] = static_cast<int>(cumulative * 255);
        }
        };

    calculateCDF(Histogram::RChannel, cdfR);
    calculateCDF(Histogram::GChannel, cdfG);
    calculateCDF(Histogram::BChannel, cdfB);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            QRgb pixel = image->pixel(x, y);
            int r = cdfR[qRed(pixel)];
            int g = cdfG[qGreen(pixel)];
            int b = cdfB[qBlue(pixel)];
            newImage->setPixel(x, y, qRgb(r, g, b));
        }
    }

    return newImage;
}

