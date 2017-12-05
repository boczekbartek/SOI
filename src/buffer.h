//
// Created by bartek on 29.11.17.
//

#ifndef FIFO_BUFFER_BUFFER_H
#define FIFO_BUFFER_BUFFER_H

#include <glob.h>
#include "types.h"

typedef struct Buffer{
    int begIdx,endIdx;
    bool readable;
    char* content;
    size_t bufferSize;
    /* methods */
    void (*init)(struct Buffer* self);
    char (*pop)(struct Buffer* self);
    int (*push)(struct Buffer* self);
    void (*printBuffer)(struct Buffer* self);
} Buffer;

char pop(struct Buffer* self);
int push(struct Buffer* self);

Buffer* newBuffer(size_t bufferSize);
void printBuffer(struct Buffer* self);

#endif //FIFO_BUFFER_BUFFER_H
