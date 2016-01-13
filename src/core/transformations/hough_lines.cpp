#include "hough_lines.h"

#include "bin_gradient.h"
#include "edge_laplacian.h"
#include "hough.h"

#include <QPainter>

HoughLines::HoughLines(PNM* img) :
    Transformation(img)
{
}

HoughLines::HoughLines(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HoughLines::transform()
{
    // Cut of value from the image;
    int  threshold      = getParameter("threshold").toInt();
    bool drawWholeLines = getParameter("draw_whole_lines").toBool();

    PNM* newImage = new PNM(image->copy());
    int width = image->width();
    int height = image->height();

//    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    EdgeLaplacian* edgeLaplacian = new EdgeLaplacian(image);
    edgeLaplacian->setParameter("size", 3);
    PNM* laplacianImage = edgeLaplacian->transform();

    BinarizationGradient* binarizationGradient = new BinarizationGradient(laplacianImage);
    PNM* binaryImage = binarizationGradient->transform();

    Hough* hough = new Hough(binaryImage);
    hough->setParameter("theta_density", 3);
    hough->setParameter("skip_edge_detection", true);
    PNM* houghImage = hough->transform();

    int theta = houghImage->width();
    int p = houghImage->height();

    QPainter* qPainter = new QPainter(newImage);
    qPainter->setPen(Qt::red);


    for (int x = 0; x < theta; x++)
        for (int y = 0; y < p; y++) {
            QRgb pixel = houghImage->pixel(x,y);
            int v = qGray(pixel);
            if (v > threshold) {
                int r = y - p/2;
                float rTheta = (x/3.0)*M_PI / 180.0;
                float sinTheta = sin(rTheta);
                float cosTheta = cos(rTheta);

                int x1 = 0;
                int y1 = r/sinTheta;
                int x2 = width - 1;
                int y2 = (r - (width-1) * cosTheta)/sinTheta;
                qPainter->drawLine(x1, y1, x2, y2);
            }
        }

    if (!drawWholeLines) {
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {
                if (qGray(binaryImage->pixel(x,y)) == 0)
                    newImage->setPixel(x,y,image->pixel(x,y));
            }
    }

    return newImage;
}
