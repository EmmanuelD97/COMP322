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
  //the reason for all these if statements is because
  //some of the words have different spacing and I had to
  //do this to make them look all uniform
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

/*
sums all the 1s and if theyre divisible by 2
then they're even
*/
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

/*
the items in the binArray were read in as characters
so that means a 1 is a 49 and a 0 is a 48.
I subtract by 48 to set them to 1 or 0 then multiply by
2 to the power of whatever spot they are in from the end to get
the decimal conversion
*/
int toDecimal(int binArray[]){
	int decimal = 0;
	for (int i = 7, j = 0; i > 0; i--, j++) {
		decimal = decimal + ((binArray[i] - 48) * pow(2, j));
	}

	return decimal;
}

//just printing the header for the chart
void printTopRow(){
	printf("Original ASCII    Decimal  Parity \n");
	printf("-------- -------- -------- -------- \n");
}

void printRest(int binAscii[]){
  //if the flag is 0 it means the number is a printable character
  //f it is set to 1 it means it is unprintable and gets sent to a
  //different function for printing
	int flag = 0;

	for(int i = 0; i < 8; i++){
		printf("%c", binAscii[i]);
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
  //again two different statements because the
  //non printable characters take more space
  //and this makes them look uniform
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
    //if first character is a letter and theres only two items input
    //read from file
    return 1;
  }
  else if(argv[1][0] == '0' || argv[1][0] == '1'){
    //if the first item is a 0 or a 1 means theres no dash
    //and there is no file to read from so we have to
    //read from commandline without a dash
    return 2;
  }
  else if(argv[1][0] == '-' && (argv[2][0] == '0' || argv[2][0] == '1')) {
    //first argument is a dash and the next argument is a 0 or a 1
    //read from commandline and take into account a dash
    return 3;
  }
  else{
    //not a valid input
    printf("its invalid\n");
    return 4;
  }

}
int inputTypeOne(char **argv){
  char buffer[1] = {0};
  int binAscii[8] = {0};
  int counter = 0, numberPrinted = 0;
  int returnVal = -2;
  int fDescriptor;

  fDescriptor = open(argv[1], O_RDONLY);

  if (fDescriptor == -1) {
    printf("File error\n");
    return(-5); //random return number
  }
  else {
    while (returnVal != 0){ //while the file is not empty read next character
      returnVal = read (fDescriptor, buffer, 1);

      if (returnVal != 0) {
        if (buffer[0] != ' '){
          binAscii[counter] = buffer[0];
          counter++;
        }
        //we have 8 characters in the array so print
        if (counter == 8) {
          printRest(binAscii);
          counter = 0;
          numberPrinted++; //keeping track of how many I've printed so far
          for (int i = 0; i < 8; i++){ //refill array with 0s
            binAscii[i] = '0';
          }
        }
      }
      else if(returnVal == 0 && numberPrinted == 0){
        printf("The file is empty!\n");
      }
    }
    //if more than one character in the array but at the end of the file
    //pad the rest to the right with 0s and print
    if (counter > 0) {
      for (int i = counter - 1; i < 8; i++) {
        binAscii[i] = '0';
      }
      printRest(binAscii);
    }
  }
  close(fDescriptor);
  return(0);
}

void inputFromLine(int typeOfInp, int argc, char **argv){
  int binAscii[8] = {0};
  int counter = 0;

  if (typeOfInp == 2){
    counter = 1;
  }
  else if (typeOfInp == 3){
    //moving the counter up one to account for the '-'
    counter = 2;
  }
  //first for loop goes through the arguments
  //second one goes through individual characters in each argument
  for (int i = counter; i < argc; i++){
    for (int j = 0; j < 8; j++){
      if (argv[i][j] == '0' || argv[i][j] == '1'){
        binAscii[j] = argv[i][j];
      }
      else if (argv[i][j] != '0' || argv[i][j] != '1'){
        //if we hit a space then pad the rest to the right with 0s
        for (int k = j; k < 8; k++) {
          binAscii[k] = '0';
        }
        //once we pad with 0s move the inner for loop to the end
        j = 8;
      }
    }

    printRest(binAscii);

    //reset array to 0s
    for (int i = 0; i < 8; i++){
      binAscii[i] = '0';
    }
  }
}



int main(int argc, char **argv){
  int typeOfInp = -1;

	if (argc < 2){
		//no parameter entered or no file
		printf("no parameters entered\n");
	}
	else{
    typeOfInp = typeOfInput(argc,argv);
    if (typeOfInp == 1){
      printTopRow();
      inputTypeOne(argv);
  	}
    else if(typeOfInp == 2){
      printTopRow();
      inputFromLine(typeOfInp, argc, argv);
    }
    else if(typeOfInp == 3){
      printTopRow();
      inputFromLine(typeOfInp, argc, argv);
    }
    else if(typeOfInp == 4){
      printf("Not a valid input!");
    }
  }
	return 0;
}
