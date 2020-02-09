#include<stdio.h>

int main (int argc, char* argv[]){
	int counter;
	printf(Program name is: %s",argv[0];
	if(argc == 1){
		printf("\nNo Extra Command Line");
	}
	if(argc >= 2)
	{
		printf("\nnumber of arguments passed: %d",argc);
		printf("\n---arguments that were passed---");
		for(counter = 0; counter < argc; counter++)
			printf("\nargv[%d]: %s,counter,argv[counter]);

	}
	return 0;
	}
