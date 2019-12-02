#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINES 20000
#define MAX_CHARS 1024

/*
* strTok - custom strtok functon that is able to handle empty elements between delimeters
*
* return - the token from the string if a delimeter is found; otherwise (char*)0
*/
char* strTok(char** newString, char* delimiter)
{
    char* string = *newString;
    char* delimiterFound = (char*) 0;
    int tokLength = 0;
    char* tok = (char*) 0;

    if(!string) return (char*) 0;

    delimiterFound = strstr(string, delimiter);

    if(delimiterFound){
        tokLength = delimiterFound-string;
    }else{
        tokLength = strlen(string);
    }

    tok = malloc(tokLength + 1);
    memcpy(tok, string, tokLength);
    tok[tokLength] = '\0';

    *newString = delimiterFound ? delimiterFound + strlen(delimiter) : (char*)0;

    return tok;
}

/*
* getColIndex - finds the index of a column in a csv header
*
* return - the coulmn index; -1 otherwise
*/
int getColIndex(char* temp, char* columnName){
	char* token;
	int index = 0;

	//all the possible permutations of token "formats"
	char quotedCol[50];
	char colWithNewLine[50];
	char quotedColWithNewLine[50];
	sprintf(quotedCol, "\"%s\"", columnName);
	sprintf(colWithNewLine, "%s\n", columnName);
	sprintf(quotedColWithNewLine, "\"%s\"\n", columnName);
	printf("Quoted COl: %s\n", quotedCol);

	while((token = strTok(&temp, ","))){
		printf("%s\n", token);
		if(strcmp(token, columnName) == 0 
			|| strcmp(token, quotedCol) == 0 
			|| strcmp(token, colWithNewLine) == 0 
			|| strcmp(token, quotedColWithNewLine) == 0){
			return index;
		}
		index++;
	}

	return -1;
}

/*
* getfield - finds an element in a line given the column index for that element
*
* return - the element if it is present in the line, NULL otherwise
*/
const char* getfield(char* line, int num)
{
	int i = 0;
	char* tok;

	while((tok = strTok(&line, ","))){
		if(tok && *tok){
			if(i == num){
				printf("%s\n", tok);
				return tok;
			}
			printf("%s ", tok);
			i++;
			continue;
		}
		i++;
		continue;
	}

    return NULL;
}

//Count the columns in a line
int countCols(char* line){
	int totalCommas = 0;

	char* tmpLine = strdup(line);

	while(*tmpLine != '\0'){
		if(*tmpLine == ','){
			totalCommas++;
		}
		tmpLine++;
	}

	if(totalCommas == 0){
		return -1;
	}

	return totalCommas;
}

bool validateLine(char* line, int headerCols){
	bool isValid = true;
	int numCols = 0;
	
	char* tmpLine = strdup(line);
	//check that number of columns (comma seperated) match
	// the number of columns in the header
	while(*tmpLine != '\0'){
		if(*tmpLine == ','){
			numCols++;
		}
		tmpLine++;
	}
	//the number of commas does not match the header
	if(numCols != headerCols){
		isValid = false;
		printf("Number of columns does not match header.\n");
	}
	printf("Number of columns: %d\n", numCols);
	
	return isValid;
}

//find the index of the largest element
int findBiggestIndex(int nums[], int size){
	int biggest = -1;
	int index = 0;
	for(int i = 0; i < size; i++){
		if(nums[i] > biggest){
			biggest = nums[i];
			index = i;
		}
	}

	return index;
}

int killProgram(){
	printf("Invalid Input Format\n");
	return 0;
}


//read csv
//store fields to array
//propogate through array
//define 10th, 9th ... 1st
//print them all out 
int main(int argc, char* argv[]){
	if(argc < 2){
		return killProgram();
	}

	//array for the whole csv
	char* names[MAX_LINES];
	int numTweetsPerName[MAX_LINES];
	int arrayLen = sizeof(numTweetsPerName)/sizeof(numTweetsPerName[0]);

	//set initial value for arrays
	for(int i = 0; i <  arrayLen; i++){
		names[i] = "";
		numTweetsPerName[i] = 0;
	}

	FILE *stream;
	char line[MAX_CHARS];
	//error check csv open
    stream = fopen(argv[1], "r");
	if(stream == NULL){
		return killProgram();
	}

	bool isFirstLine = true;
	int nameIndex;
	int headerCols;
	while(fgets(line, MAX_CHARS, stream)){
		char* tmpLine = strdup(line);

		//get the index of name
		if(isFirstLine){
			nameIndex = getColIndex(strdup(line), "name");
			headerCols = countCols(strdup(line));
			if(nameIndex == -1 || headerCols == -1){
				return killProgram();
			}
			printf("name index is %d\n", nameIndex);
			printf("header columns: %d\n", headerCols);
		}

		if(!validateLine(tmpLine, headerCols)){
			//Kill or continue?
			return killProgram();
		} else{
			if(isFirstLine){
				isFirstLine = false;
				continue;
			}

			const char* name = getfield(tmpLine, nameIndex);
			if(name == NULL){
				//Kill or continue?
				continue;
			}
			else{
				for(int i = 0; i < arrayLen; i++){
					if(strcmp(name, names[i]) == 0){
						numTweetsPerName[i]++;
						break;
					}else{
						if(strcmp(names[i], "") == 0){
							names[i] = strdup(name);
							numTweetsPerName[i]++;
							break;
						}
					}
				}
			}
		}
		free(tmpLine);
	}

	char* topNames[10] = {"", "", "", "", "", "", "", "", "", ""};
	int topTweetsPerName[10] = {0,0,0,0,0,0,0,0,0,0};
	for(int i = 0; i < 10; i++){
		//find index of largest value and set it
		int biggestIndex = findBiggestIndex(numTweetsPerName, arrayLen);
		topNames[i] = strdup(names[biggestIndex]);
		topTweetsPerName[i] = numTweetsPerName[biggestIndex];
		
		//remove the largest element to get the next largest
		numTweetsPerName[biggestIndex] = 0;
	}
	//TODO: format the output properly (remove extra quotes around names & extra lines)
	for(int i = 0; i < 10; i++){
		if(topTweetsPerName[i] == 0){
			continue;
		}
		char* tmpname = topNames[i];
		char* token;
		while((token = strTok(&tmpname, "\""))){
			topNames[i] = token;

			if(strlen(topNames[i]) > 0){
				break;
			}
		}
		printf("%s: %d\n", topNames[i], topTweetsPerName[i]);
	}
}