#include "blur_gaussian.h"
#include <math.h>

BlurGaussian::BlurGaussian(PNM* img) :
    Convolution(img)
{
}

BlurGaussian::BlurGaussian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* BlurGaussian::transform()
{
    emit message("Blurring...");

    int size = getParameter("size").toInt();
    radius = (size/2)+1;
    sigma = getParameter("sigma").toDouble();

    return convolute(getMask(size, Normalize), RepeatEdge);
}

math::matrix<float> BlurGaussian::getMask(int size, Mode)
{
    math::matrix<float> mask(size, size);

    int size2 = floor(size/2);

    for (int xM = -size2; xM <= size2; xM++)
        for (int yM = -size2; yM <= size2; yM++) {
            mask(xM+size2, yM+2+size2) = getGauss(xM, yM, sigma);
        }

    return mask;
}

float BlurGaussian::getGauss(int x, int y, float sigma)
{    
    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    float gauss = (1/(M_PI*sigma*sigma))*exp(-((x*x+y*y)/2*sigma*sigma));
    return gauss;
}

