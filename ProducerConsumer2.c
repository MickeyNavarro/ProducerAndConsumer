#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <errno.h>

//Almicke Navarro
//CST-221
//September 24, 2019
//This is my own work.

//NEW VERSION - this version, I have implemented a working buffer. We have the use of sleep() implemented as well. 

//SOLUTION: Create 2 threads (one for producer() and consumer()) and then join them so that they may wait for each other to terminate; Make sure to call the producer thread before the consumer thread to allow the producer to produce first before the consumer comes in

//Resources: https://www.geeksforgeeks.org/multithreading-c-2/
//           https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_join.html
//           https://stackoverflow.com/questions/31082559/warning-passing-argument-1-of-pthread-join-makes-integer-from-pointer-without
//           https://timmurphy.org/2010/05/04/pthreads-in-c-a-minimal-working-example/
//           https://fresh2refresh.com/c-programming/c-programs/c-program-to-find-array-size/



//define buffer
#define MAX 10
int buffer[MAX];

//define a counter to determine how many products are produced/yet to be consumed
int counter = 0;

//define the size of buffer
int length = sizeof(buffer)/sizeof(int);

//define methods
int produce();
void put();
void *producer();
void *consumer();
void consume();
int get();

//code given by instructor
int theProduct;

// The Child PID if the Parent else the Parent PID if the Child
pid_t otherPid;

//method to simply increment the product, hence creating a new product
int produce(){
    return theProduct+1;
}

//method to output that the product has been consumed
void consume(int i){
    printf("Consumed: %i\n", i);
}

//method to put the product in the buffer
void *producer() {
    printf("/***Producing***/\n");
    
    int i;
    //loop for forever
    while(1){
        //check if the buffer is full
        if(length == counter){
            
            //if yes,
            //output
            printf("Buffer is full. No new products may be added.\n");
            //put the producer to sleep
            printf("Producer will now been put to sleep.\n");
            sleep(60); //program will temperarily stop here
        }
        else {
            //if no,
            //output
            printf("Buffer is not full. You may add new products.\n");
            //produce the new product
            i = produce();
            //put the product into the buffer
            put(i);
            //increase counter
            counter++;
        }
        
    }
}

//method to consumer the product from the buffer
void *consumer() {
    printf("/***Consuming***/\n");
    
    int i;
    //loop for forever
    while(1){
        //check if buffer is full
        if (counter <= 10 && counter > 0){
            //if yes,
            printf("Buffer is full. Products may now be consumed.\n");
            //get the most recent product from the buffer
            i = get();
            //consume the product
            consume(i);
            //decrease the counter
            counter--;
            //check if the buffer is empty
            if (length ==0){
                printf("Buffer is empty. Products may NOT be consumed.\n");
                printf("Consumer will now been put to sleep.\n");
                //if yes, go to sleep
                sleep(60);
            }
        }
    }
}


//code written by student

//method to receive what the producer creates
void put(int i) {
    //set the product to the given int
    theProduct = i;
    //output what has been produced
    printf("Produced: %i\n", i);
    //put into buffer
    buffer[counter] = i;
    return;
}

//method to get the product from the buffer
int get() {
    //get the most recent product produced from the buffer
    int productToBeConsumed = buffer[counter];
    return productToBeConsumed;
}

//main method will be used to manage the processes and threads
void main() {
    //define 2 threads through the use of an array
    //side note: when creating multiple threads, put them into an array when declaring them; defining each thread with its own ID will cause errors
    pthread_t threads[2];
    
    //create threads for each the producer and consumer
    //parameters: ID, specific attributes, name of function to be executed/entry point, arguments that need to be passed to the function
    if (pthread_create(&threads[0], 0, producer, 0)) {
        printf("\n ERROR creating producer thread");
        exit(1);
    }
    if (pthread_create(&threads[1], 0, consumer, 0)){
        printf("\n ERROR creating consumer thread");
        exit(1);
    }
    
    //wait for thread termination
    //parameters: ID,arguments that need to be passed to the function
    if (pthread_join(threads[0],0)) {
        printf("\n ERROR producer thread is not joinable");
        exit(1);
    }
    if (pthread_join(threads[1],0)){
        printf("\n ERROR consumer thread is not joinable");
        exit(1);
    }
}
