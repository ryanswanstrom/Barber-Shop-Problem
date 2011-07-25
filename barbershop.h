//
// File:   barbershop.h
// Author: Ryan M. Swanstrom
//
// Created on October 24, 2008, 10:13 AM
//
#include <iostream>
#include <queue>
#include <pthread.h>
using namespace std;

#ifndef _BARBERSHOP_H
#define	_BARBERSHOP_H

#endif	/* _BARBERSHOP_H */

class barbershop {
private:
    const static int MAX_HAIR_TIME = 60000;
    const static int BARBER_ASLEEP = 0;
    const static int BARBER_BUSY = 1;
    const static int BARBER_AWAKE = 2;
    queue<int> chairs;   // a queue of chairs, the int value is the customer ID
    int MAX_CHAIRS;
    bool shop_state;  // false is closed, true is open
    int barber_state;
    
    pthread_mutex_t mutex;  // used for mutual exclusion of barbershop data
    pthread_cond_t cond_cust;  // used to synchronize the customers in the shop
    pthread_cond_t cond_shop; // used to synchronize the barber
    
public:
    barbershop();
    
    void open(int num_chairs);
    void close();
        
    void perform_cuts();
    
    bool request_cut(int id);
    
};

