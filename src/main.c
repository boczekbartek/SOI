#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <stdarg.h>
#include <signal.h>

static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

//#include "securePrint.h"
//#include "config.h"

#define BUF_SIZE 10
#define CONSUMERS 2
#define MIN_BUF_ELEMS_TO_READ 3


struct Consumer;

typedef struct Element {

    char data;

    // Each index of array responds to index number of consumer. When
    // consumer 0 reads this element, alreadyReadBy[0] will be set to true.
    int alreadyReadBy[CONSUMERS];

    /* methods */
    void (*init)(struct Element *self);
} Element;


void securePrintf(sem_t *printMutex, const char *fmt, ...) {
    sem_wait(printMutex);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    sem_post(printMutex);

}

typedef struct Buffer {
    unsigned int begIdx, endIdx;
    int id;
    int readByCnt; //counter of consumers that already read first element
    struct Element content[BUF_SIZE]; //pointer to table with content
    unsigned int bufferSize;
    /* semaphores */
    sem_t *mutex;
    sem_t *items;
    sem_t *spaces;
    sem_t *firstElemMutex;
    sem_t *printMutex;

    /* methods */
    char (*pop)(struct Buffer *self, struct Consumer *consumer);

    int (*push)(struct Buffer *self, Element *element);

    void (*printBuffer)(struct Buffer *self);
} Buffer;

Element *newElem(const char data);

typedef struct Consumer {
    unsigned int id;

    sem_t *printMutex;

    /* methods */
    char (*consume)(struct Consumer *self, struct Buffer *buffer);
} Consumer;

char consume(Consumer *self, Buffer *buffer);

Consumer *newConsumer(unsigned int id, sem_t *printMutex) {
    Consumer *c = malloc(sizeof(Consumer));
    c->printMutex = printMutex;
//    securePrintf(c->printMutex, "New Consumer with id: %d\n", id);

    c->id = id;
    c->consume = &consume;
    return c;
}

void printBuffer(struct Buffer *self) {
    sem_wait(self->mutex);

//    securePrintf(self->printMutex, "\t___Printing buffer, critic section "
//            "entered___\n");

//    securePrintf(self->printMutex, "\t___Critic section push entered___\n");
    int it;
    sem_getvalue(self->items, &it);

    if (it == 0) {
//        printf("Buffer empty.\n");
        securePrintf(self->printMutex, "\t\tBuffer empty.\n");
        sem_post(self->mutex);
//        securePrintf(self->printMutex, "\t___Printing buffer, critic section "
//                "left___\n");
        return;
    }

//    securePrintf(self->printMutex,"Buffer start: %d\tend: %d\n",
//                 self->begIdx, self->endIdx);
    securePrintf(self->printMutex, "\t\tBuffer start: %d\tend: %d\n",
                 self->begIdx, self->endIdx);
    char data;
    sem_wait(self->printMutex);
    if (self->begIdx < self->endIdx) {
        for (int i = self->begIdx; i < self->endIdx; ++i) {
            data = self->content[i].data;
            printf("\t\tbuffer[%d] = \'%c\'\n", i, data);
        }
    } else {
        for (int i = self->begIdx; i < self->bufferSize; ++i) {
            data = self->content[i].data;
            printf("\t\tbuffer[%d] = \'%c\'\n", i, data);
        }
        for (int i = 0; i < self->endIdx; ++i) {
            data = self->content[i].data;
            printf("\t\tbuffer[%d] = \'%c\'\n", i, data);
        }
    }
    sem_post(self->printMutex);
    sem_post(self->mutex);
//    securePrintf(self->printMutex, "\t___Printing buffer, critic section "
//            "left___\n");

}


int push(struct Buffer *self, Element *element) {
    /**
     * @return 0 if pushed successfully, 1 if not
     */


    sem_wait(self->spaces);
    sem_wait(self->mutex);

    int sp, it;

//    securePrintf(self->printMutex, "\t___Critic section push entered___\n");

    sem_getvalue(self->spaces, &sp);
    sem_getvalue(self->items, &it);
    securePrintf(self->printMutex, "\tvalue of sem in push: spaces: %d "
                         "items: "
                         "%d\n",
                 sp, it);
//    int numOfElemsInBuf = abs(self->endIdx - self->begIdx);
//    if (numOfElemsInBuf >= self->bufferSize) {
//        sem_post(self->mutex);
//        printBuffer(self);
//        sem_post(self->spaces);
//        return 1;
//    }
    self->content[self->endIdx] = *element;

    if (self->endIdx + 1 < self->bufferSize) {
        self->endIdx += 1;
    } else {
        self->endIdx = 0;
    }

    securePrintf(self->printMutex, "\tBuffer pushed \'%c\', elements in buffer "
            "after push: %d\n", element->data, it + 1);
    sem_post(self->mutex);
    sem_post(self->items);
    printBuffer(self);

//    securePrintf(self->printMutex, "\t___Critic section push left___\n");

    return 0;
}

