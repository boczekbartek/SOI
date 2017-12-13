//
// Created by bartek on 06.12.17.
//

#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>

#include "securePrint.h"

void securePrintf(sem_t *printMutex, const char *fmt, ...) {
    sem_wait(printMutex);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    sem_post(printMutex);

}
