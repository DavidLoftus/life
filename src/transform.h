#ifndef LIFE_TRANSFORM_H
#define LIFE_TRANSFORM_H

#include "state.h"

namespace life {
    void CountNeighbours(const Grid& src, Grid& dst);
    void NextState(const Grid& src, Grid& dst);
    void Transform(const Grid& src, Grid& dst);
}


#endif //LIFE_TRANSFORM_H
