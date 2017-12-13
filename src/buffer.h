//
// Created by bartek on 29.11.17.
//

#ifndef FIFO_BUFFER_BUFFER_H
#define FIFO_BUFFER_BUFFER_H

#include <glob.h>
#include <semaphore.h>

#include "consumer.h"
#include "types.h"
#include "element.h"

struct Consumer;
typedef struct Buffer {
    int begIdx, endIdx;
    int readByCnt; //counter of consumers that already read first element
    Element content[BUF_SIZE]; //pointer to table with content
    unsigned int bufferSize;
    /* semaphores */
    sem_t mutex;
    sem_t items;
    sem_t spaces;
    sem_t firstElemMutex;
    sem_t *printMutex;
    /* methods */
    char (*pop)(struct Buffer *self, struct Consumer* consumer);

    int (*push)(struct Buffer *self, Element *element);

    void (*printBuffer)(sem_t *printMutex, struct Buffer *self);
} Buffer;

char pop(struct Buffer *self, struct Consumer* consumer);

int push(struct Buffer *self, Element *element);

Buffer *newBuffer(unsigned int bufferSize, sem_t *printMutex);

void printBuffer(sem_t *printMutex, struct Buffer *self);

#endif //FIFO_BUFFER_BUFFER_H
