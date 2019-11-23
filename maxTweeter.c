#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_LINES = 20000;
const int MAX_CHARS = 1024;

int getWordCount(const char* text, int length)
{
    int wordCount = 0;
    if(length >= 2) //Due to quotes;
    {
        wordCount += 1;
    }
    int i = 0;
    for(i = 0; i < length; i++){
    	if(' ' == text[i])
  		{
    		wordCount++;
    	}
    }
    return wordCount;
}

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

    tok = malloc(tokLenght + 1);
    memcpy(tok, string, tokLength);
    tok[tokLenght] = '\0';

    *newString = delimiterFound ? delimiterFound + strlen(delimiter) : (char*)0;

    return tok;
}

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

void killProgram(){
	printf("Invalid Input Format\n");
	exit(0);
}

int getNameIndex(char* temp){
	char* token = strtok(temp, ",");
	int index = 0;
	while(token){
		printf("%s\n", token);
		if(strcmp(token, "name") == 0 || strcmp(token, "\"name\"") == 0){
			return index;
		}

		token = strtok(NULL, ",");
		index++;
	}

	return -1;
}

//read csv
//store fields to array
//propogate through array
//define 10th, 9th ... 1st
//print them all out 
int main(int argc, char* argv[]){
	if(argc < 2){
		killProgram();
	}

	//array for the whole csv
	char* names[MAX_LINES];
	int numTweetsPerName[MAX_LINES];
	int arrayLen = sizeof(numTweetsPerName)/sizeof(numTweetsPerName[0]);

	//set initial value for char array
	for(int i = 0; i <  arrayLen; i++){
		names[i] = "";
		numTweetsPerName[i] = 0;
	}

	FILE *stream;
	char line[MAX_CHARS];
	//error check csv open
    stream = fopen(argv[1], "r");
	if(stream == NULL){
		killProgram();
	}

	int nonce = 0;
	int nameIndex;
	while(fgets(line, MAX_CHARS, stream)){
		char* tmpLine = strdup(line);
		if(nonce == 0){
			nameIndex = getNameIndex(strdup(line));
			if(nameIndex == -1){
				killProgram();
			}
			printf("name index is %d\n", nameIndex);
			nonce++;
		}
		//why is out const??
		const char* out = getfield(tmpLine, nameIndex);
		for(int i = 0; i < arrayLen; i++){
			if(strcmp(out, names[i]) == 0){
				numTweetsPerName[i]++;
				break;
			}else{
				if(strcmp(names[i], "") == 0){
					names[i] = strdup(out);
					numTweetsPerName[i]++;
					break;
				}
			}
		}
		free(tmpLine);
	}

	char* topNames[10] = {"", "", "", "", "", "", "", "", "", ""};
	int topTweetsPerName[10] = {0,0,0,0,0,0,0,0,0,0};
	for(int i = 0; i < 10; i++){
		//find index of largest value and set it
		int index = findBiggestIndex(numTweetsPerName, arrayLen);
		topNames[i] = strdup(names[index]);
		topTweetsPerName[i] = numTweetsPerName[index];

		//remove the largest element to get the next largest
		strcpy(names[index], "");
		numTweetsPerName[index] = 0;
	}

	for(int i = 0; i < 10; i++){
		printf("%s: %d\n", topNames[i], topTweetsPerName[i]);
	}
}