/*
Emmanoel Dermkrdichyan - lab 5 - 4/30/20

This program calculates the page number and offset of a given address.
*/

#include <stdlib.h> //for atoi
#include <stdio.h> //for input/output

//the remainder of the address and page size is the offset
unsigned int offsetCalc(unsigned int givenAddr, int pageSize) {
	unsigned int offset = (givenAddr % pageSize);
	return offset;
}

//the address divided by the page size is the page number
unsigned int pageNumCalc(unsigned int givenAddr, int pageSize) {
	unsigned int pageNum = (givenAddr / pageSize);
	return pageNum;
}

void main(int argc, char **argv) {
	unsigned int givenAddr = 0;
	const int pageSize = 4096; //4-KB which is 4096 bytes

	//program needs only 1 parameter
	if (argc == 2) {
		givenAddr = atoi(argv[1]); //first argument is address

		printf("The address %d contains:\n", givenAddr);
		//calls the pageNumCalc function and prints the page number
		printf("page number = %d\n", pageNumCalc(givenAddr, pageSize));

		//calls the offsetCalc function and prints the offset
		printf("offset = %d\n", offsetCalc(givenAddr, pageSize));

		//finished all operations so exit successfully
		exit(EXIT_SUCCESS);
	}
	else {
		printf("error: wrong number of arguments passed in! (needs 1)\n");
		exit(EXIT_FAILURE);
	}
}