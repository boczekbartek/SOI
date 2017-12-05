//
// Created by bartek on 05.12.17.
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "producer.h"

int produce(struct Producer *self, struct Buffer *buffer) {
    Element **produced = malloc(self->producedAtOnce * sizeof(Element *));
    for (int i = 0; i < self->producedAtOnce; ++i) {
        Element *produced_elem = newElem('A' + (random() % 26));
        produced[i] = produced_elem;
    }
    for (int i = 0; i < self->producedAtOnce; ++i) {
        printf("Produced char: \'%c\' by producer with id = %d\n",
               produced[i]->data, self->id);
        buffer->push(buffer, produced[i]);
    }
    free(produced);
    return 0;
}

Producer *
newProducer(const unsigned int id, const unsigned int producedAtOnce) {
    /**
    *
    *@return Initialized Producer structure
    */
    Producer *p = (Producer *) malloc(sizeof(Producer));
    p->id = id;
    p->producedAtOnce = producedAtOnce;
    p->produce = &produce;
    return p;
}