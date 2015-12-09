#include "map_normal.h"

#include "edge_sobel.h"
#include "map_height.h"

MapNormal::MapNormal(PNM* img) :
    Convolution(img)
{
}

MapNormal::MapNormal(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* MapNormal::transform()
{
    int width  = image->width(),
        height = image->height();

    double strength = getParameter("strength").toDouble();

    PNM* newImage = new PNM(width, height, image->format());

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    MapHeight* mh = new MapHeight(image);
    PNM* imgHeight = mh->transform();

    EdgeSobel* es = new EdgeSobel(image);
    math::matrix<float>* Gx = es->rawHorizontalDetection();
    math::matrix<float>* Gy = es->rawVerticalDetection();

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {

            float dx = (*Gx)(i,j)/255;
            float dy = (*Gy)(i,j)/255;
            float dz = 1/strength;

            double dw = sqrt(dx*dx + dy*dy + dz*dz);
            dx = dx / dw;
            dy = dy / dw;
            dz = dz / dw;

            dx = (dx + 1.0)*(255/strength);
            dy = (dy + 1.0)*(255/strength);
            dz = (dz + 1.0)*(255/strength);

            QColor newPixel = QColor(dx,dy,dz);
            newImage->setPixel(i,j, newPixel.rgb());

        }

    return newImage;
}
