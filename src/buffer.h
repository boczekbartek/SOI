//
// Created by bartek on 29.11.17.
//

#ifndef FIFO_BUFFER_BUFFER_H
#define FIFO_BUFFER_BUFFER_H

#include <glob.h>
#include "types.h"
#include "element.h"
#include "consumer.h"

typedef struct Buffer {
    int begIdx, endIdx;
    bool readable;
    Element **content; //pointer to table with content
    unsigned int bufferSize;
    unsigned int numOfElemsInBuf;

    /* methods */
    char (*pop)(struct Buffer *self, Consumer* consumer);

    int (*push)(struct Buffer *self, Element *element);

    void (*printBuffer)(struct Buffer *self);
} Buffer;

char pop(struct Buffer *self, Consumer* consumer);

int push(struct Buffer *self, Element *element);

Buffer *newBuffer(unsigned int bufferSize);

void printBuffer(struct Buffer *self);

#endif //FIFO_BUFFER_BUFFER_H
