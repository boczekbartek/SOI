//
// Created by bartek on 05.12.17.
//

#ifndef FIFO_BUFFER_PRODUCER_H
#define FIFO_BUFFER_PRODUCER_H

#include "buffer.h"

typedef struct Producer {
    int id;

    unsigned int producedAtOnce;
    int (*produce)(struct Producer* self, struct Buffer* buffer);
}Producer;

int produce(struct Producer* self, struct Buffer* buffer);

Producer* newProducer(int id, unsigned int producedAtOnce);
#endif //FIFO_BUFFER_PRODUCER_H
