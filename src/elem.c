//
// Created by bartek on 29.11.17.
//
#include "elem.h"

static void __initElem(Elem *self) {
    int i = 0;
    for (i; i < CONSUMERS; ++i) {
        self->alreadyReadBy[i] = false;
    };

}

Elem newElem(void) {
    /** Constructor of Elem structure. Initialize table of alreadyReadBy to false.
     * Initialize all structure fuction pointers to functions, to act as methods.
     * Methods can be accessed by "." and "->" operators.
    *
    *@return Initialized Elem structure
    */
    Elem e;
    e.init = &__initElem;
    (e.init)(&e);
    return e;
}