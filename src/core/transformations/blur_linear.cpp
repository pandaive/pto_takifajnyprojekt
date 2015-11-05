#include "blur_linear.h"

BlurLinear::BlurLinear(PNM* img) :
    Convolution(img)
{
}

BlurLinear::BlurLinear(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* BlurLinear::transform()
{
    int maskSize = getParameter("size").toInt();
    QList<QVariant> tmpMask = getParameter("mask").toList();
    bool normalize = getParameter("normalize").toBool();

    math::matrix<float> mask(maskSize, maskSize);



    for (int x = 0; x < maskSize; x++)
        for (int y = 0; y < maskSize; y++) {
            mask(x,y) = tmpMask.at(3*x+y).toDouble();
        }
    mask = Convolution::reflection(mask);

    for (int j = 0; j < 3; j++)
     for (int i = 0; i < 3; i++) {
         qDebug() << Q_FUNC_INFO << mask(j,i);
     }



    if (normalize){
        float sum = Convolution::sum(mask);
        for (int x = 0; x < maskSize; x++)
            for (int y = 0; y < maskSize; y++) {
                mask(x,y) = mask(x,y)/sum;
            }
    }

    return convolute(mask, RepeatEdge);
}
