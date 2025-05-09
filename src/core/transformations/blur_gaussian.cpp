#include "blur_gaussian.h"

BlurGaussian::BlurGaussian(PNM* img) :
    Convolution(img)
{
}

BlurGaussian::BlurGaussian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* BlurGaussian::transform()
{
    emit message("Blurring...");

    int size = getParameter("size").toInt();
    radius = (size/2)+1;
    sigma = getParameter("sigma").toDouble();

    return convolute(getMask(size, Normalize), RepeatEdge);
}

math::matrix<float> BlurGaussian::getMask(int size, Mode)
{
    math::matrix<float> mask(size, size);

    int center = size / 2;
    float sum = 0.0f;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int x = i - center;
            int y = j - center;

            float value = getGauss(x, y, sigma);
            mask(i, j) = value;
            sum += value;
        }
    }

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            mask(i, j) /= sum;

    return mask;
}

float BlurGaussian::getGauss(int x, int y, float sigma)
{    
    return 1.0f / (2.0f * M_PI * sigma * sigma) * std::exp(-(x * x + y * y) / (2.0f * sigma * sigma));
}

