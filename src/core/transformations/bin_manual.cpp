#include "bin_manual.h"

BinarizationManual::BinarizationManual(PNM* img) :
    Transformation(img)
{
}

BinarizationManual::BinarizationManual(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationManual::transform()
{
    int threshold = getParameter("threshold").toInt();

    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            if (qGray(image->pixel(x,y)) < threshold)
                newImage->setPixel(x,y,0);
            else
                newImage->setPixel(x,y,1);

        }

    return newImage;
}




