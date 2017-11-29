//
// Created by bartek on 29.11.17.
//

#ifndef FIFO_BUFFER_ELEM_H
#define FIFO_BUFFER_ELEM_H

#include "types.h"
#include "config.h"

typedef struct Elem{

    char data;
    bool alreadyReadBy[CONSUMERS]; // each index of array responds to

    /* methods */
    void (*init)(struct Elem* self);
} Elem;

/*** private methods ***/
static void __initElem(Elem *self);

/*** public methods ***/
/* constructor */
Elem newElem(void);

#endif //FIFO_BUFFER_ELEM_H
