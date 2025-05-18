#include "hough.h"

#include "conversion_grayscale.h"
#include "edge_laplacian.h"

Hough::Hough(PNM* img) :
    Transformation(img)
{
}

Hough::Hough(PNM* img, ImageViewer* super) :
    Transformation(img, super)
{
}

PNM* Hough::transform()
{
    int thetaDensity = getParameter("theta_density").toInt();
    bool skipEdgeDetection = getParameter("skip_edge_detection").toBool();

    PNM* grayscaleImage = ConversionGrayscale(image).transform();
    PNM* edgeImage = grayscaleImage;
    if (!skipEdgeDetection) edgeImage = EdgeLaplacian(grayscaleImage).transform();

    int width = edgeImage->width();
    int height = edgeImage->height();
    int rhoMax = static_cast<int>(std::sqrt(width * width + height * height));
    int thetaSize = 180 * thetaDensity;
    math::matrix<int> hough(thetaSize, 2 * rhoMax + 1, 0);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (qGray(edgeImage->pixel(x, y)) > 0)
            {
                for (int k = 0; k < thetaSize; ++k)
                {
                    double theta = k * M_PI / (thetaDensity * 180.0);
                    int rho = static_cast<int>(x * std::cos(theta) + y * std::sin(theta));
                    rho += rhoMax;
                    hough(k, rho)++;
                }
            }
        }
    }

    int maxHough = 0;
    for (int k = 0; k < thetaSize; ++k)
        for (int r = 0; r < 2 * rhoMax + 1; ++r)
            if (hough(k, r) > maxHough) maxHough = hough(k, r);

    PNM* outputImage = new PNM(thetaSize, 2 * rhoMax + 1, QImage::Format_Grayscale8);
    for (int k = 0; k < thetaSize; ++k)
    {
        for (int r = 0; r < 2 * rhoMax + 1; ++r)
        {
            int nc = static_cast<int>(255.0 * hough(k, r) / maxHough);
            outputImage->setPixel(k, r, qRgb(nc, nc, nc));
        }
    }

    return outputImage;
}