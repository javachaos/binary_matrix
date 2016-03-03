/*
 * main.c
 *
 *  Created on: Mar 2, 2016
 *      Author: fred
 */

#include "binary_matrix.h"
#include <stdio.h>
#include <stdlib.h>
#define NUM_EPOCHS 500

int main(void) {

	BinaryMatrix* M = ConstructBinaryMatrix(40,80);

	int x, y, z=0;

  	UpdateEntry(M, 1, 1, 1);
  	UpdateEntry(M, 2, 2, 1);
  	UpdateEntry(M, 2, 1, 1);
  	UpdateEntry(M, 3, 1, 1);
  	UpdateEntry(M, 2, 0, 1);
  	UpdateEntry(M, 3, 3, 1);
  	UpdateEntry(M, 4, 4, 1);
  	UpdateEntry(M, 2, 3, 1);
  	UpdateEntry(M, 0, 0, 1);
  	UpdateEntry(M, 0, 3, 1);
  	UpdateEntry(M, 4, 3, 1);
  	UpdateEntry(M, 0, 2, 1);
  	UpdateEntry(M, 2, 4, 1);

  	while(z++ < NUM_EPOCHS) {
		for(y = 0; y < M->num_cols; y++) {
			for(x = 0; x < M->num_rows; x++) {
				int k = 0;
				k+=CheckEntry(M,x-1,y-1);
				k+=CheckEntry(M,x,y-1);
				k+=CheckEntry(M,x+1,y-1);
				k+=CheckEntry(M,x-1,y);
				k+=CheckEntry(M,x+1,y);
				k+=CheckEntry(M,x-1,y+1);
				k+=CheckEntry(M,x,y+1);
				k+=CheckEntry(M,x+1,y+1);
				if(CheckEntry(M,x,y)) {
					if(k < 2 || k > 3) {
				        UpdateEntry(M,x,y,0);
					}
				} else {
					if(k == 3) {
				        UpdateEntry(M,x,y,1);
					}
				}
			}
		}

		PrintMatrix(M);
		usleep(100000);
	}

	DeleteBinaryMatrix(M);

	return 0;
}

