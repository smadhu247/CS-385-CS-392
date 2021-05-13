/*******************************************************************************
 * Name         : spfind.c
 * Author       : Sanjana Madhu and Lasya Josyula
 * Date         : 03/31/2021
 * Description  : Sorted pfind
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Usage: ./spfind -d <directory> -p <permissions string> [-h]\n");
        return EXIT_FAILURE;
    }

    int pfind_to_sort[2], sort_to_parent[2];
    if (pipe(pfind_to_sort) < 0) {
        fprintf(stderr, "Error: Cannot create pipe pfind_to_sort. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }
    if (pipe(sort_to_parent) < 0) {
        fprintf(stderr, "Error: Cannot create pipe sort_to_parent. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    pid_t pid[2];

    if ((pid[0] = fork()) == 0) {
        close(pfind_to_sort[0]);
        dup2(pfind_to_sort[1], STDOUT_FILENO);
        close(sort_to_parent[0]);
        close(sort_to_parent[1]);

        if (execv("pfind", argv) < 0) {
            printf("Error: ./pfind failed.\n");
            return EXIT_FAILURE;
        }
    }

    if (pid[0] < 0){
        return EXIT_FAILURE;
    }

    if ((pid[1] = fork()) == 0){
        close(pfind_to_sort[1]);
        dup2(pfind_to_sort[0], STDIN_FILENO);
        close(sort_to_parent[0]);
        dup2(sort_to_parent[1], STDOUT_FILENO);

        if (execlp("sort", "sort", NULL) < 0){
            printf("Error: sort failed.\n");
            return EXIT_FAILURE;
        }
    }

    if (pid[1] < 0) {
        return EXIT_FAILURE;
    }

    close(sort_to_parent[1]);
    dup2(sort_to_parent[0], STDIN_FILENO);
    close(pfind_to_sort[0]);
    close(pfind_to_sort[1]);

    char buffer[4096];
    int match_counter = 0;

    while (1) {
        ssize_t count = read(STDIN_FILENO, buffer, sizeof(buffer));
        if (count == -1) {
            if (errno == EINTR) {
                continue;
            }
            else{
                perror("read()");
                return EXIT_FAILURE;
            }
        }
        else if (count == 0){
            break;
        }
        else {
            for (int i = 0; i < count; i++) {
                if (buffer[i] == '\n') {
                    match_counter++;
                }
            }
            if (write(STDOUT_FILENO, buffer, count) < 0) {
                perror("write()");
                return EXIT_FAILURE;
            }
        }
    }

    close(sort_to_parent[0]);
    
    int status;
    waitpid(pid[0], &status, 0);
    if (WEXITSTATUS(status) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    printf("Total matches: %d\n", match_counter);
    return EXIT_SUCCESS;
}
