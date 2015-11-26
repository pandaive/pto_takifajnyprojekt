#include "morph_erode.h"

MorphErode::MorphErode(PNM* img) :
    MorphologicalOperator(img)
{
}

MorphErode::MorphErode(PNM* img, ImageViewer* iv) :
    MorphologicalOperator(img, iv)
{
}

const int MorphErode::morph(math::matrix<float> window, math::matrix<bool> se)
{
    int max = 0;

    for (int x = 0; x < window.rowno(); x++)
        for (int y = 0; y < window.rowno(); y++) {
            if (se(x,y) && window(x,y) > max) {
                max = window(x,y);
            }
        }

    return max;
}
