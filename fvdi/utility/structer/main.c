/*
 * C struct to assembly equ converter
 *
 * Copyright 1997-2002, Johan Klockars
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hash.h"
#include "expr.h"
#include "list.h"

#define IFMT       if (opt & 0x02)

int opt;
extern int yydebug;

char *filename = NULL;
extern FILE *yyin;

int main(int argc, char **argv)
{
    int i, total, usage, collisions;
    int count;
    char *name[30];

    yyin = stdin;
    count = 0;
    opt = 0;
    if (argc > 1)
    {
        for (i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "-c") == 0)
                opt |= 0x01;
            else if (strcmp(argv[i], "-d") == 0)
                opt |= 0x02;
#ifdef YYDEBUG
            else if (strcmp(argv[i], "-y") == 0)
                yydebug = 1;
#endif
            else if (strcmp(argv[i], "-file") == 0)
            {
                if (i < argc - 1)
                {
                    i++;
                    filename = argv[i];
                } else
                {
                    fprintf(stderr, "No file name given!\n");
                    exit(1);
                }
                if (!(yyin = fopen(filename, "r")))
                {
                    fprintf(stderr, "Could not open file!\n");
                    exit(1);
                }
            } else
            {
                name[count] = argv[i];
                count++;
#if 0
                printf("Usage:  clfl [-c] [-file filename]\n");
                exit(1);
#endif
            }
        }
    }

    if (!yyparse())
    {
        IFMT hash_stat(&total, &usage, &collisions);
        IFMT printf("Used %d/%d hash table entries.\n", usage, total);
        IFMT printf("Maximum number of collisions: %d\n\n", collisions);
        IFMT printdefs(definitions);

        for (i = 0; i < count; i++)
        {
            convert(name[i], definitions);
        }
    } else
    {
        fprintf(stderr, "Error while parsing!\n");
        return 1;
    }
    return 0;
}
