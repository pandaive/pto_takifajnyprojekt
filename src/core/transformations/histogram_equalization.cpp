#include "histogram_equalization.h"

#include "../histogram.h"

HistogramEqualization::HistogramEqualization(PNM* img) :
    Transformation(img)
{
}

HistogramEqualization::HistogramEqualization(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HistogramEqualization::transform()
{
    int width = image->width();
    int height = image->height();
    int nbPixels = width * height;
    float p = (float)255/nbPixels;

    PNM* newImage = new PNM(width, height, image->format());

    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    if (image->format() == QImage::Format_Indexed8) {
        QHash<int, int>* L = (image->getHistogram()->get(Histogram::Channel::LChannel));

        float D [256];
        D[0] = p * L->value(0);

        for (int i = 1; i < PIXEL_VAL_MAX+1; i++)
            D[i] = D[i-1] + p * L->value(i);

        qDebug() << Q_FUNC_INFO << D[255];

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {
                QRgb pixel = image->pixel(x,y);
                int l = qGray(pixel);
                newImage->setPixel(x,y,D[l]);
            }

    }
    else { //RGB

        QHash<int, int>* R = (image->getHistogram()->get(Histogram::Channel::RChannel));
        QHash<int, int>* G = (image->getHistogram()->get(Histogram::Channel::GChannel));
        QHash<int, int>* B = (image->getHistogram()->get(Histogram::Channel::BChannel));

        float D_r [256];
        float D_g [256];
        float D_b [256];

        D_r[0] = p * R->value(0);
        D_g[0] = p * G->value(0);
        D_b[0] = p * B->value(0);

        for (int i = 1; i < PIXEL_VAL_MAX+1; i++)
            D_r[i] = D_r[i-1] + p * R->value(i);
        for (int i = 1; i < PIXEL_VAL_MAX+1; i++)
            D_g[i] = D_g[i-1] + p * G->value(i);
        for (int i = 1; i < PIXEL_VAL_MAX+1; i++)
            D_b[i] = D_b[i-1] + p * B->value(i);

        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {

                QRgb pixel = image->pixel(x,y);

                int r = qRed(pixel);
                int g = qGreen(pixel);
                int b = qBlue(pixel);

                r = D_r[r];
                g = D_g[g];
                b = D_b[b];

                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y, newPixel.rgb());
            }
    }

    return newImage;
}

