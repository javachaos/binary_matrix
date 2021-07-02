/*
 * main.c
 *
 *  Created on: Mar 2, 2016
 *      Author: fred
 *  Updated on: Jul 1, 2021
 *      Author: fred
 */
#include "../include/binary_matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <math.h>
#include <immintrin.h>
#include <string.h>

#define DRNG_NO_SUPPORT	0x0	/* For clarity */
#define DRNG_HAS_RDRAND	0x1
#define DRNG_HAS_RDSEED	0x2

typedef struct cpuid_struct {
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;
} cpuid_t;

void cpuid (cpuid_t *info, unsigned int leaf, unsigned int subleaf)
{
	asm volatile("cpuid"
	: "=a" (info->eax), "=b" (info->ebx), "=c" (info->ecx), "=d" (info->edx)
	: "a" (leaf), "c" (subleaf)
	);
}

int _is_intel_cpu ()
{
	static int intel_cpu= -1;
	cpuid_t info;

	if ( intel_cpu == -1 ) {
		cpuid(&info, 0, 0);

		if (
			memcmp((char *) &info.ebx, "Genu", 4) ||
			memcmp((char *) &info.edx, "ineI", 4) ||
			memcmp((char *) &info.ecx, "ntel", 4)
		) {
			intel_cpu= 0;
		} else {
			intel_cpu= 1;
		}
	}

	return intel_cpu;
}



int get_drng_support ()
{
	static int drng_features= -1;

	/* So we don't call cpuid multiple times for 
	 * the same information */

	if ( drng_features == -1 ) {
		drng_features= DRNG_NO_SUPPORT;

		if ( _is_intel_cpu() ) {
			cpuid_t info;

			cpuid(&info, 1, 0);

			if ( (info.ecx & 0x40000000) == 0x40000000 ) {
				drng_features|= DRNG_HAS_RDRAND;
			}

			cpuid(&info, 7, 0);

			if ( (info.ebx & 0x40000) == 0x40000 ) {
				drng_features|= DRNG_HAS_RDSEED;
			}
		} 
	}

	return drng_features;
}

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
	uint8_t rdrand_support = 0;
    if (get_drng_support ()) {
		printf ("CPU has RDRAND support, using RDRAND.\n");
		rdrand_support = 1;
	} else {
		printf ("CPU does not support RDRAND using rand() function.\n");
	}
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



    unsigned int result = 0ULL;
	//Initialize the matrix randomly.
	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			if (rdrand_support) {
                int rc = _rdrand32_step (&result);
	 	    } else {
				 result = rand();
			}
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

