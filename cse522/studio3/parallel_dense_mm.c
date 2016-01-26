/******************************************************************************
* 
* dense_mm.c
* 
* This program implements a dense matrix multiply and can be used as a
* hypothetical workload. 
*
* Usage: This program takes a single input describing the size of the matrices
*        to multiply. For an input of size N, it computes A*B = C where each
*        of A, B, and C are matrices of size N*N. Matrices A and B are filled
*        with random values. 
*
* Written Sept 6, 2015 by David Ferry
******************************************************************************/

#include <stdio.h>  //For printf()
#include <stdlib.h> //For exit() and atoi()
#include <assert.h> //For assert()
#include <time.h>

const int num_expected_args = 3;
const unsigned sqrt_of_UINT32_MAX = 65536;

// The following line can be used to verify that the parallel computation
// gives identical results to the serial computation. If the verficiation is
// successful then the program executes normally. If the verification fails
// the program will terminate with an assertion error.
//#define VERIFY_PARALLEL

typedef unsigned long long U64;

U64 convert( struct timespec timeSpec )
{
	return (timeSpec.tv_sec * 1000000000) + timeSpec.tv_nsec;
}

int main( int argc, char* argv[] ){

	unsigned index, row, col, i; //loop indicies
	unsigned matrix_size, squared_size;
	double *A, *B, *C;
	struct timespec start;
	struct timespec end;
	unsigned int numLoops = 0,
	U64  average = 0, min = 0, max = 0, ;

	#ifdef VERIFY_PARALLEL
	double *D;
	#endif

	if( argc != num_expected_args ){
		printf("Usage: ./dense_mm <size of matrices> <number of loops>\n");
		exit(-1);
	}

	matrix_size = atoi(argv[1]);
	numLoops = atoi(argv[2]);
	
	if( matrix_size > sqrt_of_UINT32_MAX ){
		printf("ERROR: Matrix size must be between zero and 65536!\n");
		exit(-1);
	}

	squared_size = matrix_size * matrix_size;

	printf("Generating matrices...\n");

	A = (double*) malloc( sizeof(double) * squared_size );
	B = (double*) malloc( sizeof(double) * squared_size );
	C = (double*) malloc( sizeof(double) * squared_size );
	#ifdef VERIFY_PARALLEL
	D = (double*) malloc( sizeof(double) * squared_size );
	#endif

	for( index = 0; index < squared_size; index++ ){
		A[index] = (double) rand();
		B[index] = (double) rand();
		C[index] = 0.0;
		#ifdef VERIFY_PARALLEL
		D[index] = 0.0;
		#endif
	}

	for( i = 0; i < numLoops; i++ )
	{

		printf("Multiplying matrices...\n");
		clock_gettime( CLOCK_MONOTONIC_RAW, &start );
		#pragma omp parallel for private(col, row, index)
		for( col = 0; col < matrix_size; col++ ){
			for( row = 0; row < matrix_size; row++ ){
				for( index = 0; index < matrix_size; index++){
				C[row*matrix_size + col] += A[row*matrix_size + index] *B[index*matrix_size + col];
				}	
			}
		}
		clock_gettime( CLOCK_MONOTONIC_RAW, &end );

		convert( start );
 		convert( end );

	}

	
	#ifdef VERIFY_PARALLEL
	printf("Verifying parallel matrix multiplication...\n");
	for( col = 0; col < matrix_size; col++ ){
		for( row = 0; row < matrix_size; row++ ){
			for( index = 0; index < matrix_size; index++){
			D[row*matrix_size + col] += A[row*matrix_size + index] *B[index*matrix_size + col];
			}	
		}
	}

	for( index = 0; index < squared_size; index++ ) 
		assert( C[index] == D[index] );
	#endif //ifdef VERIFY_PARALLEL

	printf("Multiplication done!\n");

	return 0;
}
