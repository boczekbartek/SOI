//
// Created by bartek on 29.11.17.
//

#ifndef FIFO_BUFFER_BUFFER_H
#define FIFO_BUFFER_BUFFER_H

#include "types.h"

typedef struct Buffer{
    int begIdx,endIdx;
    bool readable;

    /* methods */
    void (*init)(struct Buffer* self);

};

#endif //FIFO_BUFFER_BUFFER_H
