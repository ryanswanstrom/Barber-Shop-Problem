//
// File:   barbershop.h
// Author: Ryan M. Swanstrom
//
// Created on October 24, 2008, 10:13 AM
//
#include <iostream>
#include <queue>
#include <time.h>
#include <pthread.h>
using namespace std;

#ifndef _BARBERSHOP_H
#define	_BARBERSHOP_H

#endif	/* _BARBERSHOP_H */

enum status {ASLEEP, BUSY, AWAKE};

class barbershop {
private:
    const static int MAX_HAIR_TIME = 600000;
    queue<int> chairs;   // a queue of chairs, the int value is the customer ID
    int MAX_CHAIRS;
    int NUM_BARBERS;
    bool shop_state;  // false is closed, true is open
    status* barber_state;  //state of each barber
    clock_t* barber_tosleep; //times when the barbers went to sleep
    
    pthread_mutex_t mutex;  // used for mutual exclusion of barbershop data
    pthread_cond_t cond_cust;  // used to synchronize the customers in the shop
    pthread_cond_t* cond_barber; // used to synchronize the barbers, this is an array
    
public:
    barbershop();
    void cleanup();
    
    void open(int num_chairs, int num_barbers);
    void close();
        
    void perform_cuts(int barber_id);
    
    bool request_cut(int cust_id);
    
};
