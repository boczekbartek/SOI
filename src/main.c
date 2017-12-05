#include <stdio.h>
#include <stdlib.h>
#include "element.h"
#include "config.h"
#include "producer.h"
#include "buffer.h"

int main() {
    Element* e = newElem();
    int i = 0;
    for (i; i < CONSUMERS; ++i) {
        printf("To jest: %s\n", boolNames[e->alreadyReadBy[i]]);
    }

    Buffer* buffer = newBuffer(BUF_SIZE);
    buffer->content[5] = 'a';
    buffer->content[7] = 'c';
    buffer->printBuffer(buffer);
//    Producer* newProducer1 = newProducer(1, 1);
//    Producer* newProducer2 = newProducer(1, 2);
//    if (newProducer2->produce(newProducer2, buffer) == 0) printf("Produced "
//                                                           "successfully!!!\n");
    return 0;
}