#include "binary_matrix.h"

#include <stdio.h>
#include <stdlib.h>
#define BITS_PER_BYTE 8

BinaryMatrix* ConstructBinaryMatrix(int num_rows, int num_cols) {
	if (num_rows > 0 && num_cols > 0) {
		BinaryMatrix *b = malloc(sizeof(BinaryMatrix));
		if (b != NULL) {
            b->num_cols = num_cols;
            b->num_rows = num_rows;
            b->data = NULL;
            b->data = (int*) calloc(num_rows, (num_cols / BITS_PER_BYTE) + 1);
            if (b->data == NULL) {
                printf("Array too large, failed to allocate memory.");
                DeleteBinaryMatrix(b);
                exit(-1);
            }
        }
        return b;

	} else {
		printf("Error in CreateMatrix: number of rows and columns must be positive ending with a newline and exit the program.");
		exit(-1);
	}
}

void DeleteBinaryMatrix(BinaryMatrix* M) {
	free(M->data);
	free(M);
}

void UpdateEntry(BinaryMatrix* M, int row, int col, int c) {
	if (IsMatrixIndex(M, row, col)) {
		if (c == 1) {
			setBit(M->data, row * M->num_cols + col);
		} else if (c == 0) {
			clearBit(M->data, row * M->num_cols + col);
		} else {
			printf(
					"Error in UpdateEntry: content must be 0 or 1 ending with a newline and exit the program.\n");
			exit(-1);
		}
	} else {
		printf(
				"Error in UpdateEntry: index out of bounds ending with a newline and exit the program.\n");
		exit(-1);
	}
}

int IsMatrixIndex(BinaryMatrix* M, int row, int col) {
	if (M == NULL) {
		printf(
				"IsMatrixIndex Error: NULL parameter passed ending with a newline and exit the program.\n");
		exit(-1);
	} else {
		return row < M->num_rows && col < M->num_cols && col >= 0 && row >= 0;
	}
}

int CheckEntry(BinaryMatrix* M, int row, int col) {
	if (IsMatrixIndex(M, row, col)) {
		return test(M->data, row * M->num_cols + col);
	} else {
		return 0;
	}
}

void PrintMatrix(BinaryMatrix* M) {
	int i, j, k;
	for (k = 0; k < M->num_cols; k++) {
		putchar('-');
	}
	for (i = 0; i < M->num_rows; i++) {
		putchar('\n');
		for (j = 0; j < M->num_cols; j++) {
			int x = test(M->data, i * M->num_cols + j);
			if (x) {
				putchar('#');
			} else {
				putchar(' ');
			}
		}
	}
	putchar('\n');
	for (k = j; k > 0; k--) {
		putchar('-');
	}
	putchar('\n');
}

void setBit(int* A, int k) {
	A[k / 32] |= 1 << (k % (sizeof(int) * BITS_PER_BYTE));
}

void clearBit(int* A, int k) {
	A[k / 32] &= ~(1 << (k % (sizeof(int) * BITS_PER_BYTE)));
}

int test(int* A, int k) {
	return ((A[k / 32] & (1 << (k % (sizeof(int) * BITS_PER_BYTE)))) != 0);
}
