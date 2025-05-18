#include "edge_roberts.h"

EdgeRoberts::EdgeRoberts(PNM* img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

EdgeRoberts::EdgeRoberts(PNM* img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

void EdgeRoberts::prepareMatrices()
{
    g_x = math::matrix<float>(2, 2);
    g_x(0, 0) = 1.0f;
    g_x(0, 1) = 0.0f;
    g_x(1, 0) = 0.0f;
    g_x(1, 1) = -1.0f;
    g_y = math::matrix<float>(2, 2);
    g_y(0, 0) = 0.0f;
    g_y(0, 1) = 1.0f;
    g_y(1, 0) = -1.0f;
    g_y(1, 1) = 0.0f;
}
