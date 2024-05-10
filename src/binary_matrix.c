#include "binary_matrix.h"

BinaryMatrix *ConstructBinaryMatrix(uint16_t rows, uint16_t cols)
{
	if (rows > 0 && cols > 0)
	{
		BinaryMatrix *b = malloc(sizeof(BinaryMatrix));
		if (b != NULL)
		{
			b->num_cols = cols;
			b->num_rows = rows;
			b->data = NULL;
			b->data = (uint32_t *)calloc(rows, (cols / BITS_PER_BYTE) + 1);
			if (b->data == NULL)
			{
				printf("Array too large, failed to allocate memory.");
				DeleteBinaryMatrix(b);
				exit(-1);
			}
		}
		return b;
	}
	else
	{
		printf("Error in CreateMatrix: number of rows and columns must be positive ending with a newline and exiting the program.");
		exit(-1);
	}
}

void DeleteBinaryMatrix(BinaryMatrix *M)
{
	free(M->data);
	free(M);
}

void UpdateEntry(BinaryMatrix *M, uint16_t row, uint16_t col, uint8_t c)
{
	if (IsMatrixIndex(M, row, col))
	{
		if (c == 1)
		{
			setBit(M->data, row * M->num_cols + col);
		}
		else if (c == 0)
		{
			clearBit(M->data, row * M->num_cols + col);
		}
		else
		{
			printf(
				"Error in UpdateEntry: content must be 0 or 1 ending with a newline and exiting the program.\n");
			if (M != NULL) {
			    DeleteBinaryMatrix(M);
		    }
			exit(-1);
		}
	}
	else
	{
		printf(
			"Error in UpdateEntry: index out of bounds ending with a newline and exiting the program.\n");
		if (M != NULL) {
			DeleteBinaryMatrix(M);
		}
		exit(-1);
	}
}

int IsMatrixIndex(BinaryMatrix *M, uint16_t row, uint16_t col)
{
	return (M != NULL)
	    && (row < M->num_rows)
	    && (col < M->num_cols)
		&& (col >= 0)
		&& (row >= 0);
}

int CheckEntry(BinaryMatrix *M, uint16_t row, uint16_t col)
{
	if (IsMatrixIndex(M, row, col))
	{
		return test(M->data, row * M->num_cols + col);
	}
	else
	{
		return 0;
	}
}

void PrintMatrix(BinaryMatrix *M)
{
	int i, j, k;
	//Draw first line for this snapshot
	for (k = 0; k < M->num_cols; k++)
	{
		putchar('-');
	}
	for (i = 0; i < M->num_rows; i++)
	{
		putchar('\n');
		for (j = 0; j < M->num_cols; j++)
		{
			int x = test(M->data, i * M->num_cols + j);
			if (x)
			{
				putchar('#');
			}
			else
			{
				putchar(' ');
			}
		}
	}
	putchar('\n');
	//Draw the bottom line for this snapshot
	for (k = j; k > 0; k--)
	{
		putchar('-');
	}
	putchar('\n');
}

void setBit(uint32_t *A, uint32_t k)
{
	A[k / BITS_PER_INT] |= 1 << (k % (BITS_PER_INT));
}

void clearBit(uint32_t *A, uint32_t k)
{
	A[k / BITS_PER_INT] &= ~(1 << (k % (BITS_PER_INT)));
}

int test(uint32_t *A, uint32_t k)
{
	return ((A[k / BITS_PER_INT] & (1 << (k % (BITS_PER_INT)))) != 0);
}
