//
// Created by bartek on 05.12.17.
//

#include <stdlib.h>
#include <unistd.h>
#include "consumer.h"
#include "securePrint.h"
#include "stdio.h"

Consumer *newConsumer(unsigned int id, sem_t *printMutex) {
    Consumer *c = malloc(sizeof(Consumer));
    c->printMutex = printMutex;
    securePrintf(c->printMutex,"New Consumer with id: %d\n", id);

    c->id = id;
    c->consume = &consume;
    return c;
}

char consume(Consumer *self, Buffer *buffer) {
    securePrintf(self->printMutex, "Consumer %d is consuming.\n", self->id);
    char elem = buffer->pop(buffer, self);
    if (elem) {
        securePrintf(self->printMutex,"Consumer %d has taken element from "
                             "buffer\n",
                     self->id);
        return elem;
    } else {
        securePrintf(self->printMutex,"Consumer %d could not take element from buffer\n",
                     self->id);
        return elem;
    }
}

//void runConsumer(unsigned int id, Buffer *buffer) {
//    Consumer *consumer = newConsumer(id);
//    while (1){
//        consumer->consume(consumer, buffer);
//        sleep(2);
//    }
//}
