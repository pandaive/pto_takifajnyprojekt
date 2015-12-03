#include "edge_roberts.h"

EdgeRoberts::EdgeRoberts(PNM* img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

EdgeRoberts::EdgeRoberts(PNM* img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

void EdgeRoberts::prepareMatrices()
{
    math::matrix<float> g_xTemp (2,2);
    math::matrix<float> g_yTemp (2,2);
    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";
    g_xTemp(0,0) = 1;
    g_xTemp(1,0) = 0;
    g_xTemp(0,1) = 0;
    g_xTemp(1,1) = -1;

    g_yTemp(0,0) = 0;
    g_yTemp(1,0) = -1;
    g_yTemp(0,1) = 1;
    g_yTemp(1,1) = 0;

    g_x = g_xTemp;
    g_y = g_yTemp;

    qDebug() << Q_FUNC_INFO << g_x(0,0);

}
