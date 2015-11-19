#include "noise_bilateral.h"

NoiseBilateral::NoiseBilateral(PNM* img) :
    Convolution(img)
{
}

NoiseBilateral::NoiseBilateral(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* NoiseBilateral::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    sigma_d = getParameter("sigma_d").toInt();
    sigma_r = getParameter("sigma_r").toInt();
    radius = sigma_d;

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    if (image->format() == QImage::Format_Indexed8) {
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {
                newImage->setPixel(x,y,calcVal(x,y,LChannel));
            }
    }
    else { //if RGB
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {
                int r = calcVal(x,y,RChannel);
                int g = calcVal(x,y,GChannel);
                int b = calcVal(x,y,BChannel);

                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y, newPixel.rgb());

            }
    }

    return newImage;
}

int NoiseBilateral::calcVal(int x, int y, Channel channel)
{
    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    math::matrix<float> window;
    int windowSize = radius;
    window = Transformation::getWindow(x, y, windowSize, channel, RepeatEdge);

    float sumUp = 0, sumDown = 0;

    for (int i = 0; i < windowSize; i++)
        for (int j = 0; j < windowSize; j++) {
            float color = colorCloseness(window(i,j), window(windowSize/2,windowSize/2));
            float spatial = spatialCloseness(QPoint(i,j), QPoint(windowSize/2,windowSize/2));

            sumUp = sumUp + (window(i,j) * color * spatial);
            sumDown = sumDown + (color * spatial);
        }

    return (int)sumUp/sumDown;
}

float NoiseBilateral::colorCloseness(int val1, int val2)
{
    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    float valDiff = val1 - val2;
    return std::exp((-1)*((valDiff * valDiff)/(2*sigma_r * sigma_r)));

}

float NoiseBilateral::spatialCloseness(QPoint point1, QPoint point2)
{
    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    float xdiff = point1.x()-point2.x();
    float ydiff = point1.y()-point2.y();

    return std::exp((-1)*(((xdiff * xdiff) + (ydiff * ydiff))/(2*sigma_d*sigma_d)));

}
