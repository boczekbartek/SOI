//
// Created by bartek on 05.12.17.
//

#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include "producer.h"
#include "securePrint.h"
#include <stdio.h>

int produce(struct Producer *self, struct Buffer *buffer) {
//    securePrintf(self->printMutex, "Producing element by producer %d\n",
//                 self->id);
    printf("Producing element by producer %d\n", self->id);
    Element **produced = malloc(self->producedAtOnce * sizeof(Element *));
    for (int i = 0; i < self->producedAtOnce; ++i) {
        Element *produced_elem = newElem('A' + (random() % 26));
        produced[i] = produced_elem;
    }

    for (int i = 0; i < self->producedAtOnce; ++i) {
//        printf("Produced char: \'%c\' by producer "
//                       "with id = %d\n",
//               produced[i]->data, self->id);
//
        securePrintf(self->printMutex,"Produced char: \'%c\' by producer "
                             "with id = %d\n",
                     produced[i]->data, self->id);
        buffer->push(buffer, produced[i]);
    }
    free(produced);
    return 0;
}

Producer *newProducer(const unsigned int id,
                      const unsigned int producedAtOnce, sem_t *printMutex) {
    /**
    *
    *@return Initialized Producer structure
    */
//    securePrintf(printMutex, "New Producer with id: %d\n", id);
    printf("New Producer with id: %d\n", id);
    Producer *p = (Producer *) malloc(sizeof(Producer));
    p->printMutex = printMutex;
    p->id = id;
    p->producedAtOnce = producedAtOnce;
    p->produce = &produce;
    return p;
}

//void runProducer(unsigned int id, unsigned int producedAtOnce, Buffer* buffer){
//    Producer *producer = newProducer(id, producedAtOnce);
//    while (1) {
//        producer->produce(producer, buffer);
//        sleep(1);
//    }
//}

