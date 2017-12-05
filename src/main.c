#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "element.h"
#include "config.h"
#include "producer.h"
#include "buffer.h"

int main() {
    srand(time(NULL));

    unsigned int producer2id = 1;
    unsigned int producer2ElementOnceProcuded = 2;
    Producer* producer2 = newProducer(producer2id,
                                      producer2ElementOnceProcuded);

    Buffer *buffer = newBuffer(BUF_SIZE);
    producer2->produce(producer2, buffer);
    producer2->produce(producer2, buffer);
    buffer->printBuffer(buffer);
    Consumer* consumer0 = newConsumer(0);
    Consumer* consumer1 = newConsumer(1);

    buffer->pop(buffer, consumer1);
    buffer->pop(buffer, consumer1);
    buffer->pop(buffer, consumer1);

    buffer->pop(buffer, consumer0);
    buffer->pop(buffer, consumer1);
    buffer->pop(buffer, consumer1);

    buffer->printBuffer(buffer);

    return 0;
}