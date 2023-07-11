# entropius by @rkdnapps on GitHub 07/11/2023
#
# Started as a pandemic project to test the speed of finding prime factors from
# random input.  Picked it back up three years later to see what the free
# version of ChatGPT (https://chat.openai.com/) could do in terms of improving
# the basic code.  Added support for multi-threading, splitting the factoring
# evenly across all available threads, and variable bit lengths for the input.  
# Makes a good burn-in test for new builds.  Finding large prime factors from 
# random input should get much faster as CPU core counts move into the 1000s.
# OpenCL version for GPU testing might be a future project.
#

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <gmp.h>
#include <time.h>

typedef struct {
    mpz_t n;
    mpz_t start;
    mpz_t end;
    mpz_t largestFactor;
} ThreadData;

void* findLargestFactor(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    mpz_t n, start, end, i, factor, largestFactor;
    mpz_init_set(n, data->n);
    mpz_init_set(start, data->start);
    mpz_init_set(end, data->end);
    mpz_init_set(i, start);
    mpz_init(factor);
    mpz_init_set_ui(largestFactor, 0);

    while (mpz_cmp(i, end) <= 0 && mpz_cmp(i, n) <= 0) {
        if (mpz_divisible_p(n, i)) {
            mpz_set(factor, i);
            mpz_divexact(n, n, i);
            if (mpz_probab_prime_p(factor, 25)) {
                if (mpz_cmp(factor, largestFactor) > 0) {
                    mpz_set(largestFactor, factor);
                    gmp_printf("Thread %lu: Largest prime factor so far: %Zd\n", pthread_self(), largestFactor);
                }
            }
        }
        mpz_add_ui(i, i, 1);
    }

    mpz_set(data->largestFactor, largestFactor);

    mpz_clear(n);
    mpz_clear(start);
    mpz_clear(end);
    mpz_clear(i);
    mpz_clear(factor);
    mpz_clear(largestFactor);

    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <number of bits> <number of threads>\n", argv[0]);
        printf("Example: %s 32 32\n", argv[0]);
        return 1;
    }

    int numThreads = atoi(argv[1]);
    int numBits = atoi(argv[2]);

    // Generate a random seed based on current time
    unsigned long seed = time(NULL);
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);

    // Generate a random number from the number of bits specificed on the command line
    mpz_t n;
    mpz_init(n);
    mpz_urandomb(n, state, numBits);

    printf("Input number: ");
    mpz_out_str(stdout, 10, n);
    printf("\n");

    // Divide the work among the threads
    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];

    mpz_t rangeSize, factorRangeSize;
    mpz_init(rangeSize);
    mpz_init(factorRangeSize);
    mpz_fdiv_q_ui(rangeSize, n, numThreads);
    mpz_fdiv_q_ui(factorRangeSize, rangeSize, numThreads);

    for (int i = 0; i < numThreads; i++) {
        mpz_init(threadData[i].n);
        mpz_init(threadData[i].start);
        mpz_init(threadData[i].end);
        mpz_init(threadData[i].largestFactor);

        mpz_set(threadData[i].n, n);
        mpz_mul_ui(threadData[i].start, rangeSize, i);
        mpz_mul_ui(threadData[i].end, rangeSize, i + 1);
        mpz_add(threadData[i].end, threadData[i].end, factorRangeSize);

        pthread_create(&threads[i], NULL, findLargestFactor, &threadData[i]);
    }

    // Wait for the threads to complete
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Find the largest factor among all threads
    mpz_t largestFactor;
    mpz_init_set_ui(largestFactor, 0);

    for (int i = 0; i < numThreads; i++) {
        if (mpz_cmp(threadData[i].largestFactor, largestFactor) > 0) {
            mpz_set(largestFactor, threadData[i].largestFactor);
        }
    }

    printf("Largest prime factor: ");
    mpz_out_str(stdout, 10, largestFactor);
    printf("\n");

    // Clean up
    mpz_clear(n);
    mpz_clear(rangeSize);
    mpz_clear(factorRangeSize);
    mpz_clear(largestFactor);
    gmp_randclear(state);

    for (int i = 0; i < numThreads; i++) {
        mpz_clear(threadData[i].n);
        mpz_clear(threadData[i].start);
        mpz_clear(threadData[i].end);
        mpz_clear(threadData[i].largestFactor);
    }

    return 0;
}
