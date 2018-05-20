#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct resources
{
	char *A,*B,*C;
} 

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
	char *elementFromFile, *elementFromElement;
	char *element, *subelement;
	char *saveptr;
	char lineFromFile[100];
	int numberOfProc = numberOfLines();
	printf("Number of Lines in file: %d",numberOfProc);
	struct resources Request[numberOfProc];
	struct resources Allocation[numberOfProc];
	struct resources Available[numberOfProc];
	int initialise = 0;
        elementFromFile = "";
        elementFromElement = "";
	
	while(fgets(lineFromFile,1000,fileName))
	{
	
		column = 0;
		if (strstr(lineFromFile,"P0"))
		{
			printf("P0FOUND\n\n\n");
			initialise = 1;
		}
		elementFromFile = lineFromFile;	
		element = strtok_r(elementFromFile,"\t",&elementFromFile);
		
		
		while (element != NULL && initialise == 1)
		{
			//printf("%s,", element);
			element = strtok_r(elementFromFile,"\t",&elementFromFile);
			

		        ///printf("\n\n\n");
				
			elementFromElement = element;
				
     	
			
			if (column == 0)
				{
					Allocation[line].A = strtok_r(elementFromElement," ",&elementFromElement);
					Allocation[line].B = strtok_r(elementFromElement," ",&elementFromElement);
					Allocation[line].C = strtok_r(elementFromElement," ",&elementFromElement);
			        }
			else if (column ==1)
				{
					Request[line].A = strtok_r(elementFromElement," ",&elementFromElement);
					Request[line].B = strtok_r(elementFromElement," ",&elementFromElement);
					Request[line].C = strtok_r(elementFromElement," ",&elementFromElement);
				
				}
				
				
			
				printf("\n");		
//			}
			
	        column = column + 1;		
		}
		line = line + 1;

	}
}
