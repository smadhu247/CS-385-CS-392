/*******************************************************************************
 * Name        : mtsieve.c
 * Author      : Sanjana Madhu and Lasya Josyula
 * Date        : 04/23/2021
 * Description :
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <getopt.h>
#include <unistd.h>
#include <math.h>
#include <sys/sysinfo.h>

typedef struct arg_struct {
    int start;
    int end;
} thread_args;

int total_count = 0;
pthread_mutex_t lock;

bool is_integer(char *input) {    
    int start = 0, len = strlen(input);    
    if (len >= 1 && input[0] == '-') {        
        if (len < 2) {            
            return false;        
        }        
        start = 1;    
    }
    for (int i = start; i < len; i++) {        
        if (!isdigit(input[i])) {            
            return false;        
        }    
    }    
    return true;
}

bool get_integer(char *input, int *value) {    
    long long long_long_i;    
    if (sscanf(input, "%lld", &long_long_i) != 1) {        
        return false;    
    }   
    *value = (int)long_long_i;    
    if (long_long_i != (long long)*value) {        
       // fprintf(stderr, "Warning: Integer overflow with '%s'.\n", input);        
        return false;   
    }    
    return true;
}

/*
Purpose: Used to find primes in range [a, b].
Steps:
    1. Use standard sieve of Eratosthenes to find all primes up to and including sqrt(b), call them low_primes[]
    2. Create a boolean array high_primes[] with length = b - a + 1 and initialize each element to true
    3. for each prime p in low_primes[],
        set i = ceil((double)a/p) * p - a
        if a <= p
        set i = i + p
        starting at i, cross off all multiples of p in high_primes[]
    4. for each high_primes[i] that is true, print i + a
*/
void *sieve (void *ptr){
    thread_args* pt = (thread_args *) ptr;
    int limit = sqrt(pt->end);   
    int s = pt->start;
    int e = pt->end;
    int three_counts;

    bool low_primes[limit + 1];
    low_primes[0] = false;
	low_primes[1] = false;

    for (int i = 2; i <= limit; i++) {
        low_primes[i] = true;
    }

    for(int i = 2; i <= limit; i++){
		if (low_primes[i] == true){
			int newi = i*i; //created to ensure that all multiples of i are set to false
			for (int j = newi; j <= limit; j += i) {
				low_primes[j] = false;
			}
		}
	}

    int high_limit = e - s;
    bool high_primes[high_limit + 1];

    for (int i = 0; i <= high_limit; i++) {
        high_primes[i] = true;
    }

    for (int p = 2; p <= limit; p++) {
        int i;
        if (low_primes[p] == true) {
            i = ceil((double) s / p) * p - s;
            if (s <= p) {
                i = i + p; 
            }
            for (int j = i; j <= high_limit; j += p) {
                high_primes[j] = false;
            }
        }
    }

    for (int i = 0; i <= high_limit; i++) {
        if (high_primes[i] == true) {
            int val = i + s;
            printf("%d\n", val);
            //check for numbers liek 303 333 that are prime
            // if found, increment three_counts
        }
    }

    int retval;
    if ((retval = pthread_mutex_lock(&lock)) != 0) {
        fprintf(stderr, "Warning: Cannot lock mutex. %s.\n", strerror(retval));
    }
    total_count = total_count + three_counts;
    if ((retval = pthread_mutex_unlock(&lock)) != 0) {
        fprintf(stderr, "Warning: Cannot unlock mutex. %s.\n", strerror(retval));
    }
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {

    //Error Checking
    if(argc == 1) {
        printf("Usage: ./mtsieve -s <starting value> -e <ending value> -t <num threads>\n");
        return EXIT_FAILURE;
    }

    extern char *optarg;
    extern int optind, opterr, optopt;
    int opt;
    int s = 0;
    int e = 0;
    int t = 0;
    int num;
    int num_s = 0;
    int num_e = 0;
    int num_t = 0;
    opterr = 0;
    while ((opt = getopt(argc, argv, "s:e:t:")) != -1) {
          switch (opt){
            case 's':
                // this error does not work
                if (optarg == NULL) {
                    printf("Error: Option -s requires an argument.\n");
                    return EXIT_FAILURE;
                }
                if (!is_integer(optarg)){
                    printf("Error: Invalid input '%s' received for parameter '-%c'.\n", optarg, 's');
                    return EXIT_FAILURE;
                }  
                if (!get_integer(optarg, &num)){
                    printf("Error: Integer overflow for parameter '-%c'.\n", 's');
                    return EXIT_FAILURE;
                }
                s = atoi(optarg);
                num_s = 1;
                break;
            case 'e':
                // this error does not work
                if (optarg == NULL) {
                    printf("Error: Option -e requires an argument.\n");
                    return EXIT_FAILURE;
                }
                if (!is_integer(optarg)){
                    printf("Error: Invalid input '%s' received for parameter '-%c'.\n", optarg, 'e');
                    return EXIT_FAILURE;
                }
                if (!get_integer(optarg, &num)){
                    printf("Error: Integer overflow for parameter '-%c'.\n", 'e');
                    return EXIT_FAILURE;
                }
                e = atoi(optarg);
                num_e = 1;
                break;
            case 't':
                // this error does not work
                if (optarg == NULL) {
                    printf("Error: Option -t requires an argument.\n");
                    return EXIT_FAILURE;
                }
                if (!is_integer(optarg)){
                    printf("Error: Invalid input '%s' received for parameter '-%c'.\n", optarg, 't');
                    return EXIT_FAILURE;
                }
                if (!get_integer(optarg, &num)){
                    printf("Error: Integer overflow for parameter '-%c'.\n", 't');
                    return EXIT_FAILURE;
                }
                t = atoi(optarg);
                num_t = 1;
                break;
            case '?':
                if (optopt == 'e' || optopt == 's' || optopt == 't') {
                    fprintf(stderr, "Error: Option -%c requires an argument.\n", optopt);
                } 
                else if (isprint(optopt)) {
                    fprintf(stderr, "Error: Unknown option '-%c'.\n", optopt);
                } 
                else {
                    fprintf(stderr, "Error: Unknown option character '\\x%x'.\n", optopt);
                }
                return EXIT_FAILURE;
          }
     }

    if (argv[optind] != NULL){
        printf("Error: Non-option argument '%s' supplied.\n", argv[optind]);
        return EXIT_FAILURE;
    }
    if (num_s == 0){
        printf("Error: Required argument <starting value> is missing.\n");
        return EXIT_FAILURE;
    }
    if (s < 2) {
        printf("Error: Starting value must be >= 2.\n");
        return EXIT_FAILURE;
    }
    if (num_e == 0){
        printf("Error: Required argument <ending value> is missing.\n");
        return EXIT_FAILURE;
    }
    if (e < 2) {
        printf("Error: Starting value must be >= 2.\n");
        return EXIT_FAILURE;
    }
    if (s > e) {
        printf("Error: Ending value must be >= starting value.\n");
        return EXIT_FAILURE;
    }
    if (num_t == 0){
        printf("Error: Required argument <num threads> is missing.\n");
        return EXIT_FAILURE;
    }
    if (t < 1){
        printf("Error: Number of threads cannot be less than 1.\n");
        return EXIT_FAILURE;
    }
    if (t > (2 * get_nprocs())){
        printf("Error: Number of threads cannot exceed twice the number of processors(%d).\n", get_nprocs());
        return EXIT_FAILURE;
    }

    //Creating Segments
    int count = e - s + 1; 
    int rem; 
    if (t > count) {
        t = count;
    }
    rem = count % t;
    count = count / t;

    //Threads
    int retval;
    if ((retval = pthread_mutex_init(&lock, NULL)) != 0) {
        fprintf(stderr, "Error: Cannot create mutex. %s.\n", strerror(retval));
        return EXIT_FAILURE;
    }

    //Print segments
    printf("Finding all prime numbers between %d and %d.\n", s, e);

    if (t == 1){
        printf("%d segment:\n", t);
    } 
    if (t > 1){
        printf("%d segments:\n", t);
    }


    pthread_t threads[t];
    thread_args targs[t];

    int start = s;

    for (int i = 0; i < t; i++) {
        targs[i].start = start;
        if (i < rem) {
            targs[i].end = start + count;
        }
        else {
            targs[i].end = start + count - 1;
        }
        start = targs[i].end + 1;
        printf("[%d, %d]\n", targs[i].start, targs[i].end);
        int thread_val = i + 1;
        if ((retval = pthread_create(&threads[i], NULL, sieve, (void *) &targs[i])) != 0) {
            fprintf(stderr, "Error: Cannot create thread %d. %s.\n", thread_val, strerror(retval));
            break;
        }
    }

    for (int i = 0; i < t; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Warning: Thread %d did not join properly.\n",i + 1);
        }
    }

    if ((retval = pthread_mutex_destroy(&lock)) != 0) {
        fprintf(stderr, "Error: Cannot destroy mutex. %s.\n", strerror(retval));
        return EXIT_FAILURE;
    }

    printf("Total primes between %d and %d with two or more '3' digits: %d\n", s, e, total_count);

    return EXIT_SUCCESS;
}
