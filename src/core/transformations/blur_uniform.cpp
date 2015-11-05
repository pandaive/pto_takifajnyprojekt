#include "blur_uniform.h"

BlurUniform::BlurUniform(PNM* img) :
    Convolution(img)
{
}

BlurUniform::BlurUniform(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

math::matrix<float> BlurUniform::getMask(int size, Mode)
{
    math::matrix<float> mask(size, size);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    for (int x = 0; x < size; x++)
        for (int y = 0; y < size; y++)
            mask(x,y) = 1;

    return mask;
}
