#include "noise_median.h"

NoiseMedian::NoiseMedian(PNM* img) :
    Convolution(img)
{
}

NoiseMedian::NoiseMedian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* NoiseMedian::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    if (image->format() == QImage::Format_Indexed8) {

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {
                newImage->setPixel(x,y,getMedian(x,y,LChannel));
            }
    }
    else {  //if RGB
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {
                int r = getMedian(x,y,RChannel);
                int g = getMedian(x,y,GChannel);
                int b = getMedian(x,y,BChannel);

                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y, newPixel.rgb());
            }
    }
    return newImage;
}

int NoiseMedian::getMedian(int x, int y, Channel channel)
{
    int radius = getParameter("radius").toInt();

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    math::matrix<float> window;
    int windowSize = radius * 2 + 1;
    window = Transformation::getWindow(x, y, windowSize, channel, RepeatEdge);

    int valuesSize = windowSize * windowSize;
    float* values = new float[valuesSize];

    for (int i = 0; i < windowSize; i++)
        for (int j = 0; j < windowSize; j++) {
            values[i*windowSize+j] = window[i][j];
        }

    std::sort(values, values+valuesSize);

    return (int) ((valuesSize % 2) == 0 ?
                      (values[valuesSize/2] + values[valuesSize/2-1])/2 : floor(values[valuesSize/2]));

}
