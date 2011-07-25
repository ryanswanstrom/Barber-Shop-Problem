//
// File:   main.cpp
// Author: Ryan M. Swanstrom
//
//  This is a C++ implementation of the Barber Shop problem.
//  For the problem, see problem 6.39 in Silberschatz, Galvin and Gagne 8th Ed.
//
// This actual implementation will have One BarberShop and multiple Barbers.
//
// Created on October 24, 2008, 10:10 AM
//

#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <vector>
#include "barbershop.h"

using namespace std;

struct shop_data{
    int seats;      // number of seats in barbershop
    int barbers;    // number of barbers in the barbershop
};

// global variables
barbershop bshop;
int MAX_WAIT = 53400;  // max wait time for a customer


// method declarations
void *create_customer(void *param); // thread function
void *open_shop(void *param); // thread function
void *start_barbers(void *param);  //thread function
void *start_barber(void *param); // thread function

// This main() method should accept 3 parameters (# of customers and
// # of seats and # of barbers).  It should then open the Barbershop and create
// customers at some random interval.
int main(int argc, char** argv) {
    if (argc == 4) {
        int customers = atoi(argv[1]);
        shop_data shopData;
        shopData.seats = atoi(argv[2]);
        shopData.barbers = atoi(argv[3]);
        // create the barbershop thread
        pthread_t shop_tid;
        pthread_create(&shop_tid, NULL, open_shop, (void*)&shopData);
        pthread_join(shop_tid, NULL);
        
        pthread_t barbers_tid;
        pthread_create(&barbers_tid, NULL, start_barbers, (void*)&shopData);
        
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
        pthread_join(barbers_tid, NULL);
        bshop.cleanup();
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
void *open_shop(void *param) {
    shop_data* shopData = (shop_data*)param;
    bshop.open(shopData->seats, shopData->barbers);   
    
    pthread_exit(0);
}

void *start_barbers(void *param) {
    shop_data* shopData = (shop_data*)param;
    
    vector<pthread_t> bThreadIDs;
    // start a new thread for each Barber
    for(int i = 0; i < (shopData->barbers); i++) {
        pthread_t tid;
        pthread_create(&tid, NULL, start_barber, (void*)&i);   //create the thread
        bThreadIDs.push_back(tid);
    }
    
    // join all threads
    for (int i = 0; i < bThreadIDs.size(); i++) {
        pthread_join(bThreadIDs.at(i), NULL);
    }
    pthread_exit(0);
}

/**
 * This method will let a barber start cutting hair.
 */
void *start_barber(void *param) {
    int b_id = *((int*)param);
    bshop.perform_cuts(b_id);
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

