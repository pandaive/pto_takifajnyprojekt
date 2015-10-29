#include "histogram_stretching.h"

#include "../histogram.h"

HistogramStretching::HistogramStretching(PNM* img) :
    Transformation(img)
{
}

HistogramStretching::HistogramStretching(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HistogramStretching::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

//    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    if (image->format() == QImage::Format_Indexed8) {
        QHash<int, int>* L = (image->getHistogram()->get(Histogram::Channel::LChannel));

        int min;
        //get left
        for (int x = 0; x < PIXEL_VAL_MAX + 1; x++)
            if (L->value(x) > 0) {
                min = x;
                break;
            }
        int max;
        //get right
        for (int x = PIXEL_VAL_MAX; x > PIXEL_VAL_MIN-1; x--)
            if (L->value(x) > 0) {
                max = x;
                break;
            }

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {
                QRgb pixel = image->pixel(x,y);
                int l = qGray(pixel);
                float v = float(255)/(max-min) * (l - min);
                newImage->setPixel(x, y, v);
            }
    }
    else { //RGB

        QHash<int, int>* R = (image->getHistogram()->get(Histogram::Channel::RChannel));
        QHash<int, int>* G = (image->getHistogram()->get(Histogram::Channel::GChannel));
        QHash<int, int>* B = (image->getHistogram()->get(Histogram::Channel::BChannel));

        int minR;
        for (int x = 0; x < PIXEL_VAL_MAX + 1; x++)
            if (R->value(x) > 0) {
                minR = x;
                break;
            }
        int maxR;
        for (int x = PIXEL_VAL_MAX; x > PIXEL_VAL_MIN-1; x--)
            if (R->value(x) > 0) {
                maxR = x;
                break;
            }

        int minG;
        for (int x = 0; x < PIXEL_VAL_MAX + 1; x++)
            if (G->value(x) > 0) {
                minG = x;
                break;
            }
        int maxG;
        for (int x = PIXEL_VAL_MAX; x > PIXEL_VAL_MIN-1; x--)
            if (G->value(x) > 0) {
                maxG = x;
                break;
            }
        int minB;
        for (int x = 0; x < PIXEL_VAL_MAX + 1; x++)
            if (B->value(x) > 0) {
                minB = x;
                break;
            }
        int maxB;
        for (int x = PIXEL_VAL_MAX; x > PIXEL_VAL_MIN-1; x--)
            if (B->value(x) > 0) {
                maxB = x;
                break;
            }

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {
                QRgb pixel = image->pixel(x,y);

                int r = qRed(pixel);
                int g = qGreen(pixel);
                int b = qBlue(pixel);

                float v_r = (float(255)/(maxR-minR)) * (r - minR);
                float v_g = (float(255)/(maxG-minG)) * (g - minG);
                float v_b = (float(255)/(maxB-minB)) * (b - minB);

                QColor newPixel = QColor(v_r,v_g,v_b);
                newImage->setPixel(x, y, newPixel.rgb());

            }
        }

    return newImage;
}
