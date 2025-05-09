#include "convolution.h"

/** Overloaded constructor */
Convolution::Convolution(PNM* img) :
    Transformation(img)
{
}

Convolution::Convolution(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

/** Returns a convoluted form of the image */
PNM* Convolution::transform()
{
    return convolute(getMask(3, Normalize), RepeatEdge);
}

/** Returns a sizeXsize matrix with the center point equal 1.0 */
math::matrix<float> Convolution::getMask(int size, Mode mode = Normalize)
{
    math::matrix<float> mask(size, size);

    int center = size / 2;
    mask(center, center) = 1.0f;

    return mask;
}

/** Does the convolution process for all pixels using the given mask. */
PNM* Convolution::convolute(math::matrix<float> mask, Mode mode = RepeatEdge)
{
    int width  = image->width(),
        height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    float weight = sum(mask);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float rAccum = 0.0f;
            float gAccum = 0.0f;
            float bAccum = 0.0f;

            math::matrix<float> windowR = getWindow(x, y, mask.rowno(), RChannel, mode);
            math::matrix<float> windowG = getWindow(x, y, mask.rowno(), GChannel, mode);
            math::matrix<float> windowB = getWindow(x, y, mask.rowno(), BChannel, mode);
            for (int i = 0; i < mask.rowno(); i++)
            {
                for (int j = 0; j < mask.colno(); j++)
                {
                    float maskValue = mask(i, j);
                    rAccum += windowR(i, j) * maskValue;
                    gAccum += windowG(i, j) * maskValue;
                    bAccum += windowB(i, j) * maskValue;
                }
            }

            if (weight != 0.0f)
            {
                rAccum /= weight;
                gAccum /= weight;
                bAccum /= weight;
            }

            int r = std::clamp(static_cast<int>(rAccum), 0, 255);
            int g = std::clamp(static_cast<int>(gAccum), 0, 255);
            int b = std::clamp(static_cast<int>(bAccum), 0, 255);
            newImage->setPixel(x, y, qRgb(r, g, b));
        }
    }

    return newImage;
}

/** Joins to matrices by multiplying the A[i,j] with B[i,j].
  * Warning! Both Matrices must be squares with the same size!
  */
const math::matrix<float> Convolution::join(math::matrix<float> A, math::matrix<float> B)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            C(i, j) = A(i, j) * B(i, j);

    return C;
}

/** Sums all of the matrixes elements */
const float Convolution::sum(const math::matrix<float> A)
{
    float sum = 0.0;

    for (int i = 0; i < A.rowno(); i++)
        for (int j = 0; j < A.colno(); j++)
            sum += A(i, j);


    return sum;
}


/** Returns reflected version of a matrix */
const math::matrix<float> Convolution::reflection(const math::matrix<float> A)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            C(i, j) = A(size - 1 - i, size - 1 - j);

    return C;
}
