#include "edge_prewitt.h"

EdgePrewitt::EdgePrewitt(PNM*img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

EdgePrewitt::EdgePrewitt(PNM*img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

void EdgePrewitt::prepareMatrices()
{
    math::matrix<float> g_xTemp (3,3);
    math::matrix<float> g_yTemp (3,3);

    g_xTemp(0,0) = g_xTemp(1,0) = g_xTemp(2,0) = -1;
    g_xTemp(0,1) = g_xTemp(1,1) = g_xTemp(2,1) = 0;
    g_xTemp(0,2) = g_xTemp(1,2) = g_xTemp(2,2) = 1;

    g_yTemp(0,0) = g_yTemp(0,1) = g_yTemp(0,2) = -1;
    g_yTemp(1,0) = g_yTemp(1,1) = g_yTemp(1,2) = 0;
    g_yTemp(2,0) = g_yTemp(2,1) = g_yTemp(2,2) = 1;

    g_x = g_xTemp;
    g_y = g_yTemp;
}

