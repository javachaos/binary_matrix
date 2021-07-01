/*
 * main.c
 *
 *  Created on: Mar 2, 2016
 *      Author: fred
 */
#include "binary_matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("a.out size_x size_y iter_count   -- as ints strictly less than 500 for x and y, and 50000 for iters.\n");
	    exit(1);
    }
    char *a = argv[1];
    char *b = argv[2];
    char *c = argv[3];
    int size_x = atoi(a);
    int size_y = atoi(b);
    int iter = atoi(c);
    if (size_x <= 0 || size_y <= 0 
    || size_x > 500 || size_y > 500
    || iter < 0     || iter > 50000) {
	    printf("Input out of bounds.\n");
 	    exit(1);
    }

    struct timespec sleeptime;
    sleeptime.tv_sec = 0;
    sleeptime.tv_nsec = 50000000;
    time_t t;

	BinaryMatrix* M = ConstructBinaryMatrix(size_x, size_y);

	int x, y, z = 0;

	int width = M->num_cols - 1;
	int height = M->num_rows - 1;
	srand((unsigned) time(&t));

    //Initialize the matrix randomly.
    for (x = 0; x < width; x++) {
      for (y = 0; y < height; y++) {
         int v = (rand() % 100) > 90 ? 1 : 0;
		 if (v) {
	         UpdateEntry(M, y, x, v);
		 }
      }
    }

	int n = 0;
	while (z++ < iter) {
		n = 0;//number of mutations per iteration
		for (x = 0; x < height; x++) {
			for (y = 0; y < width; y++) {
				int k = 0;
				k += CheckEntry(M, x - 1, y - 1);
				k += CheckEntry(M, x, y - 1);
				k += CheckEntry(M, x + 1, y - 1);
				k += CheckEntry(M, x - 1, y);
				k += CheckEntry(M, x + 1, y);
				k += CheckEntry(M, x - 1, y + 1);
				k += CheckEntry(M, x, y + 1);
				k += CheckEntry(M, x + 1, y + 1);
				if (CheckEntry(M, x, y)) {
					if (k < 2 || k > 3) {
						UpdateEntry(M, x, y, 0);
						n++;
					} else if (k == 2 || k == 3) {
						UpdateEntry(M, x, y, 1);
					}
				} else {
					if (k == 3) {
						UpdateEntry(M, x, y, 1);
						n++;
					} else {
						UpdateEntry(M, x, y, 0);
					}
				}
			}
		}
		if (n == 0) {
			break;
		}
		PrintMatrix(M);
		nanosleep(&sleeptime, NULL);
		printf("\nNumber of mutations: %d\n", n);
	}
	DeleteBinaryMatrix(M);
	printf("\nTotal number of iterations run: %d\n", z-1);

	return 0;
}