char pop(struct Buffer *self, Consumer *consumer) {
    securePrintf(self->printMutex, "Consumer: %d trying to read first "
                         "element\n",
                 consumer->id);
    sem_wait(self->mutex);

    //this element was already read by this consumer
    int elementsCnt;
    sem_getvalue(self->items, &elementsCnt);
    if (elementsCnt < MIN_BUF_ELEMS_TO_READ) {
        securePrintf(self->printMutex, "Cannot read first element, to"
                " few elements in buffer [%d]\n", elementsCnt);
        sem_post(self->mutex);
        return '\0';
    }
    sem_post(self->mutex);

//    securePrintf(self->printMutex, "\t___Critic section push entered___\n");
    sem_wait(self->firstElemMutex);
    Element *first_elem = &self->content[self->begIdx];

//    securePrintf(self->printMutex, "\t___Critical section for reading first "
//            "elem entered in pop___\n");
    if (first_elem->alreadyReadBy[consumer->id] == 1) {
        securePrintf(self->printMutex, "First element [\'%c\'] was already "
                             "read by consumer with id: %d\n",
                     first_elem->data, consumer->id);
        sem_post(self->firstElemMutex);
//        securePrintf(self->printMutex,
//                     "\t___Critical section for reading first "
//                             "elem left in pop___\n");
        return '\0';
    } else if (elementsCnt < MIN_BUF_ELEMS_TO_READ &&
               self->readByCnt == CONSUMERS - 1) {
        securePrintf(self->printMutex, "Cannot read first element [\'%c\'], to"
                " few elements in "
                "buffer "
                "[%d]\n", first_elem->data, elementsCnt);
        sem_post(self->firstElemMutex);
//        securePrintf(self->printMutex,
//                     "\t___Critical section for reading first "
//                             "elem left in pop___\n");
        return '\0';
    } else {
        first_elem->alreadyReadBy[consumer->id] = 1;
        self->readByCnt += 1;
        sem_post(self->firstElemMutex);
//        securePrintf(self->printMutex,
//                     "\t___Critical section for reading first "
//                             "elem left in pop___\n");

        // do bit multiplication on alreadyReadBy array to determine if all
        // consumers has already read this element
        int res = 1;
        for (int i = 0; i < CONSUMERS; ++i) {
            res &= first_elem->alreadyReadBy[i];
        }

        // element can be deleted from buffer, it was read by all consumers
        sem_wait(self->items);
        sem_wait(self->mutex);
//    securePrintf(self->printMutex, "\t___Critical section for removing "
//            "elem entered in pop___\n");
        int sp, it;
        sem_getvalue(self->spaces, &sp);
        sem_getvalue(self->items, &it);
        securePrintf(self->printMutex,
                     "\tvalue of sem in pop: spaces: %d items: "
                             "%d\n",
                     sp, it);
        if (res == 1) {
            securePrintf(self->printMutex, "\tRead by all, \'%c\' removed, "
                    "actual elems in buffer: %d\n", first_elem->data, it);
            self->content[self->begIdx] = *newElem('\0');
            if (self->begIdx + 1 >= self->bufferSize) {
                self->begIdx = 0;
            } else {
                self->begIdx += 1;
            }
            self->readByCnt = 0;
            sem_post(self->mutex);
            sem_post(self->spaces);
            printBuffer(self);
        } else {
            securePrintf(self->printMutex,
                         "\tNot all consumers read first elem yet: %c\n",
                         self->content[self->begIdx]);

            //Not removing
            sem_post(self->mutex);
            sem_post(self->items);
            printBuffer(self);
        }
        return first_elem->data;
    }


//    securePrintf(self->printMutex, "\t___Critical section for removing "
//            "elem left in pop___\n");
}


Buffer *newBuffer(Buffer *b, int id, unsigned int bufferSize, sem_t
*printMutex, sem_t *spaces, sem_t *items, sem_t *bufforMutex, sem_t
                  *firstElemMutex) {
//    b = (Buffer *) malloc(sizeof(Buffer));
    b->printMutex = printMutex;
    b->spaces = spaces;
    b->items = items;
    b->firstElemMutex = firstElemMutex;
    b->mutex = bufforMutex;
//    securePrintf(b->printMutex, "New Buffer with size: %d\n", bufferSize);
    b->id = id;
    b->begIdx = 0;
    b->endIdx = 0;
    b->bufferSize = bufferSize;

    for (int i = 0; i < b->bufferSize; ++i) {
        b->content[i] = *newElem('\0');
    }
    b->readByCnt = 0;
    b->printBuffer = &printBuffer;
    b->push = &push;
//    b->pop = &pop;

    return b;
}


