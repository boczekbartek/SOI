//
// Created by bartek on 05.12.17.
//

#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>

Buffer *newBuffer(size_t bufferSize) {
    Buffer *b = (Buffer *) malloc(sizeof(Buffer));
    b->begIdx = 0;
    b->endIdx = 0;
    b->bufferSize = bufferSize;
    b->content = (char *) malloc(b->bufferSize);
    for (int i = 0; i < b->bufferSize; ++i) {
        b->content[i] = '\0';
    }
    b->readable = false;
    b->printBuffer = &printBuffer;
    b->push = &push;
    b->pop = &pop;
    return b;
}

void printBuffer(struct Buffer *self) {
    printf("Buffer content:\n");
    printf("Buffer start: %d , end: %d\n", self->begIdx, self->endIdx);
    for (int i = 0; i < self->bufferSize; ++i) {
        char elem = self->content[i];
        if (elem) {
            printf("buffer[%d] = %c\n", i, elem);
        }
    }
}

int push(struct Buffer *self) {
    //TODO
}

char pop(struct Buffer *self) {
    //TODO
}
