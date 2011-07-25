//
// File:   main.cpp
// Author: Ryan M. Swanstrom
// 
//  This is a C++ implementation of the Barber Shop problem.
//  For the problem, see problem 6.39 in Silberschatz, Galvin and Gagne 8th Ed.
//  
//
// Created on October 24, 2008, 10:10 AM
//

#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <vector>
#include "barbershop.h"

using namespace std;

// global variables
barbershop bshop;
int MAX_WAIT = 5000;  // max wait time for a customer

// method declarations
void *create_customer(void *param); // thread function
void *create_shop(void *param); // thread function

// This main() method should accept 2 parameters (# of customers and
// # of seats).  It should then open the Barbershop and create
// customers at some random interval.
int main(int argc, char** argv) {
    if (argc ==3) {
        int customers = atoi(argv[1]);
        int seats = atoi(argv[2]);
        // create the barbershop thread
        pthread_t shop_tid;
        pthread_create(&shop_tid, NULL, create_shop, (void*)&seats);
        vector<pthread_t> threadIDs;
        for (int i = 1; i <= customers; i++) {
            
            // wait a random amount of time before sending the next customer
            int r = rand();
            int wait = r % MAX_WAIT;
            usleep(wait);    // sleep for microseconds
            pthread_t tid;
            pthread_create(&tid, NULL, create_customer, (void*)&i);   //create the thread
            // add tid to vector
            threadIDs.push_back(tid);
            
        }
        
        // join all threads
        for (int i = 0; i < threadIDs.size(); i++) {
            pthread_join(threadIDs.at(i), NULL);
        }
        
        bshop.close();
        pthread_join(shop_tid, NULL);
    } else {
        cout << "Incorrect parameters:" << endl;
        cout << " ./BarberShop 100 15" << endl;
        cout << "That would be for 100 customers and 15 seats at the barber shop.";
    }
    
    return (EXIT_SUCCESS);
}

/**
 * This is a function that will run in a new thread.
 * It will open the barbershop for business
 */
void *create_shop(void *param) {
    int seats = *((int*)param);
    bshop.open(seats);
    pthread_exit(0);
}

/**
 * This is a function that will run in a new thread.
 * It will create a new customer and request a hair cut.
 */
void *create_customer(void *param) {
    int id = *((int*)param);
    bool gotCut = bshop.request_cut(id);
    pthread_exit(0);
}
