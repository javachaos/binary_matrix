/*
 * main.c
 *
 *  Created on: Mar 2, 2016
 *      Author: fred
 *  Updated on: Jul 1, 2021
 *      Author: fred
 */
#include "binary_matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <math.h>
#include <immintrin.h>

static volatile int keepRunning = 1;

void intHandler(int dummy)
{
	keepRunning = 0;
}

void drawRect(BinaryMatrix *M, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	if (x > M->num_cols || y > M->num_rows 
	 || x < 0 || y < 0
	 || w > M->num_cols || h > M->num_rows 
	 || w < 0 || h < 0) {
		return;
	}

	//Draw X axis
	for (int i = x; i < w; i++) {
		UpdateEntry(M, y, i, 1);
	}
	//Draw Y axis
	for (int j = y; j < h; j++) {
		UpdateEntry(M, j, x, 1);
	}
	for (int i = x; i < w; i++) {
		UpdateEntry(M, y+h-1, i, 1);
	}
	for (int j = y; j < h; j++) {
		UpdateEntry(M, j, x+w-1, 1);
	}
    UpdateEntry(M, y+h-1, x+w-1, 1);
}

void drawGasket(BinaryMatrix *M, uint8_t x, uint8_t y, float dimension) {
	if (dimension <= 4) {
		drawRect(M, x, y, round(dimension), round(dimension));
	} else {
		float newDimension = dimension / 2;
		drawGasket(M, x, y, newDimension);
		drawGasket(M, x + newDimension, y, newDimension);
		drawGasket(M, x + newDimension, y + newDimension, newDimension);
	}
}

int main(int argc, char *argv[])
{

	signal(SIGINT, intHandler);
	if (argc != 4)
	{
	    BinaryMatrix *M = ConstructBinaryMatrix(50, 50);
		drawGasket(M,0,0,30);
	    PrintMatrix(M);
	    DeleteBinaryMatrix(M);
		printf("a.out size_x size_y iter_count   -- as ints strictly greater than 4 and less than 255 for x and y, and 65535 for iters.\n");
		exit(1);
	} else {
	
	char *a = argv[1];
	char *b = argv[2];
	char *c = argv[3];
	uint8_t size_x = atoi(a);
	uint8_t size_y = atoi(b);
	int iter = atoi(c);
	if (size_x < 4 || size_y < 4 || size_x > UINT8_MAX || size_y > UINT8_MAX || iter < 0 || iter > UINT16_MAX)
	{
		printf("Input out of bounds.\n");
		exit(1);
	}

	struct timespec sleeptime;
	sleeptime.tv_sec = 0;
	sleeptime.tv_nsec = 50000000;

	BinaryMatrix *M = ConstructBinaryMatrix(size_x, size_y);

	int x, y, z = 0;

	int width = M->num_cols - 1;
	int height = M->num_rows - 1;


    unsigned long long result = 0ULL;
	//Initialize the matrix randomly.
	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{

            int rc = _rdrand64_step (&result);
			int v = (result % 100) > 90 ? 1 : 0;
			if (v)
			{
				UpdateEntry(M, y, x, v);
			}
		}
	}

	int n = 0;
	while (z++ < iter && keepRunning)
	{
		n = 0; //number of mutations per iteration
		for (x = 0; x < height; x++)
		{
			for (y = 0; y < width; y++)
			{
				int k = 0;
				k += CheckEntry(M, x - 1, y - 1);
				k += CheckEntry(M, x, y - 1);
				k += CheckEntry(M, x + 1, y - 1);
				k += CheckEntry(M, x - 1, y);
				k += CheckEntry(M, x + 1, y);
				k += CheckEntry(M, x - 1, y + 1);
				k += CheckEntry(M, x, y + 1);
				k += CheckEntry(M, x + 1, y + 1);
				if (CheckEntry(M, x, y))
				{
					if (k < 2 || k > 3)
					{
						UpdateEntry(M, x, y, 0);
						n++;
					}
					else if (k == 2 || k == 3)
					{
						UpdateEntry(M, x, y, 1);
					}
				}
				else
				{
					if (k == 3)
					{
						UpdateEntry(M, x, y, 1);
						n++;
					}
					else
					{
						UpdateEntry(M, x, y, 0);
					}
				}
			}
		}
		if (n == 0)
		{
			break;
		}
		PrintMatrix(M);
		nanosleep(&sleeptime, NULL);
		printf("\nNumber of mutations: %d\n", n);
	}
	PrintMatrix(M);
	DeleteBinaryMatrix(M);
	printf("\nTotal number of iterations run: %d\n", z - 1);

	return 0;
	}
}

