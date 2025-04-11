#include "correction.h"

Correction::Correction(PNM* img) :
    Transformation(img)
{
}

Correction::Correction(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* Correction::transform()
{
    float shift  = getParameter("shift").toFloat();
    float factor = getParameter("factor").toFloat();
    float gamma  = getParameter("gamma").toFloat();

    int width  = image->width();
    int height = image->height();

    for (int i = 0; i < PIXEL_VAL_MAX + 1; i++)
    {
        LUT[i] = qBound(0, int((i + shift) * factor), PIXEL_VAL_MAX);
        LUT[i] = qBound(0, int(pow(LUT[i], gamma)), PIXEL_VAL_MAX);
    }

    PNM* newImage = new PNM(width, height, image->format());
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
        {
            QRgb pixel = image->pixel(x, y);
            QColor newPixel = QColor(LUT[qRed(pixel)], LUT[qGreen(pixel)], LUT[qBlue(pixel)]);
            newImage->setPixel(x, y, newPixel.rgb());
        }

    return newImage;
}
