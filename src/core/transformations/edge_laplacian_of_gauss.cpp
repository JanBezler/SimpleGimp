﻿#include "edge_laplacian_of_gauss.h"

#include "blur_gaussian.h"

EdgeLaplaceOfGauss::EdgeLaplaceOfGauss(PNM* img) :
    Convolution(img)
{
}

EdgeLaplaceOfGauss::EdgeLaplaceOfGauss(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

math::matrix<float> EdgeLaplaceOfGauss::getMask(int, Mode)
{
    size = getParameter("size").toInt();
    double sigma = getParameter("sigma").toDouble();

    math::matrix<float> mask(size, size);
    int center = static_cast<int>(size / 2);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int x = i - center;
            int y = j - center;
            mask(i, j) = getLoG(x, y, sigma);
        }
    }
    return mask;
}

float EdgeLaplaceOfGauss::getLoG(int x, int y, float s)
{
    return ((x * x + y * y - 2) / (s * s)) * BlurGaussian::getGauss(x, y, s);
}

int EdgeLaplaceOfGauss::getSize()
{
    return size;
}
