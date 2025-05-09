#include "blur_linear.h"

BlurLinear::BlurLinear(PNM* img) :
    Convolution(img)
{
}

BlurLinear::BlurLinear(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* BlurLinear::transform()
{
    int maskSize = getParameter("size").toInt();
    QList<QVariant> tmpMask = getParameter("mask").toList();
    bool normalize = getParameter("normalize").toBool();

    math::matrix<float> mask(maskSize, maskSize);

    float sum = 0.0f;
    for (int i = 0; i < maskSize; i++)
    {
        for (int j = 0; j < maskSize; j++)
        {
            int index = i * maskSize + j;
            float value = tmpMask.at(index).toDouble();
            mask(i, j) = value;
            sum += value;
        }
    }

    if (normalize && sum != 0.0f)
        for (int i = 0; i < maskSize; i++)
            for (int j = 0; j < maskSize; j++)
                mask(i, j) /= sum;

    return convolute(mask, RepeatEdge);
}
