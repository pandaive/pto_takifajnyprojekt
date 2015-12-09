#include "edge_sobel.h"

EdgeSobel::EdgeSobel(PNM* img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

EdgeSobel::EdgeSobel(PNM* img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

void EdgeSobel::prepareMatrices()
{
    math::matrix<float> g_xTemp (3,3);
    math::matrix<float> g_yTemp (3,3);

    g_xTemp(0,0) = -1;
    g_xTemp(1,0) = -2;
    g_xTemp(2,0) = -1;
    g_xTemp(0,1) = g_xTemp(1,1) = g_xTemp(2,1) = 0;
    g_xTemp(0,2) = 1;
    g_xTemp(1,2) = 2;
    g_xTemp(2,2) = 1;

    g_yTemp(0,0) = -1;
    g_yTemp(0,1) = -2;
    g_yTemp(0,2) = -1;
    g_yTemp(1,0) = g_yTemp(1,1) = g_yTemp(1,2) = 0;
    g_yTemp(2,0) = 1;
    g_yTemp(2,1) = 2;
    g_yTemp(2,2) = 1;

    g_x = g_xTemp;
    g_y = g_yTemp;
}

math::matrix<float>* EdgeSobel::rawHorizontalDetection()
{
    math::matrix<float>* x_gradient = new math::matrix<float>(this->image->width(), this->image->height());

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    int width = image->width();
    int height = image->height();

    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            (*x_gradient)(x,y) = sum(join(g_x, getWindow(x, y, 3, LChannel, RepeatEdge)));
        }

    return x_gradient;
}

math::matrix<float>* EdgeSobel::rawVerticalDetection()
{
    math::matrix<float>* y_gradient = new  math::matrix<float>(this->image->width(), this->image->height());

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    int width = image->width();
    int height = image->height();

    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            (*y_gradient)(x,y) = sum(join(g_y, getWindow(x, y, 3, LChannel, RepeatEdge)));
        }

    return y_gradient;
}
