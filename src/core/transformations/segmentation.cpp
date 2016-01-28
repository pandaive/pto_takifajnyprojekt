#include "segmentation.h"

#include "conversion_grayscale.h"
#include "blur_gaussian.h"

#include <queue>

Segmentation::Segmentation(PNM* img) :
    Transformation(img)
{
}

Segmentation::Segmentation(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

#define INIT -1
#define MASK -2
#define WSHED 0

QPoint* Segmentation::neighbourhood(QPoint p)
{
    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    return new QPoint();
}

PNM* Segmentation::transform()
{
    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);

    ConversionGrayscale* conversionGrayscale = new ConversionGrayscale(image);
    PNM* grayImage = conversionGrayscale->transform();

    math::matrix<int> lab (width, height);
    math::matrix<int> dist (width, height);

    int curlab = 0;
    std::queue<QPoint> fifo;

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            lab[i][j] = INIT;
            dist[i][j] = 0;
        }

    std::list<int> hSorted;
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            hSorted.push_back(qGray(grayImage->pixel(i,j)));
        }


    hSorted.sort();
    hSorted.unique();

    int curDist = 0;

    for (std::list<int>::iterator h = hSorted.begin(); h != hSorted.end(); h++) {
        for (int p_i = 0; p_i < width; p_i++)
            for (int p_j = 0; p_j < height; p_j++) {
                if (qGray(image->pixel(p_i,p_j)) == *h) {
                    lab[p_i][p_j] = MASK;
                    for (int x = -1; x < 2; x++)
                        for (int y = -1; y < 2; y++) {
                            if (x != 0 || y !=0) {
                                int q_i = p_i+x;
                                int q_j = p_j+y;
                                if (q_i >= 0 && q_j >= 0 && q_i < width && q_j < height)
                                    if (lab[q_i][q_j] > 0 || lab[q_i][q_j] == WSHED) {
                                        //(∗ Initialize queue with neighbours at level h of current basins or watersheds ∗)
                                        dist[p_i][p_j] = 1;
                                        fifo.push(QPoint(p_i,p_j));
                                    } //endif
                            }
                        }
                }
            }//endfor
        curDist = 1;
        fifo.push(QPoint(-1,-1));
        while(true) { //loop (∗ extend basins ∗)
            QPoint p = fifo.front();
            fifo.pop();
            if (p.x() == -1 && p.y()==-1) {
                if (fifo.empty()) { //line 29
                    break;
                }
                else {
                    fifo.push(QPoint(-1, -1));
                    curDist += 1;
                    p = fifo.front();
                    fifo.pop();
                }
            }
            for (int x = -1; x < 2; x++)
                for (int y = -1; y < 2; y++) { //line 36 - for all neighbours - labelling p
                    if (x != 0 || y != 0) {
                        int p_i = p.x();
                        int p_j = p.y();
                        int q_i = p_i + x;
                        int q_j = p_j + y;
                        if (q_i >= 0 && q_j >= 0 && q_i < width && q_j < height) {
                            if ((dist[q_i][q_j] < curDist) && (lab[q_i][q_j] > 0 || lab[q_i][q_j] == WSHED)) {
                                //(∗ q belongs to an existing basin or to watersheds ∗)
                                if (lab[q_i][q_j] > 0) {
                                    if (lab[p_i][p_j] == MASK or lab[p_i][p_j] == WSHED)
                                        lab[p_i][p_j] = lab[q_i][q_j]; //41
                                    else if (lab[p_i][p_j] != lab[q_i][q_j]) {
                                        lab[p_i][p_j] = WSHED; // 43
                                    }
                                }
                                else if (lab[p_i][p_j] == MASK)
                                    lab[p_i][p_j] = WSHED;
                            } //48
                            else if (lab[q_i][q_j] == MASK && dist[q_i][q_j] == 0) {
                                //(∗ q is plateau pixel ∗)
                                dist[q_i][q_j] = curDist + 1;
                                fifo.push(QPoint(q_i,q_j));
                            }
                        }
                    }
                }//endfor 51
        } //end loop

        //(∗ detect and process new minima at level h ∗)
        for (int p_i = 0; p_i < width; p_i++)
            for (int p_j = 0; p_j < height; p_j++) {
                if (qGray(image->pixel(p_i,p_j)) == *h) {
                    //  (∗ reset distance to zero ∗)
                    dist[p_i][p_j] = 0;

                    if (lab[p_i][p_j] == MASK) { //56
                        curlab += 1;
                        fifo.push(QPoint(p_i,p_j));
                        lab[p_i][p_j] = curlab;
                        while (!fifo.empty()) { //59
                            QPoint q = fifo.front();
                            fifo.pop();
                            for (int x = -1; x < 2; x++)
                                for (int y = -1; y < 2; y++) { // 61 (∗ inspect neighbours of q ∗)
                                    if (x != 0 || y != 0) {
                                        int q_i = q.x();
                                        int q_j = q.y();
                                        int r_i = q_i + x;
                                        int r_j = q_j + y;
                                        if (r_i >= 0 && r_j >= 0 && r_i < width && r_j < height) {
                                            if (lab[r_i][r_j] == MASK) {
                                                fifo.push(QPoint(r_i, r_j));
                                                lab[r_i][r_j] = curlab;
                                            }//endif
                                        }//endif
                                    }//endif
                                }//end for
                        }// end while
                    }//end if
                }//end if
            }//end for

    }//end for h

    int max = 0;
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            if (lab[i][j] > max)
                max = lab[i][j];

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            newImage->setPixel(i, j, (lab[i][j]) * 255/max);


    return newImage;
}
