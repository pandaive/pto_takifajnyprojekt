#include "corner_harris.h"

#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"

CornerHarris::CornerHarris(PNM* img) :
    Convolution(img)
{
}

CornerHarris::CornerHarris(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* CornerHarris::transform()
{
    int    threshold    = getParameter("threshold").toInt();
    double sigma        = getParameter("sigma").toDouble(),
           sigma_weight = getParameter("sigma_weight").toDouble(),
           k_param      = getParameter("k").toDouble();

    int width  = image->width(),
        height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    math::matrix<float> Ixx(width, height);
    math::matrix<float> Iyy(width, height);
    math::matrix<float> Ixy(width, height);
    math::matrix<float> cornerCandidates(width, height);
    math::matrix<float> cornerNonmaxSuppress(width, height);

    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            Ixx(x,y) = Iyy(x,y) = Ixy(x,y) = cornerCandidates(x,y) = cornerNonmaxSuppress(x,y) = 0;
        }

    ConversionGrayscale* conversionGrayscale = new ConversionGrayscale(image);
    PNM* grayscaleImage = conversionGrayscale->transform();

    BlurGaussian* blurGaussian = new BlurGaussian(grayscaleImage);
    blurGaussian->setParameter("size", 3);
    blurGaussian->setParameter("sigma", 1.6);
    PNM* blurredImage = blurGaussian->transform();

    EdgeSobel* edgeSobel = new EdgeSobel(blurredImage);
    math::matrix<float>* Gx = edgeSobel->rawHorizontalDetection();
    math::matrix<float>* Gy = edgeSobel->rawVerticalDetection();

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            Ixx(i,j) = (*Gx)(i,j) * (*Gx)(i,j);
            Iyy(i,j) = (*Gy)(i,j) * (*Gy)(i,j);
            Ixy(i,j) = (*Gx)(i,j) * (*Gy)(i,j);
        }

    for (int i = 1; i < width - 1; i++)
        for (int j = 1; j < height - 1; j++) {

            float Sxx = 0, Syy = 0, Sxy = 0;

            for (int k = -1; k < 2; k++)
                for (int l = -1; l < 2; l++) {
                    float gauss = BlurGaussian::getGauss(k,l,sigma);
                    Sxx = Sxx + Ixx(i+k, j+l)*gauss;
                    Syy = Syy + Iyy(i+k, j+l)*gauss;
                    Sxy = Sxy + Ixy(i+k, j+l)*gauss;
                }

            Sxx = Sxx/sigma_weight;
            Syy = Syy/sigma_weight;
            Sxy = Sxy/sigma_weight;

            math::matrix<float> H(2,2);
            H(0,0) = Sxx;
            H(0,1) = H(1,0) = Sxy;
            H(1,1) = Syy;

            float detH = H(0,0)*H(1,1) - H(0,1)*H(1,0);
            float trH = H(0,0) + H(1,1);
            float r = detH - k_param * pow(trH, 2);

            if (r > threshold)
               cornerCandidates(i,j) = r;
        }

    bool search = true;

    while (search) {
        search = false;
        for (int i = 1; i < width - 1; i++)
            for (int j = 1; j < height - 1; j++) {

                bool max = true;

                for (int k = -1; k < 2; k++)
                    for (int l = -1; l < 2; l++) {
                        if ((k!=0 || l!=0) && cornerCandidates(i+k, j+l) >= cornerCandidates(i,j))
                            max = false;
                    }

                if (max)
                    cornerNonmaxSuppress(i,j) = cornerCandidates(i,j);
                else {
                    if (cornerCandidates(i,j) > 0)
                        search = true;
                    cornerNonmaxSuppress(i,j) = 0;
                }
            }
        cornerCandidates = cornerNonmaxSuppress;
    } //end while(search)

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            if (cornerCandidates(i,j) == 0)
                newImage->setPixel(i,j,Qt::color0);
            else
                newImage->setPixel(i,j,Qt::color1);
        }

    return newImage;
}
