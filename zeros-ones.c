#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

void unPrintable(int decimal){
  //the odd spacing is to have a uniform print
  char unprintables[34][6] = {
    "NUL","SOH","STX","ETX","EOT","ENQ","ACK","BEL",
    " AS","TAB"," LF"," VT"," FF"," CR"," SO"," SI","DLE",
    "DC1","DC2","DC3","DC4","NAK","SYN","ETB","CAN",
    " EM","SUB","ESC"," FS"," GS"," RS"," US","SPACE","DEL"
  };
  if (decimal != 127){
		if (decimal != 32){
			printf("      ");
			for (int i = 0; i < 6; i++){
				printf("%c", unprintables[decimal][i]);
			}
		}
		else{
			printf("    ");
			for (int i = 0; i < 6; i++){
				printf("%c", unprintables[decimal][i]);
			}
			printf("  ");
		}
  }
  else{
		printf("      ");
		for (int i = 0; i < 6; i++){
    	printf("%c", unprintables[33][i]);
		}
  }
}

char* sumforPar(int binArray[]){
	int sum = 0, i = 0;
	for (i = 0; i < 8; i++){
			sum = sum + binArray[i];
	}
	if (sum % 2 == 0){
			return "EVEN";
	}
	else{
			return "ODD";
	}
}

int toDecimal(int binArray[]){
	int decimal = 0;
	for (int i = 7, j = 0; i > 0; i--, j++) {
		decimal = decimal + ((binArray[i] - 48) * pow(2, j));
	}

	return decimal;
}

void printTopRow(){
	printf("Original ASCII    Decimal  Parity \n");
	printf("-------- -------- -------- -------- \n");
}

void printRest(int binAscii[]){
	int flag = 0;

	for(int i = 0; i < 8; i++){
		printf("%c", binAscii[i]); //%s to %d
		//printf(" ");
	}
	if (toDecimal(binAscii) > 32 && toDecimal(binAscii) < 127){
		flag = 0;
	}
	else{
		flag = 1;
	}
	//for printing ASCII char
	if (flag == 0){
		printf("\t%c", toDecimal(binAscii));
	}
	else{
		unPrintable(toDecimal(binAscii));
	}

	//for printing decimal
	if(flag == 0){
		printf("%9d ", toDecimal(binAscii));
	}
	else{
		printf("%6d ", toDecimal(binAscii));
	}
	printf("%s", sumforPar(binAscii));
	printf("\n");
}

int typeOfInput(int argc, char **argv){
  if((argv[1][0] > 96 && argv[1][0] < 123) && argc == 2){
    //read from file
    printf("its a file\n");
    return 1;
  }
  else if(argv[1][0] == '0' || argv[1][0] == '1'){
    //read from commandline no dash
    return 2;
  }
  else if(argv[1][0] == '-' && (argv[2][0] == '0' || argv[2][0] == '1')) {
    //read from commandline with dash
    return 3;
  }
  else{
    //not a valid input
    return 4;
  }

}

int main(int argc, char **argv){
	int binAscii[8] = {0};
  int returnVal = -2;
  int counter = 0;
  int typeOfInp = -1;

	if (argc < 2){
		//no parameter entered or no file
		printf("no parameters entered\n");
	}
	else{
    typeOfInp = typeOfInput(argc,argv);
    if (typeOfInp == 1){
      char buffer[1] = {0};
      int fDescriptor;

      fDescriptor = open(argv[1], O_RDONLY);

      if (fDescriptor == -1) {
        printf("File error\n");
        return(-5); //random return number
      }
      else {
        printTopRow();
        while (returnVal != 0){
          returnVal = read (fDescriptor, buffer, 1);

          if (returnVal != 0) {
            if (buffer[0] != ' '){
              binAscii[counter] = buffer[0];
              counter++;
            }

            if (counter == 8) {
              printRest(binAscii);
              counter = 0;
              for (int i = 0; i < 8; i++){
                binAscii[i] = '0';
              }
            }
          }
        }

        if (counter > 0) {
          for (int i = counter - 1; i < 8; i++) {
            binAscii[i] = '0';
          }
          printRest(binAscii);
        }
      }
  		close(fDescriptor);
  	}
  }
	return 0;
}
