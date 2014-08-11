#ifndef seg_update_mask_H
#define seg_update_mask_H

#include "array2d.h"
#include <QPoint>

void seg_update_mask(const Array2D &array,Array2D &mask,QPoint point,float threshold);

#endif
