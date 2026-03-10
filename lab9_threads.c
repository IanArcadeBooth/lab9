/*
* File: lab09_threads.c
* Author: Ian Booth
* Date: 2026/03/10
* Description: This program demonstrates multithreading using
* POSIX threads. Three threads are created to calculate the
* rows of a matrix multiplication. Each thread calculates
* one row of the result vector and stores it in a shared
* array protected by a mutex.
*/

#include <stdio.h>
#include <pthread.h>

#define SIZE 3

/* matrix A */
int matrixA[SIZE][SIZE] = {
    {1, 2, 3},
    {0, -1, 1},
    {3, 2, 1}
};

/* matrix B (vector) */
int matrixB[SIZE] = {1, 3, 2};

/* result vector */
int result[SIZE];

/* mutex for shared data */
pthread_mutex_t mutex;

/*
* Description: Calculates one row of the matrix multiplication.
* @param arg: pointer to the row number
* @return: returns NULL when thread finishes
* side effects: writes result to shared result array
*/
void* multiply_row(void* arg)
{
    int row = *(int*)arg;
    int k;
    int sum = 0;

    for(k = 0; k < SIZE; k++)
    {
        sum += matrixA[row][k] * matrixB[k];
    }

    pthread_mutex_lock(&mutex);
    result[row] = sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

/*
* Description: Main program that creates three threads to
* perform matrix multiplication and prints the final result.
* @param none
* @return: returns 0 when program completes
* side effects: creates and joins POSIX threads
*/
int main(void)
{
    pthread_t threads[SIZE];
    int rows[SIZE] = {0,1,2};
    int i;

    pthread_mutex_init(&mutex, NULL);

    /* create threads */
    for(i = 0; i < SIZE; i++)
    {
        pthread_create(&threads[i], NULL, multiply_row, &rows[i]);
    }

    /* wait for threads */
    for(i = 0; i < SIZE; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Result Vector:\n");

    for(i = 0; i < SIZE; i++)
    {
        printf("%d\n", result[i]);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
