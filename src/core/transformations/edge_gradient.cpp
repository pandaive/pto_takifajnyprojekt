#include "edge_gradient.h"
#include <algorithm>

EdgeGradient::EdgeGradient(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

EdgeGradient::EdgeGradient(PNM* img) :
    Convolution(img)
{
}

PNM* EdgeGradient::verticalDetection()
{
    return convolute(g_y, RepeatEdge);
}

PNM* EdgeGradient::horizontalDetection()
{
    return convolute(g_x, RepeatEdge);
}

PNM* EdgeGradient::transform()
{
    PNM* newImage = new PNM(image->width(), image->height(), image->format());

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    int width = image->width();
    int height = image->height();

    PNM* imageX = horizontalDetection();
    PNM* imageY = verticalDetection();

    if (image->format() == QImage::Format_Indexed8) {
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {
                int l = sqrt(pow(qGray(imageX->pixel(x,y)), 2) + pow(qGray(imageY->pixel(x,y)), 2));
                newImage->setPixel(x,y,l);

            }
    }
    else { //if RGB

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {
                int r = sqrt(pow(qRed(imageX->pixel(x,y)), 2) + pow(qRed(imageY->pixel(x,y)), 2));
                int g = sqrt(pow(qGreen(imageX->pixel(x,y)), 2) + pow(qGreen(imageY->pixel(x,y)), 2));
                int b = sqrt(pow(qBlue(imageX->pixel(x,y)), 2) + pow(qBlue(imageY->pixel(x,y)), 2));

                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y, newPixel.rgb());
            }
    }

    return newImage;
}

