#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct resources
{
	int A,B,C;
};

FILE * fileName;
void loadFile();
int numberOfLines();

int main(int argc, char* argv[])
{
fileName = fopen(argv[1],"r");
loadFile();
}

int numberOfLines()
{
	int lines =0;
	char ch;
	while(!feof(fileName))
		{
  		ch = fgetc(fileName);
  		if(ch == '\n')
    			lines++;
	}
	rewind(fileName);
lines = lines -2;
return lines;
}
void loadFile()
{
	int line =0, column=0;
	char *elementFromFile;
	char *element, *subelement;
	char *saveptr;
	char lineFromFile[100];
	int numberOfProc = numberOfLines();
	printf("Number of Lines in file: %d",numberOfProc);
	struct resources processes[numberOfProc];
	int initialise = 0;
	
	while(fgets(lineFromFile,1000,fileName))
	{
		if (strstr(lineFromFile,"P"))
		{
			initialise = 1;
			elementFromFile = lineFromFile;
		}
	
		element = strtok_r(elementFromFile,"\t",&elementFromFile);
		
		while (element != NULL && initialise == 1)
		{
			printf(" %s ", element);
			
			element = strtok_r(elementFromFile,"\t",&elementFromFile);
			
			column = column + 1;
			if(column == 3)
				column = 0;
		}

	}
}
