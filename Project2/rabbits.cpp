#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>

// #define CSV 1

float Ranf( unsigned int *seedp,  float low, float high ) {
    float r = (float) rand_r( seedp ); // 0 - RAND_MAX
    return( low  +  r * ( high - low ) / (float)RAND_MAX);
}

const float RYEGRASS_GROWS_PER_MONTH = 20.0;
const float ONE_RABBITS_EATS_PER_MONTH = 1.0;

const float AVG_PRECIP_PER_MONTH = 12.0; // average
const float AMP_PRECIP_PER_MONTH = 4.0; // plus or minus
const float RANDOM_PRECIP        = 2.0; // plus or minus noise

const float AVG_TEMP = 60.0; // average
const float AMP_TEMP = 20.0; // plus or minus
const float RANDOM_TEMP = 10.0; // plus or minus noise

const float MIDTEMP   = 60.0;
const float MIDPRECIP = 14.0;

const float HAWK_HUNGER = 1.2;

unsigned int seed = 0;



namespace state {

    int now_year;  // 2023-2028
    int now_month; // 0-11

    int now_percipitation;
    int now_temp;
    int now_height;
    int now_num_rabbits;
    int now_num_hawks;

};





void rabbits();
void rye_grass();
void watcher();
void my_agent();

void update_weather();

void update_weather() {
    float ang = (  30.*(float)state::now_month + 15.  ) * ( M_PI / 180. );

    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    state::now_temp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    state::now_percipitation = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    if( state::now_percipitation < 0. ) {
        state::now_percipitation = 0.;
    }
}


void rabbits() {
    printf("rabbits\n");
    while(state::now_year < 2029 ) {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:

        // compute next now_num_rabits

        int carying_capacity = state::now_height;


        int next_num_rabbits = state::now_num_rabbits;

        float hawk_accuracy = (float)state::now_num_rabbits / (float)state::now_percipitation;
        int eaten_rabbits = state::now_num_hawks * hawk_accuracy * HAWK_HUNGER;

        next_num_rabbits -= eaten_rabbits;

        if (state::now_num_rabbits > carying_capacity) {
            next_num_rabbits -= 1;
        } else if (state::now_num_rabbits < carying_capacity) {
            next_num_rabbits += 2;
        }

        if (next_num_rabbits < 0) {
            next_num_rabbits = 0;
        }


        // DoneComputing barrier:
        #pragma omp barrier;

        // assign next value to current value

        state::now_num_rabbits = next_num_rabbits;

        // DoneAssigning barrier:
        #pragma omp barrier;

        // DonePrinting barrier:
        #pragma omp barrier;
    }

}

void my_agent() {
    printf("my_agent\n");
    while(state::now_year < 2029 ) {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:

        // compute next now_num_hawks

        int hawks = state::now_num_hawks;
        int rabbits = state::now_num_rabbits;

        float rabbits_per_hawk = (float)rabbits / (float)hawks;
        
        int next_hawks = hawks;

        if (rabbits_per_hawk > HAWK_HUNGER) {
            next_hawks += round((float)hawks * rabbits_per_hawk);
        } else {
            next_hawks -= round(1.0/rabbits_per_hawk);
        }

        if (next_hawks < 0) {
            next_hawks = 0;
        }

        if (next_hawks == 0 && rabbits > 5 * HAWK_HUNGER) {
            next_hawks = 1;
        }


        // DoneComputing barrier:
        #pragma omp barrier;

        state::now_num_hawks = next_hawks;

        // DoneAssigning barrier:
        #pragma omp barrier;

        // DonePrinting barrier:
        #pragma omp barrier;

    }
}

void rye_grass() {
    printf("rye_grass\n");

    while(state::now_year < 2029 ) {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:

        // compute next now_height

        float temp = state::now_temp;
        float precip = state::now_percipitation;

        float temp_factor = exp( -pow((temp - MIDTEMP) / 10., 2.));
        float precip_factor = exp( -pow((precip - MIDPRECIP) / 10., 2.));

        float growth = temp_factor * precip_factor * RYEGRASS_GROWS_PER_MONTH;
        float eaten = (float)state::now_num_rabbits * ONE_RABBITS_EATS_PER_MONTH;
        float next_height = state::now_height + growth - eaten;
        
        if (next_height < 0) {
            next_height = 0;
        }

        // DoneComputing barrier:
        #pragma omp barrier;

        // assign next value to current value
        state::now_height = next_height;

        // DoneAssigning barrier:
        #pragma omp barrier;

        // DonePrinting barrier:
        #pragma omp barrier;
    }
}

void watcher() {
    printf("watcher\n");

    while(state::now_year < 2029 ) {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:


        // DoneComputing barrier:
        #pragma omp barrier;


        // DoneAssigning barrier:
        #pragma omp barrier;

        // Print state
        // increment month and year
        // update weather

#ifndef CSV
        printf("Year: %d, Month: %d, Temp: %d, Percip: %d, Grass: %d, Rabbits: %d, Hawks: %d\n", 
            state::now_year,
            state::now_month,
            state::now_temp,
            state::now_percipitation,
            state::now_height,
            state::now_num_rabbits,
            state::now_num_hawks);
#else
        printf("%d,%d,%d,%d,%d,%d,%d\n", 
            state::now_year,
            state::now_month + (state::now_year - 2023) * 12,
            state::now_temp,
            state::now_percipitation,
            state::now_height,
            state::now_num_rabbits,
            state::now_num_hawks);
#endif


        state::now_month += 1;
        if (state::now_month >= 12) {
            state::now_month = 0;
            state::now_year += 1;
        }

        update_weather();

        // DonePrinting barrier:
        #pragma omp barrier;
    }
}




int main() {

    state::now_year  = 2023;
    state::now_month = 0;

    state::now_num_rabbits = 2;
    state::now_height = 5.;
    state::now_num_hawks = 2;

    update_weather();

    omp_set_num_threads(4);
    #pragma omp parallel sections 
    {
        #pragma omp section
        {
            rabbits();
        }

        #pragma omp section
        {
            rye_grass();
        }

        #pragma omp section
        {
            watcher();
        }

        #pragma omp section
        {
            my_agent();
        }

    }

    return 0;
}




