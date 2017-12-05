//
// Created by bartek on 29.11.17.
//

#ifndef FIFO_BUFFER_ELEM_H
#define FIFO_BUFFER_ELEM_H

#include "types.h"
#include "config.h"

typedef struct Element {

    char data;

    // Each index of array responds to index number of consumer. When
    // consumer 0 reads this element, alreadyReadBy[0] will be set to true.
    int alreadyReadBy[CONSUMERS];

    /* methods */
    void (*init)(struct Element *self);
} Element;


/*** public methods ***/
/* constructor */
Element *newElem(char data);

#endif //FIFO_BUFFER_ELEM_H
