/*******************************************************************************
 * Name        : permstat.c
 * Author      : Sanjana Madhu and Lasya Josyula
 * Date        : 03/12/2021
 * Description :
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int perms[] = {S_IRUSR, S_IWUSR, S_IXUSR,
               S_IRGRP, S_IWGRP, S_IXGRP,
               S_IROTH, S_IWOTH, S_IXOTH};

void display_usage(char *progname) {
    printf("Usage: %s <filename>\n", progname);
}

/**
 * TODO
 * Returns a string (pointer to char array) containing the permissions of the
 * file referenced in statbuf.
 * Allocates enough space on the heap to hold the 10 printable characters.
 * The first char is always a - (dash), since all files must be regular files.
 * The remaining 9 characters represent the permissions of user (owner), group,
 * and others: r, w, x, or -.
 */
char* permission_string(struct stat *statbuf) {
    
    char * strin = (char *)malloc((10+1) *sizeof(char));
    int permission_valid;
    
    strin[0] = '-';
    for (int i = 1; i < 10; i += 3){
        permission_valid = statbuf->st_mode & perms[i-1];
        if (permission_valid){
            *(strin+i) = 'r';
        } else{
            *(strin+i) = '-';
        }
        permission_valid = statbuf->st_mode & perms[i];
        if (permission_valid){
            *(strin+i+1) = 'w';
        } else{
            *(strin+i+1) = '-';
        }
        permission_valid = statbuf->st_mode & perms[i+1];
        if (permission_valid){
            *(strin+i+2) = 'x';
        } else{
            *(strin+i+2) = '-';
        }
    }
    return strin;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        display_usage(argv[0]);
        return EXIT_FAILURE;
    }

    struct stat statbuf;
    if (stat(argv[1], &statbuf) < 0) {
        fprintf(stderr, "Error: Cannot stat '%s'. %s.\n", argv[1],
                strerror(errno));
        return EXIT_FAILURE;
    }

    /* TODO
     * If the argument supplied to this program is not a regular file,
     * print out an error message to standard error and return EXIT_FAILURE.
     * For example:
     * $ ./permstat iamdir
     * Error: 'iamdir' is not a regular file.
     */

    if (!S_ISREG(statbuf.st_mode)){
        fprintf(stderr, "Error: '%s' is not a regular file.\n", argv[1]);
        return EXIT_FAILURE;
    }
    
    char *perms = permission_string(&statbuf);
    printf("Permissions: %s\n", perms);
    free(perms);

    return EXIT_SUCCESS;
}
