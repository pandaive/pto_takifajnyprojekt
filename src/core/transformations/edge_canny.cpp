#include "edge_canny.h"

#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"

math::matrix<float> magnitude;
math::matrix<float> direction;

EdgeCanny::EdgeCanny(PNM* img) :
    Convolution(img)
{
}

EdgeCanny::EdgeCanny(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

int getDirection(float d){
    float degrees = d*100;
    if ((degrees > 0 && degrees <= 22.5) || (degrees > 157.5 && degrees <= 180))
        return 0;
    if (degrees > 22.5 && degrees <= 67.5)
        return 1;
    if (degrees > 67.5 && degrees <= 112.5)
        return 2;
    if (degrees > 112.5 && degrees <= 157.5)
        return 3;

    return 0;
}

bool hysteresis(int x, int y,
                int x_neighbour, int y_neighbour, int direction_neighbour,
                int lower_thresh) {

    if (magnitude(x,y) < lower_thresh)
        return false;

    if (getDirection(direction(x,y)) != direction_neighbour)
        return false;

    for (int i = -1; i < 2; i++) {
        if (x+i < 0 || x+i < 255)
            break;
        for (int j = -1; j < 2; j++) {
            if (y+j < 0 || y+j > 255)
                break;
            if (x+i == x_neighbour && y+j == y_neighbour)
                break;

            if (magnitude(x+i, y+j) > magnitude(x,y) && getDirection(direction(x,y)) == getDirection(direction(x+i, y+j)))
                return false;
        }
    }

    return true;
}

PNM* EdgeCanny::transform()
{
    int width  = image->width(),
        height = image->height();

    int upper_thresh = getParameter("upper_threshold").toInt(),
        lower_thresh = getParameter("lower_threshold").toInt();

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);

    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    magnitude = math::matrix<float>(width, height);
    direction = math::matrix<float>(width,height);
    std::list<QPoint> initialEdges;

    ConversionGrayscale* conversionGrayscale = new ConversionGrayscale(image);
    PNM* grayImage = conversionGrayscale->transform();


    BlurGaussian* blurGaussian = new BlurGaussian(grayImage);
    blurGaussian->setParameter("size", 3);
    blurGaussian->setParameter("sigma", 1.6);
    PNM* blurred = blurGaussian->transform();

    EdgeSobel* edgeSobel = new EdgeSobel(blurred);
    math::matrix<float>* Gx = edgeSobel->rawHorizontalDetection();
    math::matrix<float>* Gy = edgeSobel->rawVerticalDetection();

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            newImage->setPixel(i, j, PIXEL_VAL_MIN);
            magnitude(i,j) = sqrt(pow((*Gx)(i,j),2) + pow((*Gy)(i,j),2));
            direction(i,j) = atan(abs((*Gy)(i,j))/abs((*Gx)(i,j)));
        }

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            int d = getDirection(direction(i,j));

            int diff_s1[2], diff_s2[2]; //i,j dla sasiad1, sasiad2 (s1_i, s1_j, s2_i, s2_j - roznica od wyjsciowych i,j
            switch(d) {
            case 0:
                diff_s1[0] = -1; diff_s1[1] = 0;
                diff_s2[0] = 1; diff_s2[1] = 0;
            case 1:
                diff_s1[0] = 1; diff_s1[1] = 1;
                diff_s2[0] = -1; diff_s2[1] = 1;
            case 2:
                diff_s1[0] = 0; diff_s1[1] = 1;
                diff_s2[0] = 0; diff_s2[1] = -1;
            case 3:
                diff_s1[0] = -1; diff_s1[1] = -1;
                diff_s2[0] = 1; diff_s2[1] = 1;
            }

            if ((i+diff_s1[0] >= 0 && i+diff_s1[0] < width && j+diff_s1[1] >= 0 && j+diff_s1[1] < width
                 && i+diff_s2[0] >=0 && i+diff_s2[0] < width && j+diff_s2[1] >= 0 && j+diff_s2[1] < width
                 && magnitude(i,j) > magnitude(i+diff_s1[0], j+diff_s1[1]))
                 && (magnitude(i,j) > magnitude(i+diff_s2[0], j+diff_s2[1]))
                 && magnitude(i,j) > upper_thresh) {

                newImage->setPixel(i, j, PIXEL_VAL_MAX);
                initialEdges.push_back(QPoint(i,j));
            }

           while(!initialEdges.empty()) {
               QPoint currentEdge = initialEdges.back();
               initialEdges.pop_back();
               if (hysteresis(currentEdge.x(), currentEdge.y(), i, j, d, lower_thresh))
                   newImage->setPixel(currentEdge.x(), currentEdge.y(), PIXEL_VAL_MAX);

           }
        }

    return newImage;
}
