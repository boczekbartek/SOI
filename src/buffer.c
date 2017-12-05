//
// Created by bartek on 05.12.17.
//

#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <iso646.h>

Buffer *newBuffer(unsigned int bufferSize) {
    Buffer *b = (Buffer *) malloc(sizeof(Buffer));
    b->begIdx = 0;
    b->endIdx = 0;
    b->bufferSize = bufferSize;
    b->content = malloc(b->bufferSize * sizeof(Element *));
    for (int i = 0; i < b->bufferSize; ++i) {
        b->content[i] = NULL;
    }
    b->readByCnt = 0;
    b->printBuffer = &printBuffer;
    b->push = &push;
    b->pop = &pop;
    return b;
}

void printBuffer(struct Buffer *self) {
    if (abs(self->endIdx - self->begIdx) == 0) {
        printf("Buffer empty.\n");
        return;
    }
    printf("Buffer start: %d\tend: %d\n", self->begIdx, self->endIdx);
    for (int i = self->begIdx; i < self->endIdx; ++i) {
        char data = self->content[i]->data;
        printf("buffer[%d] = \'%c\'\n", i, data);
    }
}

int push(struct Buffer *self, Element *element) {
    //TODO multiprocessing proof
    /**
     * @return 0 if pushed successfully, 1 if not
     */
    int numOfElemsInBuf = abs(self->endIdx - self->begIdx);
    if (numOfElemsInBuf >= self->bufferSize) {
        return 1;
    }
    self->content[self->endIdx] = element;
    if (self->endIdx + 1 < self->bufferSize) {
        self->endIdx += 1;
    } else {
        self->endIdx = 1;
    }
    return 0;

}

char pop(struct Buffer *self, Consumer *consumer) {
    //TODO multiprocessing proof
    printf("Reading first element by consumer with id: %d\n", consumer->id);

    Element *first_elem = self->content[self->begIdx];

    //this element was already read by this consumer
    int elementsCnt = abs(self->endIdx-self->begIdx);
    if (first_elem->alreadyReadBy[consumer->id] == 1) {
        printf("First element [\'%c\'] was already read by consumer with id: %d\n",
               first_elem->data, consumer->id);
        return '\0';
    } else if (elementsCnt < MIN_BUF_ELEMS_TO_READ &&
               self->readByCnt == CONSUMERS - 1) {
        printf("Cannot read first element [\'%c\'], to few elements in buffer "
                       "[%d]\n", first_elem->data, elementsCnt);
        return '\0';
    } else {
        first_elem->alreadyReadBy[consumer->id] = 1;
        self->readByCnt += 1;
    }

    // do bit multiplication on alreadyReadBy array to determine if all
    // consumers has already read this element
    int res = 1;
    for (int i = 0; i < CONSUMERS; ++i) {
        res &= first_elem->alreadyReadBy[i];
    }

    // element can be deleted from buffer, it was read by all consumers
    if (res == 1) {
        printf("Read by all.\n");
        free(self->content[self->begIdx]);
        self->content[self->begIdx] = NULL;
        if (self->begIdx + 1 >= self->bufferSize) {
            self->begIdx = 0;
        } else {
            self->begIdx += 1;
        }
        self->readByCnt = 0;
    }

    return first_elem->data;

}
