#include "hough.h"

#include "conversion_grayscale.h"
#include "edge_laplacian.h"

Hough::Hough(PNM* img) :
    Transformation(img)
{
}

Hough::Hough(PNM* img, ImageViewer* super) :
    Transformation(img, super)
{
}

PNM* Hough::transform()
{
    int thetaDensity = getParameter("theta_density").toInt();
    int thetaSize = 180 * thetaDensity;
    int width = image->width();
    int height = image->height();

    int pMax = sqrt(pow(width, 2) + pow(height, 2));

    int newWidth = thetaSize;
    int newHeight = pMax*2+1;

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    PNM* newImage = new PNM(newWidth, newHeight, QImage::Format_Indexed8);

    math::matrix<float> hough(newWidth, newHeight);
    for (int x = 0; x < newWidth; x++)
        for (int y = 0; y < newHeight; y++) {
            hough(x,y) = 0;
        }

    ConversionGrayscale* conversionGrayscale = new ConversionGrayscale(image);
    image = conversionGrayscale->transform();

    if (getParameter("skip_edge_detection").toBool() == false) {
        EdgeLaplacian* edgeLaplacian = new EdgeLaplacian(image);
        edgeLaplacian->setParameter("size", 3);
        image = edgeLaplacian->transform();
    }

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            if (qGray(image->pixel(i,j)) > 0) {
                for (int k = 0; k < thetaSize; k++) {
                    float theta = (k * M_PI)/(thetaDensity * 180.0);
                    float p = i * cos(theta) + j * sin(theta);
                    hough(k, p + pMax)++;
                }
            }
        }

    float max;
    for (int x = 0; x < newWidth; x++)
        for (int y = 0; y < newHeight; y++) {
            if (hough(x,y) > max)
                max = hough(x,y);
        }

    for (int x = 0; x < newWidth; x++)
        for (int y = 0; y < newHeight; y++) {
            float v = (hough(x,y)*255)/max;
            newImage->setPixel(x,y,v);
        }

    return newImage;
}
