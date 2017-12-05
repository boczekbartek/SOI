//
// Created by bartek on 05.12.17.
//

#include <stdlib.h>
#include "consumer.h"

Consumer* newConsumer(unsigned int id){
    Consumer* c = malloc(sizeof(Consumer));
    c->id = id;
    return c;
}