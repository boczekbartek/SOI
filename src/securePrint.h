//
// Created by bartek on 06.12.17.
//

#ifndef FIFO_BUFFER_SECUREPRINT_H
#define FIFO_BUFFER_SECUREPRINT_H

#include <semaphore.h>

void securePrintf(sem_t *printMutex, const char *fmt, ...);

#endif //FIFO_BUFFER_SECUREPRINT_H
