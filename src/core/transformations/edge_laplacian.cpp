#include "edge_laplacian.h"

EdgeLaplacian::EdgeLaplacian(PNM* img) :
    Convolution(img)
{
}

EdgeLaplacian::EdgeLaplacian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

math::matrix<float> EdgeLaplacian::getMask(int size, Mode)
{
    math::matrix<float> mask(size, size);

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            mask(i, j) = -1.0f;

    int center = static_cast<int>(size / 2);
    mask(center, center) = size * size - 1;

    return mask;
}

