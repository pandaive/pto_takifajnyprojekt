#include "morphological_operator.h"

MorphologicalOperator::MorphologicalOperator(PNM* img) :
    Transformation(img)
{
}

MorphologicalOperator::MorphologicalOperator(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

// abstract
const int MorphologicalOperator::morph(math::matrix<float>, math::matrix<bool>)
{
    return 0;
}

math::matrix<bool> MorphologicalOperator::getSE(int size, SE shape)
{
    switch (shape)
    {
    case Square:    return seSquare(size);
    case Cross:     return seCross(size);
    case XCross:    return seXCross(size);
    case VLine:     return seVLine(size);
    case HLine:     return seHLine(size);
    default:        return seSquare(size);
    }
}


math::matrix<bool> MorphologicalOperator::seSquare(int size)
{
    math::matrix<bool> ret(size, size);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            ret(i,j) = true;
        }

    return ret;
}

math::matrix<bool> MorphologicalOperator::seCross(int size)
{
    math::matrix<bool> ret(size, size);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            if (i == size/2 || j == size/2)
                ret(i,j) = true;
            else
                ret(i,j) = false;
        }

    return ret;
}

math::matrix<bool> MorphologicalOperator::seXCross(int size)
{
    math::matrix<bool> ret(size, size);

   //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            if (i == j || j == size-i)
                ret(i,j) = true;
            else
                ret(i,j) = false;
        }

    return ret;
}

math::matrix<bool> MorphologicalOperator::seVLine(int size)
{
    math::matrix<bool> ret(size, size);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            if (i == size/2)
                ret(i,j) = true;
            else
                ret(i,j) = false;
        }

    return ret;
}

math::matrix<bool> MorphologicalOperator::seHLine(int size)
{
    math::matrix<bool> ret(size, size);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            if (j == size/2)
                ret(i,j) = true;
            else
                ret(i,j) = false;
        }

    return ret;
}

PNM* MorphologicalOperator::transform()
{  
    int size  = getParameter("size").toInt();
    SE  shape = (MorphologicalOperator::SE) getParameter("shape").toInt();

    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(image->width(), image->height(), QImage::Format_RGB32);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            math::matrix<bool> se = getSE(size, shape);
            math::matrix<float> windowR, windowG, windowB;

            windowR = Transformation::getWindow(x, y, size, RChannel, RepeatEdge);
            windowG = Transformation::getWindow(x, y, size, GChannel, RepeatEdge);
            windowB = Transformation::getWindow(x, y, size, BChannel, RepeatEdge);


            int r = morph(windowR, se);
            int g = morph(windowG, se);
            int b = morph(windowB, se);

            QColor newPixel = QColor(r,g,b);
            newImage->setPixel(x,y, newPixel.rgb());
        }

    return newImage;
}
