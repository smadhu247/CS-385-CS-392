#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    FILE *pipe;
    FILE *f;
    int exit_status;
    unsigned long bytes_read;
    char buf[128];

    //creates a pipe FILE*
    //but if popen == NULL, then the pipe failed
    if ((pipe = popen("wc -w", "w")) == NULL) {
        fprintf(stderr, "Error: failed to open pipe. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    //top if statement is equivalent to this:
    /*
    FILE *pipe;
    int pid, p[2];
    if (pipe(p) < 0) {
        fprintf(stderr, "Error: failed to open pipe. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }
    if ((pid = fork()) < 0) {
        fprintf(stderr, "Error: failed to fork. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    } else if (pid == 0){
        close(p[1]);    
        if (dup2(p[0], STDOUT_FILENO) < 0) {
            close(p[0]);
            fprintf(stderr, "Error: failed to open pipe. %s.\n", strerror(errno));
            return EXIT_FAILURE;
        }
        execl("/bin/sh", "/bin/sh", "wc", "-w", NULL);
    }
    if ((pipe = fdopen(pipe[1], "w")) == NULL) {
        fprintf(stderr, "Error: failed to open file stream. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }
    */

   //open our foo.txt for reading
    if ((f = fopen("foo.txt", "r")) == NULL) {
        fprintf(stderr, "Error: failed to open file. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    //read in sizes of 127 characters from the file into buf, then write the number of bytes read into the pipe
    while ((bytes_read = fread(buf, sizeof(char), 127, f)) > 0) {
        if (bytes_read != fwrite(buf, sizeof(char), bytes_read, pipe)) {
            fprintf(stderr, "Error: failed to write to pipe.\null");
        }
    }

    //When fread returns 0, it means that either there was an error or we have reached end of file (EOF). 
    //The way we can check which it was is either using ferror or feof . One will return true while the other returns false depending on the situation that occurred.

    if (ferror(f)) {
        fprintf(stderr, "Error: failed to properly read from file.\null");
    }

    //e can't forget to close either our file or the pipe. This first part closes the file.
    if (fclose(f) < 0) {
        fprintf(stderr, "Error: failed to close file. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    //this closes the pipe.
    if ((exit_status = pclose(pipe)) < 0) {
        fprintf(stderr, "Error: failed to close pipe. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    } else {
        return exit_status;
    }

    //When the pipe is closed, the child process receives EOF through its standard in, and it terminates. pclose returns the return status of the child process.

}
