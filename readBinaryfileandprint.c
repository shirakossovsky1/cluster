
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	int i,j,numOfGroups,sizeOfGroup;
	int *row;
	
	FILE *fileIn;

	fileIn = fopen(argv[1],"r");

	fread(&numOfGroups,sizeof(int),1,fileIn);

	printf("there is %d gropus in the divison\n",numOfGroups);

	for (i = 0; i < numOfGroups; ++i)
	{
		fread(&sizeOfGroup,sizeof(int),1,fileIn);
		printf("In group %d there is %d vertices\n", i , sizeOfGroup);
		
		row = (int*)malloc(sizeOfGroup*sizeof(int));
		
		fread(row,sizeof(int),sizeOfGroup,fileIn);
		
		for (j = 0; j < sizeOfGroup; ++j){
			printf("%d ",*(row+j));
		}
		printf("\n");
		free(row);
	}
	fclose(fileIn);
	
	return 0;
}