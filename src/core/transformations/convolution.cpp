#include "convolution.h"

/** Overloaded constructor */
Convolution::Convolution(PNM* img) :
    Transformation(img)
{
}

Convolution::Convolution(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

/** Returns a convoluted form of the image */
PNM* Convolution::transform()
{
    return convolute(getMask(3, Normalize), RepeatEdge);
}

/** Returns a sizeXsize matrix with the center point equal 1.0 */
math::matrix<float> Convolution::getMask(int size, Mode mode = Normalize)
{
    math::matrix<float> mask(size, size);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    for (int x = 0; x < size; x++)
        for (int y = 0; y < size; y++) {
            mask(x,y) = 0.0;
        }
    mask(floor(size/2), floor(size/2)) = 1.0;

    return mask;
}

/** Does the convolution process for all pixels using the given mask. */
PNM* Convolution::convolute(math::matrix<float> mask, Mode mode = RepeatEdge)
{
    int width  = image->width(),
        height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    float weight = sum(mask);
    math::matrix<float> window;
    float akumulator;

    if (image->format() == QImage::Format_Indexed8) {
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {

                window = Transformation::getWindow(x, y, mask.rowno(), LChannel, mode);
                akumulator = sum(join(window, reflection(mask)));
                //qDebug() << Q_FUNC_INFO << akumulator;

                if (weight != 0)
                    akumulator = akumulator/weight;
                if (akumulator >= 0 && akumulator < PIXEL_VAL_MAX)
                    newImage->setPixel(x,y,(int)akumulator);
            }
    }
    else //if rgb
    {
        int r,g,b;
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {

                window = Transformation::getWindow(x, y, mask.rowno(), RChannel, mode);
                akumulator = sum(join(window, reflection(mask)));
                //qDebug() << Q_FUNC_INFO << akumulator;

                if (weight != 0)
                    akumulator = akumulator/weight;
                if (akumulator >= 0 && akumulator < PIXEL_VAL_MAX)
                    r = (int)akumulator;
                else if (akumulator < 0)
                    r = 0;
                else if (akumulator > PIXEL_VAL_MAX)
                    r = 255;


                window = Transformation::getWindow(x, y, mask.rowno(), GChannel, mode);
                akumulator = sum(join(window, reflection(mask)));
                //qDebug() << Q_FUNC_INFO << akumulator;

                if (weight != 0)
                    akumulator = akumulator/weight;
                if (akumulator >= 0 && akumulator < PIXEL_VAL_MAX)
                    g = (int)akumulator;
                else if (akumulator < 0)
                    g = 0;
                else if (akumulator > PIXEL_VAL_MAX)
                    g = 255;

                window = Transformation::getWindow(x, y, mask.rowno(), BChannel, mode);
                akumulator = sum(join(window, reflection(mask)));
                //

                if (weight != 0)
                    akumulator = akumulator/weight;
                if (akumulator >= 0 && akumulator < PIXEL_VAL_MAX)
                    b = (int)akumulator;
                else if (akumulator < 0)
                    b = 0;
                else if (akumulator > PIXEL_VAL_MAX)
                    b = 255;

                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(x,y, newPixel.rgb());
            }
    }

    return newImage;
}

/** Joins to matrices by multiplying the A[i,j] with B[i,j].
  * Warning! Both Matrices must be squares with the same size!
  */
const math::matrix<float> Convolution::join(math::matrix<float> A, math::matrix<float> B)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);


    if (A.rowno()==B.rowno() && A.colno() == B.colno())
        for (int x = 0; x < size; x++)
            for (int y = 0; y < size; y++)
                C(x,y) = A(x,y) * B(x,y);

    return C;
}

/** Sums all of the matrixes elements */
const float Convolution::sum(const math::matrix<float> A)
{
    float sum = 0.0;
    int size = A.rowno();
    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";
    for (int x = 0; x < size; x++)
        for (int y = 0; y < size; y++)
            sum = sum + A(x,y);

    return sum;

}


/** Returns reflected version of a matrix */
const math::matrix<float> Convolution::reflection(const math::matrix<float> A)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";
    for (int x = 0; x < size; x++)
        for (int y = 0; y < size; y++)
            C(x,y) = A(size-1-x, size-1-y);

    return C;
}
