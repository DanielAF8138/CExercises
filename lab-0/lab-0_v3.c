/*
 *Assignment: Lab 0.
 *Date: 1/17/24
 *Author: Daniel Fuller fulle2da@dukes.jmu.edu
 *File Name: lab-0_v3.c
 *
 *
 * CS 261 Example
 *
 * fopen / printf / fscanf / Intro to Scope of Variables
 *
 * Building command:     gcc lab0.c  -o lab0
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *dataFp ;
	unsigned char start , count ;
	int i = 640, counter = 0 ;
	
	dataFp = fopen ("testData.txt" , "r" ) ;
	if ( dataFp == NULL )  // Did it fail ?
	{
		printf("ERROR:  could not open the file\n");
        exit( EXIT_FAILURE );
	}
    
	fscanf( dataFp , "%hhx %hhd" , &start , &count );
	printf( "Will count starting at HEX %X  for %hhd iterations\n" , start , count );

	for (int  i=0 ; i< count ; i++ ) {
		printf( "  %02hhX" , start+i ) ;
		counter++;
		if (counter == 8) {
    		printf("\n");
			counter = 0;
  		}
	}	
	
	printf("\nFinal value of 'i' = %d\n" , i ) ;
	
	return 0;
}
