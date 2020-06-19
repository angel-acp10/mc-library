#define _TEST_OBJ_ 1

#include "mc-obj.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    mcObj_t * screen = mcObj_create(NULL, NULL);

    mcObj_t * o1 = mcObj_create(screen, screen);
    mcObj_t * o11 = mcObj_create(o1, screen);
    mcObj_t * o12 = mcObj_create(o1, screen); 
    mcObj_t * o121 = mcObj_create(o12, screen);
    mcObj_t * o1211 = mcObj_create(o121, screen);
    mcObj_t * o1212 = mcObj_create(o121, screen);
    mcObj_t * o1213 = mcObj_create(o121, screen);
    mcObj_t * o13 = mcObj_create(o1, screen);
    printf("\nCreate 8 objects. Whose adresses are:\n");
    printf("\tscr \t= %p \tchild_list.size = %d\n", screen, screen->child_list.size);
    printf("\to1 \t= %p \tchild_list.size = %d\n", o1, o1->child_list.size);
    printf("\to11 \t= %p \tchild_list.size = %d\n", o11, o11->child_list.size);
    printf("\to12 \t= %p \tchild_list.size = %d\n", o12, o12->child_list.size);
    printf("\to121 \t= %p \tchild_list.size = %d\n", o121, o121->child_list.size);
    printf("\to1211 \t= %p \tchild_list.size = %d\n", o1211, o1211->child_list.size);
    printf("\to1212 \t= %p \tchild_list.size = %d\n", o1212, o1212->child_list.size);
    printf("\to1213 \t= %p \tchild_list.size = %d\n", o1213, o1213->child_list.size);
    printf("\to13 \t= %p \tchild_list.size = %d\n", o13, o13->child_list.size);

    /*
     *                  o1
     *                  |
     *              ---------
     *              |   |   |
     *             o11 o12 o13
     *                  |
     *                 o121
     *                  |
     *             -------------
     *             |     |     |
     *             o1211 o1212 o1213
     */

    printf("\nSTART o11 deletion\n");
    mcObj_delete(o11);
    printf("END o11 deletion\n");

    printf("\nRESULT:\n\n");
    printf("\tscr \t= %p \tchild_list.size = %d\n", screen, screen->child_list.size);
    printf("\to1 \t= %p \tchild_list.size = %d\n", o1, o1->child_list.size);
    printf("\to11 \t= %p \tchild_list.size = %d\n", o11, o11->child_list.size);
    printf("\to12 \t= %p \tchild_list.size = %d\n", o12, o12->child_list.size);
    printf("\to121 \t= %p \tchild_list.size = %d\n", o121, o121->child_list.size);
    printf("\to1211 \t= %p \tchild_list.size = %d\n", o1211, o1211->child_list.size);
    printf("\to1212 \t= %p \tchild_list.size = %d\n", o1212, o1212->child_list.size);
    printf("\to1213 \t= %p \tchild_list.size = %d\n", o1213, o1213->child_list.size);
    printf("\to13 \t= %p \tchild_list.size = %d\n", o13, o13->child_list.size);

    /*
     *                  o1
     *                  |
     *                  -----
     *                  |   |
     *                  o12 o13
     *                  |
     *                 o121
     *                  |
     *             -------------
     *             |     |     |
     *             o1211 o1212 o1213
     */

    printf("\nSTART o1 deletion\n");
    mcObj_delete(o1);
    printf("END o1 deletion\n");

    printf("\nRESULT:\n\n");
    printf("\tscr \t= %p \tchild_list.size = %d\n", screen, screen->child_list.size);
    printf("\to1 \t= %p \tchild_list.size = %d\n", o1, o1->child_list.size);
    printf("\to11 \t= %p \tchild_list.size = %d\n", o11, o11->child_list.size);
    printf("\to12 \t= %p \tchild_list.size = %d\n", o12, o12->child_list.size);
    printf("\to121 \t= %p \tchild_list.size = %d\n", o121, o121->child_list.size);
    printf("\to1211 \t= %p \tchild_list.size = %d\n", o1211, o1211->child_list.size);
    printf("\to1212 \t= %p \tchild_list.size = %d\n", o1212, o1212->child_list.size);
    printf("\to1213 \t= %p \tchild_list.size = %d\n", o1213, o1213->child_list.size);
    printf("\to13 \t= %p \tchild_list.size = %d\n", o13, o13->child_list.size);

    return 0;
}
