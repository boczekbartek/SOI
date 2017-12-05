//
// Created by bartek on 29.11.17.
//
#include <stdlib.h>
#include "element.h"


Element *newElem(const char data) {
    /** Constructor of Elem structure. Initialize table of alreadyReadBy to false.
     * Initialize all structure fuction pointers to functions, to act as methods.
     * Methods can be accessed by "." and "->" operators.
    *
    *@return Initialized Elem structure
    */
    Element *e = (Element *) malloc(sizeof(Element));
    for (int i = 0; i < CONSUMERS; ++i) {
        e->alreadyReadBy[i] = 0;
    };
    e->data = data;
    return e;
}