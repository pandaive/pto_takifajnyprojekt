#include "edge_zero.h"

#include "edge_laplacian_of_gauss.h"

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img) :
    Convolution(img)
{
}

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* EdgeZeroCrossing::transform()
{
    int width = image->width(),
        height = image->height();

    int    size  = getParameter("size").toInt();
    double sigma = getParameter("sigma").toDouble();
    int    t     = getParameter("threshold").toInt();

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    int v0 = 128;

    EdgeLaplaceOfGauss elog(image);

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            math::matrix<float> window = elog.getWindow(i,j,size, LChannel, RepeatEdge);

            int min = PIXEL_VAL_MAX;
            int max = PIXEL_VAL_MIN;

            for (int x = 0; x < window.rowno(); x++)
                for (int y = 0; y < window.rowno(); y++) {
                    if (window(x,y) < min)
                        min = window(x,y);
                    if (window(x,y) > max)
                        max = window(x,y);
                }

            if (min < (v0 - t) && max > (v0 + t)) {
                int l = window(window.rowno()/2, window.rowno()/2);
                newImage->setPixel(i,j,l);
            }
            else
                newImage->setPixel(i,j,0);

        }

    return newImage;
}

