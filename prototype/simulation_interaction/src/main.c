#include <stdlib.h>
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

typedef enum Order_t{
    ORDER_FORWARD = 1,
    ORDER_BACKWARD = 2,
    ORDER_LEFT = 3,
    ORDER_RIGHT = 4,

    ORDER_NONE = -1
} Order;

typedef _Atomic Order Atomic_order;

struct MessageQueue_t{
    Atomic_order msg;
    struct MessageQueue_t* next;
};
typedef struct MessageQueue_t MessageQueue;

struct CLIParam {
    MessageQueue* messageProducer;
    cnd_t* cond;
    mtx_t* lock;
    atomic_bool* const terminate;
};

struct SimParam {
    const char* exchangeFileName;
    MessageQueue* messageConsumer;
    cnd_t* cond;
    mtx_t* mutex;
    atomic_bool* const terminate;
};

void produceMessage(MessageQueue** head, Order newOrder){
    /**
     * @pre head->next == NULL
     * @pre newOrder != ORDER_NONE
     */

    // if false then we're not at the head so there is a coding error so we abort
    assert((*head)->next == NULL);

    // never add a NONE order or it would block the channel
    if(newOrder == ORDER_NONE) return; 

    // creating the next end of pipe
    MessageQueue* newMessage = (MessageQueue*)malloc(sizeof(MessageQueue));
    *newMessage = (MessageQueue){
        .msg = ORDER_NONE,
        .next = NULL
    };
    (*head)->next = newMessage;

    // send the message and allow for consumption of it
    atomic_store_explicit(&((*head)->msg), newOrder, memory_order_release);
    //! DO NOT DEREFERENCE HEAD UNTIL IT'S MOVED !!!

    // we move the head to the end of the pipe
    *head = newMessage;
    //! WE CAN NOW DEREFERENCE HEAD AGAIN

    /**
     * @post old(head)->next == new(head)
     * @post old(head) is consumable AKA old(head)->msg != ORDER_NONE
     */
}

Order consumeMessage(MessageQueue** tail){

    // get the value
    Order value = atomic_load_explicit(&((*tail)->msg), memory_order_acquire);
    
    // if there is an actual value at the tail
    if(value != ORDER_NONE){
        // we consume it
        MessageQueue* next_tail = (*tail)->next;
        free(*tail);
        *tail = next_tail;
    }

    return value;
}

int simInterfaceProcess(void* param){
    // extract the parameters
    struct SimParam* args = (struct SimParam*)param;
    const char* exchangeFileName = args->exchangeFileName;
    MessageQueue* messageConsumer = args->messageConsumer;
    cnd_t* cond = args->cond;
    mtx_t* mutex = args->mutex;
    atomic_bool* const terminate = args->terminate;
    
    // we continue sending stuff to the simulation until we're told to stop
    while(!atomic_load(terminate)){
        // we wait until there are messages to fetch
        mtx_lock(mutex);
        while(atomic_load(&(messageConsumer->msg)) == ORDER_NONE && !atomic_load(terminate)){
            cnd_wait(cond, mutex);
        }
        mtx_unlock(mutex);

        // then we consume all the messages from the queue until none remains
        Order readOrder;
        while((readOrder = consumeMessage(&messageConsumer)) != ORDER_NONE){
            FILE* fileHandler = fopen(exchangeFileName, "a");
            if(!fileHandler){
                perror("[CRITICAL] - Unable to open the exchange file, aborting\n");
                atomic_store(terminate, true);
                // NOTE : to manage memory correctly we should cleanup all of the MessageQueue here but we'll ignore that since it's a POC
                // TODO : do it properly
                break;
            }
            
            switch(readOrder){
                case ORDER_FORWARD:
                    fputs("FORWARD\n", fileHandler);
                    break;
                case ORDER_BACKWARD:
                    fputs("BACKWARD\n", fileHandler);
                    break;
                case ORDER_LEFT:
                    fputs("LEFT\n", fileHandler);
                    break;
                case ORDER_RIGHT:
                    fputs("RIGHT\n", fileHandler);
                    break;

                default:
                    perror("[WEIRD] - This is a weird situation, we've read a message that's not a valid order from the channel ???\n");
            }

            fclose(fileHandler);
        }
    }
}

void afficherMenu(){
    printf("=== MENU ===\n");
    printf("[1] - Go Forward\n");
    printf("[2] - Go Backward\n");
    printf("[3] - Turn Left\n");
    printf("[4] - Turn Right\n");
    printf("[5] - CLOSE\n");
    printf("USER >> ");
}

size_t readln(char* out, size_t maxlen){
    char in;
    bool hasText = true;
    
    size_t readCount = 0;
    while(hasText){
        in = getchar();
        if(in == '\n' || in == EOF){ // end of line or end of reading
            hasText = false;
        }
        else if(readCount < maxlen - 1){
            out[readCount] = in;
            readCount++;
        }
        else{
            // the remaining char are just consumed to flush the buffer between reads
        }
    }

    out[readCount] = '\0';
    return readCount;
}

int cliProcess(void* param){

    // extract the parameters
    struct CLIParam* args = (struct CLIParam*)param;
    MessageQueue* messageProducer = args->messageProducer;
    cnd_t* cond = args->cond;
    mtx_t* lock = args->lock;
    atomic_bool* const terminate = args->terminate;


    // we continue sending stuff to the simulation until we're told to stop
    while( !atomic_load(terminate) ){
        afficherMenu();
        
        char buffer[2];
        readln(buffer, 2);

        switch(buffer[0]){
            case '1':
                produceMessage(&messageProducer, ORDER_FORWARD);
                break;
            case '2':
                produceMessage(&messageProducer, ORDER_BACKWARD);
                break;
            case '3':
                produceMessage(&messageProducer, ORDER_LEFT);
                break;
            case '4':
                produceMessage(&messageProducer, ORDER_RIGHT);
                break;
            case '5':
                // finish the program
                atomic_store(terminate, true);
                break;

            default:
                printf("[ERROR] - Invalid input, please chose from the menu\n");
        }

        mtx_lock(lock);
        cnd_signal(cond);
        mtx_unlock(lock);
    }
}


int main(){
    atomic_bool terminate = false;
    cnd_t condition_var;
    mtx_t mutex;
    mtx_init(&mutex, mtx_plain);
    cnd_init(&condition_var);

    // allocate the first block of the channel, note that it will be made unusable after the consumer use it
    MessageQueue* channel = (MessageQueue*)malloc(sizeof(MessageQueue));
    channel->msg = ORDER_NONE; // set it as empty
    channel->next = NULL;

    struct CLIParam p1 = {
        .messageProducer = channel,
        .cond = &condition_var,
        .lock = &mutex,
        .terminate = &terminate,
    };

    struct SimParam p2 = {
        .exchangeFileName = "channel.txt",
        .messageConsumer = channel,
        .cond = &condition_var,
        .mutex = &mutex,
        .terminate = &terminate,
    };

    thrd_t ConsoleProcess;
    thrd_t ComunicationProcess;
    if(thrd_create(&ComunicationProcess, simInterfaceProcess, &p2) != thrd_success || thrd_create(&ConsoleProcess, cliProcess, &p1) != thrd_success) {
        perror("[CRITICAL] - We were unable to start the threads\n");
        return -1;
    }
    // Thread started and running
    printf("[SUCCESS] - Threads created\n");
    thrd_join(ConsoleProcess, NULL);
    printf("[LOG] - console closed\n");
    thrd_join(ComunicationProcess, NULL);
    printf("[LOG] - communication closed\n");
    
    printf("[SUCCESS] - End of the program\n");    
    return 0;
}