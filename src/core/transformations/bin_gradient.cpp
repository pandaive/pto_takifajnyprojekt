#include "bin_gradient.h"
#include <algorithm>

BinarizationGradient::BinarizationGradient(PNM* img) :
    Transformation(img)
{
}

BinarizationGradient::BinarizationGradient(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationGradient::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    int sumGandI = 0;
    int sumG = 0;
    int T = 0;

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            int I = qGray(image->pixel(i,j));
            int Gx = qGray(image->pixel(std::min(i+1, width-1),j)) - qGray(image->pixel(std::max(i-1, 0),j));
            int Gy = qGray(image->pixel(i,std::min  (j+1, height-1))) - qGray(image->pixel(i,std::max(j-1, 0)));

            sumGandI = sumGandI + std::max(Gx, Gy) * I;
            sumG = sumG + std::max(Gx, Gy);
        }

    T = sumGandI/sumG;

    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            if (qGray(image->pixel(x,y)) < T)
                newImage->setPixel(x,y,0);
            else
                newImage->setPixel(x,y,1);
        }

    return newImage;
}
