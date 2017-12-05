//
// Created by bartek on 05.12.17.
//

#ifndef FIFO_BUFFER_CONSUMER_H
#define FIFO_BUFFER_CONSUMER_H

typedef struct Consumer{
    unsigned int id;
}Consumer;

Consumer* newConsumer(unsigned int id);

#endif //FIFO_BUFFER_CONSUMER_H
