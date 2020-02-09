#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void printTopRow() {
	printf("Original ASCII    Decimal  Parity \n");
	printf("-------- -------- -------- -------- \n");
}


int toDecimal(int binArray[]) {
	int decimal = 0;
	for (int i = 7, j = 0; i > 0; i--, j++) {
		decimal = decimal + ((binArray[i] - 48) * pow(2, j));
	}

	return decimal;
}

char* sumforPar(int binArray[]) {
	int sum = 0, i = 0;
	for (i = 0; i < 8; i++) {
			sum += binArray[i];
	}
	if (sum % 2 == 0)
	{
			return "EVEN";
	}
	else {
			return "ODD";
	}
}

int main(int argc, char **argv) {
	int readerInt;
	int binAscii[8] = {0};

	if (argc < 2){
		//no parameter entered or no file
		printf("file name not valid\n");
	}
	else{
		printTopRow();
		FILE* file = fopen(argv[1], "r");

		int counter = 0;

		while((readerInt = fgetc(file)) != EOF){ //(readerInt = fgetc(file)) != EOF
				if (readerInt != ' '){
					binAscii[counter] = readerInt;
					counter++;
				}
				//printf("this is the reader int: %d", readerInt);
				if (counter == 8 || (counter > 0 && readerInt == ' ')){
					for(int i = 0; i < 8; i++){
						printf("%c", binAscii[i]); //%s to %d
						//printf(" ");
					}
					printf("\t");

					//for printing ASCII char
					printf("%c", toDecimal(binAscii));

					//for printing decimal
					printf("%8d\t", toDecimal(binAscii));

					printf("%s", sumforPar(binAscii));

					//reseting the array and counter
					for(int i = 0; i < 8; i++){
						binAscii[i] = '0';
					}
					counter = 0;

					printf("\n");
				}
		}
		if (counter > 0) {
			//printf("counter: %d", counter);
			for(int j = counter - 1; j < 8; j++){
				binAscii[j] = '0';
			}
			for(int i = 0; i < 8; i++){
				printf("%c", binAscii[i]); //%s to %d
			}
			printf("\t");

			//for printing ASCII char
			printf("%c", toDecimal(binAscii));

			//for printing decimal
			printf("%8d\t", toDecimal(binAscii));

			printf("%s", sumforPar(binAscii));

			printf("\n");
			counter = 0;
		}

		fclose(file);
	}

	return 0;
}
