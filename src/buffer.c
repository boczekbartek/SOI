//
// Created by bartek on 05.12.17.
//

#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include "securePrint.h"


Buffer *newBuffer(unsigned int bufferSize, sem_t *printMutex) {
    Buffer *b = (Buffer *) malloc(sizeof(Buffer));
    b->printMutex = printMutex;
    securePrintf(b->printMutex, "New Buffer with size: %d\n", bufferSize);

    b->begIdx = 0;
    b->endIdx = 0;
    b->bufferSize = bufferSize;

//    for (int i = 0; i < b->bufferSize; ++i) {
//        b->content[i] = NULL;
//    }
    b->readByCnt = 0;
    b->printBuffer = &printBuffer;
//    b->push = &push;
//    b->pop = &pop;

    /*init semaphores */
    sem_init(&b->mutex, 0, 1);
    sem_init(&b->spaces, 0, b->bufferSize-1);
    sem_init(&b->items, 0, 0);
    sem_init(&b->firstElemMutex,0,1);
    return b;
}

void printBuffer(sem_t *printMutex ,struct Buffer *self) {
    securePrintf(printMutex,"Printing buffer.\n");
    if (abs(self->endIdx - self->begIdx) == 0) {
        printf("Buffer empty.\n");
//        securePrintf(self->printMutex, "Buffer empty.\n");
        return;
    }

//    securePrintf(self->printMutex,"Buffer start: %d\tend: %d\n",
//                 self->begIdx, self->endIdx);
    printf("Buffer start: %d\tend: %d\n",
           self->begIdx, self->endIdx);
    char data;
    for (int i = self->begIdx; i < self->endIdx; ++i) {
        data = self->content[i].data;
//        securePrintf(self->printMutex,"buffer[%d] = \'%c\'\n", i, data);
        printf("buffer[%d] = \'%c\'\n", i, data);
    }
}

//int push(struct Buffer *self, Element *element) {
//    /**
//     * @return 0 if pushed successfully, 1 if not
//     */
//    sem_wait(&self->spaces);
//    sem_wait(&self->mutex);
//
//    int numOfElemsInBuf = abs(self->endIdx - self->begIdx);
//    if (numOfElemsInBuf >= self->bufferSize) {
//        return 1;
//    }
//    self->content[self->endIdx] = &element;
//    if (self->endIdx + 1 < self->bufferSize) {
//        self->endIdx += 1;
//    } else {
//        self->endIdx = 1;
//    }
//    sem_post(&self->mutex);
//    sem_post(&self->items);
//    return 0;
//}
//
//char pop(struct Buffer *self, Consumer *consumer) {
//    securePrintf(self->printMutex,"Reading first element by consumer with id:"
//                         " %d\n",
//                 consumer->id);
//
//    Element *first_elem = self->content[self->begIdx];
//
//    //this element was already read by this consumer
//    int elementsCnt = abs(self->endIdx-self->begIdx);
//    sem_wait(&self->firstElemMutex);
//    if (first_elem->alreadyReadBy[consumer->id] == 1) {
//        securePrintf(self->printMutex,"First element [\'%c\'] was already "
//                             "read by consumer "
//                             "with id: "
//                "%d\n",
//               first_elem->data, consumer->id);
//        sem_post(&self->firstElemMutex);
//        return '\0';
//    } else if (elementsCnt < MIN_BUF_ELEMS_TO_READ &&
//               self->readByCnt == CONSUMERS - 1) {
//        securePrintf(self->printMutex,"Cannot read first element [\'%c\'], to"
//                " few elements in "
//                             "buffer "
//                       "[%d]\n", first_elem->data, elementsCnt);
//        sem_post(&self->firstElemMutex);
//        return '\0';
//    } else {
//        first_elem->alreadyReadBy[consumer->id] = 1;
//        self->readByCnt += 1;
//        sem_post(&self->firstElemMutex);
//    }
//
//    // do bit multiplication on alreadyReadBy array to determine if all
//    // consumers has already read this element
//    int res = 1;
//    for (int i = 0; i < CONSUMERS; ++i) {
//        res &= first_elem->alreadyReadBy[i];
//    }
//
//    // element can be deleted from buffer, it was read by all consumers
//    sem_wait(&self->items);
//    sem_wait(&self->mutex);
//    if (res == 1) {
//        securePrintf(self->printMutex,"Read by all.\n");
//        free(self->content[self->begIdx]);
//        self->content[self->begIdx] = NULL;
//        if (self->begIdx + 1 >= self->bufferSize) {
//            self->begIdx = 0;
//        } else {
//            self->begIdx += 1;
//        }
//        self->readByCnt = 0;
//    }
//    sem_post(&self->mutex);
//    sem_post(&self->spaces);
//
//    return first_elem->data;
//}
//*/