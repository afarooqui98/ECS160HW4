#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINES 20000
#define MAX_CHARS 1024

/*
* getColIndex - finds the index of a column in a csv header
*
* return - the coulmn index; -1 otherwise
*/
int getColIndex(char* temp, char* columnName){
	char* token = strtok(temp, ",");
	int index = 0;
	char quotedCol[50];
	sprintf(quotedCol, "\"%s\"", columnName);
	printf("Quoted COl: %s\n", quotedCol);
	while(token){
		printf("%s\n", token);
		if(strcmp(token, columnName) == 0 || strcmp(token, quotedCol) == 0){
			return index;
		}

		token = strtok(NULL, ",");
		index++;
	}

	return -1;
}

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

bool validateLine(char* line, int tweetIndex){
	bool isValid = true;
	int numQuotes = 0;
	//int numComma = 0;
	char* tmpLine = strdup(line);
	//check that all quotes are matching
	while(*tmpLine != '\0'){
		if(*tmpLine == '\"'){
			numQuotes++;
		}
		tmpLine++;
	}
	//the number of quotes in a line is not even
	if(numQuotes%2 != 0){
		isValid = false;
		printf("Number of quotes is not even.\n");
	}
	printf("Number of quotes: %d\n", numQuotes);
	//check that tweet does not contain a comma

	const char* tweet = getfield(strdup(line), tweetIndex);
	if(tweet == NULL){
		isValid = false;
		printf("No tweet found\n");
	}
	else{
		//check that the tweet does not contain a comma
		printf("TWEET: %s\n", tweet);
		//verify tweet has proper amount of quotes:
		numQuotes = 0;
		tmpLine = strdup(tweet);
		while(*tmpLine != '\0'){
			if(*tmpLine == '\"'){
				numQuotes++;
			}
			tmpLine++;
		}
		if(numQuotes%2 != 0){
			isValid = false;
			printf("Tweet has unmatching quotes.\n");
		}

		//Try to check for commas, after testing it seems
		//that a comma causes the tweet to be cut off
		// so the proper way of testing for a comma would
		// be to look for an unmatched quote

		//Is this necessary? 
		// while(*tweet != '\0'){
		// 	if(*tweet == ','){
		// 		numComma++;
		// 	}
		// 	tweet++;
		// }
		// printf("Number of commas: %d\n", numComma);
		// //tweets that contain commas are not valid
		// if(numComma > 0){
		// 	isValid = false;
		// 	printf("Tweet has a comma.\n");
		// }
	}

	//TODO: Check other cases that invalidate the line
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
	int textIndex;
	while(fgets(line, MAX_CHARS, stream)){
		char* tmpLine = strdup(line);

		//get the index of name
		if(isFirstLine){
			nameIndex = getColIndex(strdup(line), "name");
			textIndex = getColIndex(strdup(line), "text");
			if(nameIndex == -1 || textIndex == -1){
				return killProgram();
			}
			// printf("name index is %d\n", nameIndex);
			// printf("text index is %d\n", textIndex);
			isFirstLine = false;
		}
		if(!validateLine(tmpLine, textIndex)){
			//Kill or continue?
			return killProgram();
		}
		else{
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

		printf("%s: %d\n", topNames[i], topTweetsPerName[i]);
	}
}