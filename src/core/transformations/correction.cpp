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


    PNM* newImage = new PNM(width, height, image->format());

    for (int x=0; x<PIXEL_VAL_MAX+1; x++)
    {
        int v = x;
        v = (v + shift > 255) ? 255 : (v + shift <= 0) ? 0 : (v + shift);
        v = (v * factor > 255) ? 255 : (v * factor <= 0) ? 0 : (v * factor);
        v = (pow(v, gamma) > 255) ? 255 : (pow(v, gamma) <= 0) ? 0 : (pow(v,gamma));
        Correction::LUT[x] = v;
    }

//    qDebug() << Q_FUNC_INFO << "Not implemented yet!";


    if (image->format() == QImage::Format_Indexed8)
    {
        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
                {
                    QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                    int v = qGray(pixel);
                    v = Correction::LUT[v];
                    newImage->setPixel(x,y, v);
                }
    }
    else //if (image->format() == QImage::Format_RGB32)
    {
        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

                int r = qRed(pixel);    // Get the 0-255 value of the R channel
                int g = qGreen(pixel);  // Get the 0-255 value of the G channel
                int b = qBlue(pixel);   // Get the 0-255 value of the B channel

                r = Correction::LUT[r];
                g = Correction::LUT[g];
                b = Correction::LUT[b];

                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y, newPixel.rgb());

            }
    }

    return newImage;
}
