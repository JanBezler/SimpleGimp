#include "edge_sobel.h"

EdgeSobel::EdgeSobel(PNM* img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

EdgeSobel::EdgeSobel(PNM* img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

void EdgeSobel::prepareMatrices()
{
    g_x = math::matrix<float>(3, 3);
    g_x(0, 0) = -1.0f;
    g_x(0, 1) = 0.0f;
    g_x(0, 2) = 1.0f;
    g_x(1, 0) = -2.0f;
    g_x(1, 1) = 0.0f;
    g_x(1, 2) = 2.0f;
    g_x(2, 0) = -1.0f;
    g_x(2, 1) = 0.0f;
    g_x(2, 2) = 1.0f;
    g_y = math::matrix<float>(3, 3);
    g_y(0, 0) = -1.0f;
    g_y(0, 1) = -2.0f;
    g_y(0, 2) = -1.0f;
    g_y(1, 0) = 0.0f;
    g_y(1, 1) = 0.0f;
    g_y(1, 2) = 0.0f;
    g_y(2, 0) = 1.0f;
    g_y(2, 1) = 2.0f;
    g_y(2, 2) = 1.0f;
}

math::matrix<float>* EdgeSobel::rawHorizontalDetection()
{
    math::matrix<float>* x_gradient = new math::matrix<float>(this->image->width(), this->image->height());
    for (int y = 1; y < this->image->height() - 1; ++y)
    {
        for (int x = 1; x < this->image->width() - 1; ++x)
        {
            float gradient = 0.0f;
            for (int i = -1; i <= 1; ++i)
            {
                for (int j = -1; j <= 1; ++j)
                {
                    QRgb pixel = this->image->pixel(x + j, y + i);
                    float intensity = qGray(pixel);
                    gradient += intensity * g_x(i + 1, j + 1);
                }
            }
            (*x_gradient)(x, y) = gradient;
        }
    }
    return x_gradient;
}

math::matrix<float>* EdgeSobel::rawVerticalDetection()
{
    math::matrix<float>* y_gradient = new math::matrix<float>(this->image->width(), this->image->height());
    for (int y = 1; y < this->image->height() - 1; ++y)
    {
        for (int x = 1; x < this->image->width() - 1; ++x)
        {
            float gradient = 0.0f;
            for (int i = -1; i <= 1; ++i)
            {
                for (int j = -1; j <= 1; ++j)
                {
                    QRgb pixel = this->image->pixel(x + j, y + i);
                    float intensity = qGray(pixel);
                    gradient += intensity * g_y(i + 1, j + 1);
                }
            }
            (*y_gradient)(x, y) = gradient;
        }
    }
    return y_gradient;
}