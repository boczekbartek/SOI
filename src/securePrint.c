//
// Created by bartek on 06.12.17.
//

#include <semaphore.h>
#include <cstdarg>
#include <cstdio>

#include "securePrint.h"

void securePrintf(const char * fmt, ...){
    sem_wait(&sem);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    sem_post(&sem);
}
