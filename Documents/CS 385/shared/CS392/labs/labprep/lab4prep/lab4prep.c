#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 128


/**
 * This program trivially copies a file from one location to another.
 * Uses stat to determine if the source and destination files are the same, and
 * errors out in that case to prevent erasing the contents of the file.
 */
int main(int argc, char *argv[]) {
   
    //printf("%d\n", open("fileexists.txt", O_RDONLY)); //returns 3 => success
    //printf("%d\n", open("this_file_doesnt_exist.txt", O_RDONLY)); //returns -1 => fail
    //printf("%d\n", open("make_a_file.txt", O_RDONLY | O_CREAT)); //returns 4 => success
    //printf("%d\n", open("make_a_file1.txt", O_RDWR | O_CREAT)); //returns 5 => success
    //printf("%d\n", open("make_a_file2.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)); //returns 6 => success
    //printf("%d\n", open("somefile.txt", O_RDWR | O_TRUNC)); //returns 7 => success

    int fd = open("fileexists.txt", O_RDWR);
    //contains "the meaning of life is 42"
    char buf[128];
    memset(buf, '\0', 128); //buffer contains all zeros

    read(fd, buf, 4);
    printf("'%s'\n", buf); //prints out "the "

    read(fd, buf, 8);
    printf("'%s'\n", buf); //prints out "meaning "

    //write(fd, "of dogs", 7); //changes life to of dogs
    
    lseek(fd, 0, SEEK_SET);
    write(fd, "a ",3);
    
    close(fd);
    
    struct stat statbuf;
    printf("%d\n", stat("file_exists.txt", &statbuf)); //returns 0
    printf("%d\n",stat("file_doesnt_exist.txt", &statbuf)); //returns -1
    
    
    int fd_a, fd_b;
    fd_a = open("somefile.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    write(fd_a, "somestring", strlen("somestring"));
    fd_b = open("somefile.txt", O_RDWR | O_TRUNC);
    
    /* I intended to leave fd_a here */
    write(fd_a, "someotherstring", strlen("someotherstring"));
    close(fd_a);
    close(fd_b);

    return EXIT_SUCCESS;
}