//void createSubporcess(void (*function)(), sem_t *printMutex, const char
//*funName) {
//    pid_t pid = fork();
//    if (pid == 0) {
//        securePrintf(printMutex, "Function name %s\n", funName);
//        function();
//    }
//}

sem_t *attachMutex(const char *memoryRegion) {
    /* Allocate a shared memory segment. */
//    printf("Attaching %s mutex...\n", memoryRegion);
    sem_t *rptr;
    int fd;
    fd = shm_open(memoryRegion, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1);
//        printf("Error");

    if (ftruncate(fd, sizeof(sem_t)) == -1);
//        printf("Error");


    rptr = mmap(NULL, sizeof(sem_t),
                PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (rptr == MAP_FAILED)
        printf("Error");

    return rptr;

//    int shm_id = shmget(IPC_PRIVATE, sizeof(sem_t),
//                        IPC_CREAT |
//                        SHM_R |
//                        SHM_W);
//    return (sem_t *) shmat(shm_id, NULL, 0);
}

Buffer *attachBuffer() {
    /* Allocate a shared memory segment. */
//    printf("\t\t\t***Attaching buffer...***\n");
    Buffer *rptr;
    int fd;
    fd = shm_open("/bufferMem", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
        printf("Error");

    if (ftruncate(fd, sizeof(Buffer)) == -1)
        printf("Error");


    rptr = mmap(NULL, sizeof(Buffer),
                PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (rptr == MAP_FAILED)
        printf("Error");

    return rptr;

//    int shmId = shmget(IPC_PRIVATE, sizeof(Buffer) + BUF_SIZE * sizeof(Element),
//                       IPC_CREAT |
//                       SHM_R |
//                       SHM_W);
//
//    /* Attach the shared memory segment. */
//    return (Buffer *) shmat(shmId, NULL, 0);
}

//Buffer *initBuffer(sem_t *printMutex) {
//    printf("\t\t\t****Buffer init...****\n");
//    securePrintf(printMutex, "%d\n", buffer->id);
//    memset(buffer, 0, sizeof(buffer));
//    return buffer;
//}

sem_t *initStructures() {
//    printf("\t\t\t****Print mutex init...****\n");

    sem_t *printMutex = attachMutex("/printMutex");
    sem_init(printMutex, 1, 1);

    sem_t *items = attachMutex("/items");
    sem_init(items, 1, 0);

    sem_t *spaces = attachMutex("/spaces");
    sem_init(spaces, 1, BUF_SIZE);

    sem_t *bufforMutex = attachMutex("/bufforMutex");
    sem_init(bufforMutex, 1, 1);

    sem_t *firstElemMutex = attachMutex("/firstElemMutex");
    sem_init(firstElemMutex, 1, 1);

    sem_t *productionMutex = attachMutex("/productionMutex");
    sem_init(productionMutex, 1, 1);
    /*init semaphores */

    Buffer *buffer = attachBuffer();
    buffer = newBuffer(buffer, 10, BUF_SIZE, printMutex, spaces, items,
                       bufforMutex, firstElemMutex);

    return printMutex;
}

void end(sem_t *printMutex) {
    securePrintf(printMutex, "Cleaning shared memory\n");
    shm_unlink("/printMutex");
    shm_unlink("/bufferMem");
    shm_unlink("/bufferMem");
    shm_unlink("/items");
    shm_unlink("/spaces");
    shm_unlink("/bufforMutex");
    shm_unlink("/firstElemMutex");
    shm_unlink("/productionMutex");
    exit(0);
}

typedef struct Producer {
    unsigned int id;

    unsigned int producedAtOnce;
    sem_t *printMutex;
    sem_t *productionMutex;

    int (*produce)(struct Producer *self, struct Buffer *buffer);
} Producer;


//
int produce(struct Producer *self, struct Buffer *buffer) {
//    securePrintf(self->printMutex, "...Producing element by producer %d...\n",
//                 self->id);
//    printf("Producing element by producer %d\n", self->id);
    Element produced[self->producedAtOnce];
    for (int i = 0; i < self->producedAtOnce; ++i) {
        Element *produced_elem = newElem('A' + (random() % 26));
        produced[i] = *produced_elem;
    }
    if (self->producedAtOnce == 1) {
        securePrintf(self->printMutex, "Produces %d produces %d elems: "
                             "\'%c\'\n",
                     self->id, self->producedAtOnce, produced[0].data);
    } else {
        securePrintf(self->printMutex, "Produces %d produces %d elems: \'%c\', "
                             "\'%c\'\n",
                     self->id, self->producedAtOnce, produced[0].data,
                     produced[1].data);
    }

    sem_wait(self->productionMutex);
//    securePrintf(self->printMutex, "\t___Entered production critic section by "
//            "producer: %d___\n", self->id);
    for (int i = 0; i < self->producedAtOnce; ++i) {
//        printf("Produced char: \'%c\' by producer "
//                       "with id = %d\n",
//               produced[i]->data, self->id);
//
//        securePrintf(self->printMutex, "Producer: %d char: \'%c\'\n",
//                     self->id, produced[i].data);

        push(buffer, &produced[i]);
    }
    sem_post(self->productionMutex);

//    securePrintf(self->printMutex, "\t___left production critic section by "
//            "producer: %d___\n", self->id);
    return 0;
}

Producer *newProducer(const unsigned int id,
                      const unsigned int producedAtOnce, sem_t *printMutex,
                      sem_t *productionMutex) {
    /**
    *
    *@return Initialized Producer structure
    */

//    securePrintf(printMutex, "\t\t\t***New Producer with id: %d***\n", id);
//    printf("New Producer with id: %d\n", id);
    Producer *p = (Producer *) malloc(sizeof(Producer));
    p->productionMutex = productionMutex;
    p->printMutex = printMutex;
    p->id = id;
    p->producedAtOnce = producedAtOnce;
    p->produce = &produce;
    return p;
}


void print(sem_t *printMutex) {
    pid_t pid = fork();
    Buffer *buffer = attachBuffer();

    if (pid == 0) {
        while (1) {
            printBuffer(buffer);
            sleep(2);
        }

    } else if (pid < 0) {

        securePrintf(buffer->printMutex, "!!!..Forking error in print!..!!!\n");
    }
}

void attachMutexes(Buffer *buffer) {
}

void produce1(sem_t *printMutex) {

    Buffer *buffer = attachBuffer();
    sem_t *productionMutex = attachMutex("/productionMutex");

    Producer *producer = newProducer(0, 1, printMutex, productionMutex);
    srand(time(NULL));
    pid_t pid = fork();
    if (pid == 0) {
        for (int i = 0; i < 8; ++i) {
            produce(producer, buffer);
            sleep(1);
        }
    } else if (pid < 0) {
        securePrintf(buffer->printMutex, "!!!..Forking error in produce1!."
                ".!!!\n");
    }
}

void produce2(sem_t *printMutex) {

    Buffer *buffer = attachBuffer();
    sem_t *productionMutex = attachMutex("/productionMutex");
    Producer *producer = newProducer(1, 2, printMutex, productionMutex);
    srand(time(NULL));
    pid_t pid = fork();
    if (pid == 0) {
        for (int i = 0; i < 7; ++i) {
            produce(producer, buffer);
            usleep(1500000);
        }
    } else if (pid < 0) {
        securePrintf(buffer->printMutex, "!!!..Forking error in produce2!."
                ".!!!\n");
    }
}

void consume1(sem_t *printMutex) {
    Buffer *buffer = attachBuffer();
    Consumer *consumer = newConsumer(0, printMutex);
    pid_t pid = fork();
    if (pid == 0) {
        for (int i = 0; i < 9; ++i) {
            consume(consumer, buffer);
            usleep(1700000);
        }
    } else if (pid < 0) {
        securePrintf(buffer->printMutex, "!!!..Forking error in consume1!."
                ".!!!\n");
    }

}

void consume2(sem_t *printMutex) {
    Buffer *buffer = attachBuffer();
    Consumer *consumer = newConsumer(1, printMutex);
    pid_t pid = fork();
    if (pid == 0) {
        for (int i = 0; i < 10; ++i) {
            consume(consumer, buffer);
            usleep(2300000);
        }
    } else if (pid < 0) {
        securePrintf(buffer->printMutex, "!!!..Forking error in consume2!."
                ".!!!\n");
    }

}

int main() {
    sem_t *printMutex = initStructures();

//    print(printMutex);
    produce1(printMutex);
    produce2(printMutex);
    consume1(printMutex);
    consume2(printMutex);
//    createSubporcess(print, printMutex, "print");
//    createSubporcess(produce1, printMutex, "produce1");

    int status = 0;
    pid_t wpid;

    signal(SIGINT, intHandler);

    while (keepRunning && (wpid = wait(&status)) > 0) {
        printf("Exit status of %d was %d (%s)\n", (int) wpid, status,
               (status > 0) ? "accept" : "reject");
        exit(0);
    }
    end(printMutex);
    return 0;
}

char consume(Consumer *self, Buffer *buffer) {
//    securePrintf(self->printMutex, "Consumer %d is consuming.\n", self->id);
    char elem = pop(buffer, self);
    if (elem != '\0') {
        securePrintf(self->printMutex, "Consumer %d has taken \'%c\' from "
                             "buffer\n",
                     self->id, elem);
        return elem;
    } else {
        securePrintf(self->printMutex,
                     "Consumer %d could not take element from buffer\n",
                     self->id);
        return elem;
    }
}

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
