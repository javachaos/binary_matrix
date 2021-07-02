/*
 * binary_matrix.h
 * Name / StudentID
 * An implementation of a Binary Matrix
 */
#ifndef _BINARY_MATRIX_H_
#define _BINARY_MATRIX_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define BITS_PER_BYTE 8

typedef struct
{
	uint8_t num_rows;
	uint8_t num_cols;
	int *data;
} BinaryMatrix;

/**
 * Allocate a bit matrix in the least amount of storage.
 */
BinaryMatrix *ConstructBinaryMatrix(uint8_t num_rows, uint8_t num_cols);

/**
 * Free the memory allocated to store the bit matrix.
 */
void DeleteBinaryMatrix(BinaryMatrix *M);

/**
 * Ensure the index of row,col is a matrix index.
 */
int IsMatrixIndex(BinaryMatrix *M, uint8_t row, uint8_t col);

/**
 * Update an entry with value c.
 */
void UpdateEntry(BinaryMatrix *M, uint8_t row, uint8_t col, uint8_t c);

/**
 * Check the value of an entry.
 */
int CheckEntry(BinaryMatrix *M, uint8_t row, uint8_t col);

/**
 * Check if two binary matricies are identical bit for bit
 */
int CheckEqual(BinaryMatrix *M, BinaryMatrix *N);

/**
 * Deep copy a BinaryMatrix
 */
BinaryMatrix *clone(BinaryMatrix *M);

/**
 * Print the matrix
 */
void PrintMatrix(BinaryMatrix *M);

#endif /* _BINARY_MATRIX_H_ */
/**
 * Set Kth binary bit in A to 1.
 */
void setBit(int A[], int k);

/**
 * Set Kth binary bit in A to 0.
 */
void clearBit(int A[], int k);

/**
 * Test if Kth binary bit is 1 or 0.
 */
int test(int A[], int k);
