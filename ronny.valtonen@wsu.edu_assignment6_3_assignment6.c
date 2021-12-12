/***********************************************************************
name: Ronny Z Valtonen
    Dining PHILO Problem
description:
    See CS 360 Lecture for details.

README:
    gcc -Wall -o [NAME] assignment6.c -lm -lpthread
    ./assignment6
***********************************************************************/

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>


// Ignore useless warning
_Pragma("GCC diagnostic ignored \"-Wint-to-pointer-cast\"")

// Defines
#define PHILO 5

// Globals
pthread_mutex_t philosopher_mtx[PHILO];

/* Definitions */

/*********************************************************************

Parameters:
int mean = eating will be 9 and thinking will be 11 seconds.

int stddev = eating will be 3 and thinking will be 7 seconds.

General Run:
Successive calls to randomGaussian produce integer return values
having a gaussian distribution with the given mean and standard
deviation.  Return values may be negative.
// Writen by Ben Mccamish at Washington State University Washington.
********************************************************************/
int randomGaussian(int mean, int stddev);

/*********************************************************************

Parameters:
int chop = The chopstick

General Run:
This is the pointer for the chopstick mutexes. lock and unlock the
mutexes accordingly. Print out when the philosopher is eatings
and thinking and the amount of time it is doing so. Sleep (think/eat)
according to the randomGaussian function. Finally, if the philosopher
as eaten for 100 seconds, say that it left the table and break it away
from the function. When all philosophers have finished, end the
function.
********************************************************************/
void *consumer(int chop);

/*********************************************************************


Parameters:
Does not take any inputs from user.

General Run:
Loop through, init all threads, loop through, creating threads, Loop
through, joining threads.
********************************************************************/
int main(int argc, char *argv[]);




											// Main //

// Random number function
int randomGaussian(int mean, int stddev) {
    double mu = 0.5 + (double) mean;
    double sigma = fabs((double) stddev);
    double f1 = sqrt(-2.0 * log((double) rand() / (double) RAND_MAX));
    double f2 = 2.0 * 3.14159265359 * (double) rand() / (double) RAND_MAX;

    if (rand() & (1 << 5)) {
    	return (int) floor(mu + sigma * cos(f2) * f1);
  }
    else {
    	return (int) floor(mu + sigma * sin(f2) * f1);
  }
}

// Chopstick Function
void *consumer(int chop) {
	// Setup how long each philosopher eats to keep track of them.
	static int eatTime = 0;
	// Setup the thinking time.
	static int thinkingTime;
	// Setup the eating time.
	static int eatingTime;

	// Keep looping as long as all philosophers are still eating.
	while (1) {
		// The thinking time takes in 11 and 7.
		thinkingTime = randomGaussian(11, 7);
		// It's a negative number, assume it's zero.
		if (thinkingTime <= 0) {
			thinkingTime = 0;
		}
		// The thinking time takes in 9, and 3.
		eatingTime = randomGaussian(9, 3);
		// It's a negative number, assume it's zero.
		if (eatingTime <= 0) {
			eatingTime = 0;
		}
			// Print the philosopher number, how long it is thinking for, and eating time.
	   printf ("Philosopher %d is thinking for %d seconds (%d)\n", chop, thinkingTime, eatTime);
		 // Sleep for the randomGaussian function.
		 sleep(thinkingTime);

		 // Try to lock the mutex..
	   pthread_mutex_trylock(&philosopher_mtx[chop]);
	   pthread_mutex_trylock(&philosopher_mtx[(chop+1) % PHILO]);

	   printf ("Philosopher %d is eating for %d seconds (%d)\n", chop, eatingTime, eatTime);

		 // Sleep for the randomGaussian function.
	   sleep(eatingTime);

		 // Unlock Chopstick
	   pthread_mutex_unlock(&philosopher_mtx[chop]);
		 // Error check that you didn't already unlock it.
		 if (pthread_mutex_unlock(&philosopher_mtx[chop]) != 0) {
			 exit(EXIT_FAILURE);
		 }

	   pthread_mutex_unlock(&philosopher_mtx[(chop+1) % PHILO]);
		 // Error check that you didn't already unlock it.
		 if (pthread_mutex_unlock(&philosopher_mtx[(chop+1) % PHILO]) != 0) {
			 exit(EXIT_FAILURE);
		 }

		 if (eatTime >= 100) {
			 printf ("Philosopher %d has left the table.\n", chop);
			 break;
		 }
		 eatTime += eatingTime;
 	}
	return NULL;
}

// Main Function
int main(int argc, char *argv[]) {
	pthread_t philosopher[PHILO];
	static int init = 0;
	static int create = 0;
	static int join = 0;

  // Initialize the threads for each philosopher
	while (init < PHILO) {
		pthread_mutex_init(&philosopher_mtx[init], NULL);

    // Init, returns 0 on success
    if (pthread_mutex_init(&philosopher_mtx[init], NULL) != 0) {
      exit(EXIT_FAILURE);
    } else init++;
	}

  // Create the threads for each philosopher
	while (create < PHILO) {
		pthread_create(&philosopher[create], NULL, (void *)consumer, (void *)create);

    // Create returns 0 on success
    if (pthread_create(&philosopher[create], NULL, (void *)consumer, (void *)create) != 0) {
      exit(EXIT_FAILURE);
    } else create++;
	}

  // Join the threads for each philosopher
	while (join < PHILO) {
		pthread_join(philosopher[join], NULL);

    // Join returns 0 on success
    if (pthread_join(philosopher[join], NULL) != 0) {
      exit(EXIT_FAILURE);
    } else join++;
	}

  return 0;
}

	 // End of Function //
