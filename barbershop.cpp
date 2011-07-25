//
// File:   barbershop.cpp
// Author: Ryan M. Swanstrom
//
// Created on October 24, 2008, 10:13 AM
//
#include "barbershop.h"

/**
 * barbershop constructor
 * Initialize the mutex and condition variables.
 */
barbershop::barbershop() {
    pthread_mutex_init(&this->mutex, NULL);
    pthread_cond_init(&this->cond_shop, NULL);
    pthread_cond_init(&this->cond_cust, NULL);
}

/**
 * This method opens the barbershop for business.
 * sets MAX_CHAIRS
 * inits Barber to sleeping state
 * sets shop_state to open.
 * A lock was obtained so that no other method could manipulate the data.
 */
void barbershop::open(int num_chairs) {
    // only perform when shop is closed
    if (!this->shop_state) {
        pthread_mutex_lock(&mutex);
        this->MAX_CHAIRS = num_chairs;
        this->barber_state = this->BARBER_ASLEEP;
        this->shop_state = true;
        pthread_mutex_unlock(&mutex);
        cout << "Welcome to the Barber shop. Today we have " << num_chairs
                << " seats" << endl;
        
        // call a method to perform cuts
        this->perform_cuts();
    }
}

/**
 * This method will close the barbershop
 * It sends out 2 signals for condition varibables on cond_shop and
 * cond_cust.
 */
void barbershop::close() {
    pthread_mutex_lock(&mutex);
    cout << " closing shop" << endl;
    if (this->chairs.size() <= 0) {
        pthread_cond_signal(&cond_shop);
    }
    this->shop_state = false;
    pthread_mutex_unlock(&mutex);
    cout << "Barber Shop is closed for the day(not accepting new customers)."
            << endl;
}

/**
 * This is the method to perform the hair cuts
 */
void barbershop::perform_cuts() {
    
    // continuous loop while shop is open
    while( this->shop_state ) {
        pthread_mutex_lock(&mutex);
        if (this->chairs.size() > 0) {
            // pop off queue
            int id = this->chairs.front();  // get element
            this->chairs.pop();     // remove element            
            this->barber_state = this->BARBER_BUSY;
            
            // random wait time for hair cut
            int r = rand();
            int wait = r % this->MAX_HAIR_TIME;
            usleep(wait);    // sleep for microseconds
            cout << "  customer " << id << " is finished with hair cut: "
                    << wait << endl;
            pthread_cond_signal(&this->cond_cust);
        } else {
            cout << " Barber going to sleep, no customers." << endl;
            this->barber_state = this->BARBER_ASLEEP;           
            pthread_cond_wait(&cond_shop, &mutex);  //wait on shop
        }
        pthread_mutex_unlock(&mutex);
    }
}

/**
 * This method will wake the barber if asleep, put the customer
 * in a chair, or send the customer away.
 *
 * A hair cut should take a random amount of time less than MAX_HAIR_TIME
 */
bool barbershop::request_cut(int id) {
    bool got_in = false;
    pthread_mutex_lock(&mutex);
    cout << "  customer " << id << " enters the shop." << endl;
    // only proceed if shop is open
    if (this->shop_state) {
        if (this->chairs.size() < this->MAX_CHAIRS) {
            // check if barber is sleeping
            if (this->barber_state == this->BARBER_ASLEEP) {
                // wake up the barber
                cout << "  customer " << id << " wakes up the Barber" << endl;
                this->barber_state = this->BARBER_AWAKE;
                // signal barber
                pthread_cond_signal(&cond_shop);
            }
            
            // add customer to the queue of chairs
            this->chairs.push(id);
            pthread_cond_wait(&this->cond_cust, &mutex);
            got_in = true;
        } else {
            cout << "  customer " << id << " is rejected (shop is full)." << endl;
        }
        
    } else {
        cout <<  "  customer " << id << " is rejected (shop is closed)." << endl;
    }
    pthread_mutex_unlock(&mutex);
    return got_in;
}

