// getLine.c                                      Stan Eisenstat (10/27/09)

// Read a line of text using the file pointer *fp and returns a pointer to a
// null-terminated string that contains the text read, include the newline (if
// any) that ends the line.  Storage for the line is allocated with malloc()
// and realloc().  If the end of the file is reached before any characters are
// read, then the NULL pointer is returned.

#include <stdlib.h>
#include "getLine.h"
#include <stdio.h>
#include <ctype.h>

char *getLine(FILE *fp)
{
    char *line;                 // Line being read
    int size;                   // #chars allocated
    int c, i;

    size = sizeof(double);                      // Minimum allocation
    line = malloc (size);
    for (i = 0;  (c = getc(fp)) != EOF; )  {
	if (i == size-1) {
	    size *= 2;                          // Double allocation
	    line = realloc (line, size);
	}
    if (!isalnum(c))
    {
        while (!isalnum(c))
        {
            c = getc(fp);
            if (c == EOF)
                break;
        }
        ungetc(c, fp);
        if (i != 0)//we've seen a word already
            break;
        c = getc(fp);
        if (c == EOF)
            break;
    }
    line[i++] = tolower(c);


    }

    if (c == EOF && i == 0)  {                  // Check for immediate EOF
	free (line);
	return NULL;
    }

    line[i++] = '\0';                           // Terminate word
    line = realloc (line, i);                   // Trim excess storage

    return (line);
}
