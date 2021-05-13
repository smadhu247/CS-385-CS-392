/*******************************************************************************
 * Name        : sort.c
 * Author      : Sanjana Madhu and Lasya Josyula
 * Date        : March 2, 2021
 * Description : Uses quicksort to sort a file of either ints, doubles, or
 *               strings.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quicksort.h"

#define MAX_STRLEN     64 // Not including '\0'
#define MAX_ELEMENTS 1024

typedef enum {
    STRING,
    INT,
    DOUBLE
} elem_t;

/**
 * Basic structure of sort.c:
 *
 * Parses args with getopt.
 * Opens input file for reading.
 * Allocates space in a char** for at least MAX_ELEMENTS strings to be stored,
 * where MAX_ELEMENTS is 1024.
 * Reads in the file
 * - For each line, allocates space in each index of the char** to store the
 *   line.
 * Closes the file, after reading in all the lines.
 * Calls quicksort based on type (int, double, string) supplied on the command
 * line.
 * Frees all data.
 * Ensures there are no memory leaks with valgrind.
 */


void displayUsage(){
        printf("Usage: ./sort [-i|-d] filename\n"
               "   -i: Specifies the file contains ints.\n"
               "   -d: Specifies the file contains doubles.\n"
               "   filename: The file to sort.\n"
               "   No flags defaults to sorting strings.\n");
}

int main(int argc, char **argv) {
    
    extern char *optarg;
    extern int optind, opterr, optopt;
    int opt;
    elem_t typeDefined = STRING;
    int i_flag = 0;
    int d_flag = 0;

    int count = 0;
    opterr = 0;
    
    while ((opt = getopt(argc, argv, ":id")) != -1){
        switch (opt){
            case 'i':
                typeDefined = INT;
                i_flag = 1;
                break;
            case 'd':
                typeDefined = DOUBLE;
                d_flag = 1;
                break;
            case '?':
                //wrong flag - case 2
                printf("Error: Unknown option '-%c' received.\n", optopt);
                displayUsage();
                return EXIT_FAILURE;
        }
    }
    
    count = i_flag + d_flag;
    //checks that there are no input arguments - case 1
    if(argc == 1){
        displayUsage();
        return EXIT_FAILURE;
    }

    // printf("%d\n", count);

    if (count > 1){
        printf("Error: Too many flags specified.\n");
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(argv[optind], "r");

    if(argc == 2 && count == 1) {
        printf("Error: No input file specified.\n");
        return EXIT_FAILURE;
    }

    if(argv[optind+1]){
        printf("Error: Too many files specified.\n");
        return EXIT_FAILURE;
    }
    
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open '%s'. %s.\n", argv[optind], strerror(errno));
        return EXIT_FAILURE; // exit(EXIT_FAILURE);
    }


    
    char ** val = (char **)malloc(MAX_ELEMENTS * sizeof(char *));
    if (val == NULL){
        return EXIT_FAILURE;
    }
    
    //read in the data
    size_t index = 0;
    char str[MAX_STRLEN + 2];
    char *eoln;
    
    while(fgets(str, MAX_STRLEN + 2, fp) != NULL){
        eoln = strchr(str, '\n');
        if (eoln == NULL){
            str[MAX_STRLEN] = '\0';
        } else {
            *eoln = '\0';
        }
        if (strlen(str) != 0){
            val[index] = (char *)malloc((MAX_STRLEN + 1) * sizeof(char));
            strcpy(val[index++], str);
        }
    }
    
    fclose(fp);
    
    if (typeDefined == STRING){
        quicksort(val, index, sizeof(char *), str_cmp);
        for (int in = 0; in < index; in++){
            printf("%s\n", *(val+in));
        }
    }
    
    else if (typeDefined == INT){
        //since its a int create a array with type int
        int *val2 = (int *)malloc(index * sizeof(int));
        for (int j = 0; j < index; j++){
            val2[j] = atoi(*(val+j));
        }
        quicksort(val2, index, sizeof(int), int_cmp);
        for (int in2 = 0; in2 < index; in2++){
            printf("%d\n", *(val2+in2));
        }
        free(val2);
    }
    
    else if (typeDefined == DOUBLE){
        //since its a double create a array with type double
        double *val3 = (double *)malloc(index * sizeof(double));
        for (int k = 0; k < index; k++){
            val3[k] = atof(*(val+k));
        }
        quicksort(val3, index, sizeof(double), dbl_cmp);
        for (int in3 = 0; in3 < index; in3++){
            printf("%f\n", *(val3+in3));
        }
        free(val3);
    }
    
    for (int i = 0; i < index; i++){
        free(*(val+i));
    }
    free(val);
    return EXIT_SUCCESS;
    
}

