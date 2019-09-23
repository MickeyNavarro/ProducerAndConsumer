#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>

//Almicke Navarro
//CST-221
//September 16, 2019
//This is my own work.

//SOLUTION:

//Resources: https://www.geeksforgeeks.org/multithreading-c-2/
//           https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_join.html
//           https://stackoverflow.com/questions/31082559/warning-passing-argument-1-of-pthread-join-makes-integer-from-pointer-without
//           https://timmurphy.org/2010/05/04/pthreads-in-c-a-minimal-working-example/


//define buffer
#define BUFFER 10
int buffer[BUFFER];

//define methods
int produce();
void put();
void *producer();
void *consumer();
void consume();
int get();

//code given by instructor
int theProduct;

int produce(){
    return theProduct+1;
}

void consume(int i){
    printf("Consumed: %i\n", i);
}

void *producer() {
    int i;
    while(1){
        i = produce();
        put(i);
        //allow the program to sleep for 1 sec to avoid a fast output
        sleep(1);
    }
}

void *consumer() {
    int i;
    while(1){
        i = get();
        consume(i);
        sleep(1);
    }
}


//code written by student

//method to receive what the producer creates
void put(int i) {
    //set the product to the given int
    theProduct = i;
    printf("Produced: %i\n", i);
    return;
}

//method to return the product
//similar to produce() method, however it only returns the product it gets; does not increase the product
int get() {
    return theProduct;
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
