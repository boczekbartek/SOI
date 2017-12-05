//
// Created by bartek on 05.12.17.
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "producer.h"

int produce(struct Producer* self, struct Buffer* buffer) {
    srand(time(NULL));
    char* produced = (char*)malloc(self->producedAtOnce);
    for(int i =0; i<self->producedAtOnce; ++i){
        produced[i] = 'A' + (random() % 26);
    }
    for(int i = 0; i<self->producedAtOnce; ++i) {
        printf("Produced char: %c\n", produced[i]);
    }
    free(produced);
    return 0;
}

Producer* newProducer(const int id, unsigned int producedAtOnce) {
    /**
    *
    *@return Initialized Producer structure
    */
    Producer* p = (Producer*)malloc(sizeof(Producer));
    p->id = id;
    p->producedAtOnce = producedAtOnce;
    p->produce = &produce;
    return p;
}