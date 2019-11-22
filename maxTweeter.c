#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

const char* getfield(char* line, int num)
{
    const char* tok;
    //for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    for (tok = strtok(line, ","); ; tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

// int main(int argc, char** argv)
// {
//     printf("Input file: %s\n", argv[1]);
//     FILE* stream = fopen(argv[1], "r");

//     char line[1024];
//     float lines = 0;
//     float lenTotal = 0;
//     while (fgets(line, 1024, stream))
//     {
//         char* tmp = strdup(line);
//         const char* out = getfield(tmp, 12);
//         float nextLength = strlen(out);
//         lines++;
//         printf("Text: %s\n", out);
//         printf("Length: %f\n", nextLength);
        
//         float wC = getWordCount(out, nextLength);
//         printf("Word Count: %f\n", wC);
//         int aveCPW = (nextLength - (wC-1))/wC;
// 		printf("Chars per word: %d\n", aveCPW);
//         char* wordTwoGuess = (char *) malloc(aveCPW+1 * sizeof(char));
//         memcpy(wordTwoGuess, &out[aveCPW + 1], aveCPW);
//         printf("Guess at second word: %s\n", wordTwoGuess);


//         if(wC == 2)
// 	{
//       	  printf("%c\n", out[2000048]);
// 	}

// 	if(wC > 5)
// 	{
// 	  free(tmp);
// 	}
//         lenTotal += nextLength;
//         // NOTE strtok clobbers tmp
//         free(tmp);
//     }

//     printf("Average Tweet Length: %f\n", lenTotal/lines);
// }

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

//read csv
//store fields to array
//propogate through array
//define 10th, 9th ... 1st
//print them all out 
int main(int argc, char* argv[]){
	if(argc < 2){
		printf("please supply an in the form of a csv file or path to csv file as an argument\n");
		exit(0);
	}

	//array for the whole csv
	char* names[20000];
	int numTweetsPerName[20000];
	int arrayLen = sizeof(numTweetsPerName)/sizeof(numTweetsPerName[0]);

	//set initial value for char array
	for(int i = 0; i <  arrayLen; i++){
		names[i] = "";
		numTweetsPerName[i] = 0;
	}

	FILE *stream;
	char line[1024];

	//error check csv open
    stream = fopen(argv[1], "r");
	if(stream == NULL){
		printf("error printing file\n");
		exit(0);
	}

	int nonce = 0;
	while(fgets(line, 1024, stream)){
		char* tmpLine = strdup(line);
		//why is out const??
		const char* out = getfield(tmpLine, 9);
		if(nonce == 0){
			printf("%s\n", out);
			nonce++;
		}
		
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