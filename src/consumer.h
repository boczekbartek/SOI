//
// Created by bartek on 05.12.17.
//

#ifndef FIFO_BUFFER_CONSUMER_H
#define FIFO_BUFFER_CONSUMER_H


#include "buffer.h"

struct Buffer;

typedef struct Consumer{
    unsigned int id;

    sem_t *printMutex;
    /* methods */
    char (*consume)(struct Consumer* self, struct Buffer *buffer);
} Consumer;

Consumer* newConsumer(unsigned int id, sem_t *printMutex);

char consume(Consumer* self, struct Buffer *buffer);

void runConsumer(unsigned int id,  struct Buffer* buffer);

#endif //FIFO_BUFFER_CONSUMER_H
