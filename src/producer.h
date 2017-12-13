//
// Created by bartek on 05.12.17.
//

#ifndef FIFO_BUFFER_PRODUCER_H
#define FIFO_BUFFER_PRODUCER_H

#include <semaphore.h>
#include "buffer.h"

typedef struct Producer {
    unsigned int id;

    unsigned int producedAtOnce;
    sem_t* printMutex;

    int (*produce)(struct Producer *self, struct Buffer *buffer);
} Producer;

int produce(struct Producer *self, struct Buffer *buffer);

Producer *newProducer(unsigned int id,
                      unsigned int producedAtOnce,
                      sem_t *printMutex);

void runProducer(unsigned int id, unsigned int producedAtOnce, Buffer* buffer);

#endif //FIFO_BUFFER_PRODUCER_H
