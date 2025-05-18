#include "edge_prewitt.h"

EdgePrewitt::EdgePrewitt(PNM*img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

EdgePrewitt::EdgePrewitt(PNM*img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

void EdgePrewitt::prepareMatrices()
{
    g_x = math::matrix<float>(3, 3);
    g_x(0, 0) = -1.0f;
    g_x(0, 1) = 0.0f;
    g_x(0, 2) = 1.0f;
    g_x(1, 0) = -1.0f;
    g_x(1, 1) = 0.0f;
    g_x(1, 2) = 1.0f;
    g_x(2, 0) = -1.0f;
    g_x(2, 1) = 0.0f;
    g_x(2, 2) = 1.0f;
    g_y = math::matrix<float>(3, 3);
    g_y(0, 0) = -1.0f;
    g_y(0, 1) = -1.0f;
    g_y(0, 2) = -1.0f;
    g_y(1, 0) = 0.0f;
    g_y(1, 1) = 0.0f;
    g_y(1, 2) = 0.0f;
    g_y(2, 0) = 1.0f;
    g_y(2, 1) = 1.0f;
    g_y(2, 2) = 1.0f;
}

