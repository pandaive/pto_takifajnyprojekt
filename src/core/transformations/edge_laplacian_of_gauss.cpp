#include "edge_laplacian_of_gauss.h"
#include <algorithm>
#include "blur_gaussian.h"

EdgeLaplaceOfGauss::EdgeLaplaceOfGauss(PNM* img) :
    Convolution(img)
{
}

EdgeLaplaceOfGauss::EdgeLaplaceOfGauss(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

math::matrix<float> EdgeLaplaceOfGauss::getMask(int, Mode)
{
    size = getParameter("size").toInt();
    double sigma = getParameter("sigma").toDouble();

    math::matrix<float> mask(size, size);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            mask(i,j) = getLoG(i-size/2, j-size/2, sigma);
        }

    return mask;
}

float EdgeLaplaceOfGauss::getLoG(int x, int y, float s)
{
    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    return (((pow(x,2) + pow(y,2) - 2))/(pow(s,2))) * BlurGaussian::getGauss(x,y,s);
}

int EdgeLaplaceOfGauss::getSize()
{
    return size;
}
