#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "getLine.h"
#include "protoTree.h"
int main( int argc, char * argv[] )
{
	int i = 1;
	FILE * fp;
	Tree dictTree;
	create(&dictTree); 


	while (i < argc)
	{
		if (strcmp(argv[i], "-print") == 0)
		{
			//print the leaves with the count
			printTree(dictTree);
			i++;
		}
		else if (strcmp(argv[i], "-dump") == 0)
		{
				dump(dictTree);
				i++;
		}
		else if (strcmp(argv[i], "-set") == 0)
		{
			long value;
			char* endptr = (char*) 1;
			value = strtol(argv[i + 1], &endptr, 10);
			if (*endptr != '\0')
			{
				fprintf(stderr, " Invalid Integer to set IF value: %s\n", strerror (errno));
				exit(-1);
			}
			//it was a number
			setIF(value);
			i += 2;
		}
		else if (strcmp(argv[i], "-d") == 0)
		{
			fp = fopen(argv[i + 1], "r");
			if (fp == NULL)
			{
				fprintf(stderr, " Invalid -d file: %s\n", strerror (errno));
				exit(-1);
			}
			//deal with deletions
			char* word;
			while ((word = getLine(fp)) != NULL)
			{
				dictTree = delete(&dictTree, word);
				free(word);

			}
			fclose(fp);
			i += 2;
		}
		else if (strcmp(argv[i], "-epl") == 0)
		{
			epl(dictTree);
			i++;
		}
		else//check to see if file pointer is valid, and insert
		{
			fp = fopen(argv[i], "r");
			if (fp == NULL)
			{
				fprintf(stderr, " Invalid insert file: %s\n", strerror (errno));
				exit(-1);
			}
			char* word;
			while ((word = getLine(fp)) != NULL)
			{
				searchInsert(&dictTree, word);
				free(word);
			}
			fclose(fp);
			i++;
		}
	}
	//finished while loop of all the command line arguments
	exit(1);
}

