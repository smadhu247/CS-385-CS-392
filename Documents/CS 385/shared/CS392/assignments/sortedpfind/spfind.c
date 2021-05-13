#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

//properly sorting?
//how to get total matches not show up?
//error check read()? or anything else?

int main(int argc, char *argv[]) {

  int pfind_to_sort[2], sort_to_parent[2];
  if (pipe(pfind_to_sort) < 0) {
      fprintf(stderr, "Error: Cannot create pipe pfind_to_sort. %s.\n",
              strerror(errno));
      return EXIT_FAILURE;
  }
  if (pipe(sort_to_parent) < 0) {
      fprintf(stderr, "Error: Cannot create pipe sort_to_parent. %s.\n",
              strerror(errno));
      return EXIT_FAILURE;
  }

  pid_t pid[2];

  if ((pid[0] = fork()) == 0) {
      // process is in child -> pfind
      close(pfind_to_sort[0]);
      dup2(pfind_to_sort[1], STDOUT_FILENO);

      // Close all unrelated file descriptors.
      close(sort_to_parent[0]);
      close(sort_to_parent[1]);

      if (execv("pfind", argv) < 0) {
          fprintf(stderr, "Error: pfind failed. %s.\n", strerror(errno));
          return EXIT_FAILURE;
      }
  }

  if(pid[0] < 0){
      return EXIT_FAILURE;
  }

  if ((pid[1] = fork()) == 0) {
      // process is in child -> sort
      close(pfind_to_sort[1]);
      dup2(pfind_to_sort[0], STDIN_FILENO);
      close(sort_to_parent[0]);
      dup2(sort_to_parent[1], STDOUT_FILENO);

      if (execlp("sort", "sort", NULL) < 0) {
          fprintf(stderr, "Error: sort failed. %s.\n", strerror(errno));
          return EXIT_FAILURE;
      }
  }

  if(pid[1] < 0){
      return EXIT_FAILURE;
  }

  close(sort_to_parent[1]);
  // This step is not required. You could read directly from sort_to_parent[0]
  // instead of from STDIN_FILENO.
  dup2(sort_to_parent[0], STDIN_FILENO);

  // Close all unrelated file descriptors.
  close(pfind_to_sort[0]);
  close(pfind_to_sort[1]);

  char buffer[4096];
  int match_counter = 0;
  while (1) {
      ssize_t count = read(STDIN_FILENO, buffer, sizeof(buffer)); //error check read
      
      if (count == -1) {
          if (errno == EINTR) {
              continue;
          } else {
              perror("read()");
              exit(EXIT_FAILURE);
          }
      } else if (count == 0) {
          break;
      } else {
        for (int i = 0; i < count; i++){
            if (buffer[i] == '\n'){
                match_counter++;
            }
        }
        if (write(STDOUT_FILENO, buffer, count) < 0){
             perror("write()");
             exit(EXIT_FAILURE);
        }
      }
  }
  close(sort_to_parent[0]);
  // Don't forget to wait for both children.
  int status = 0;
  waitpid(pfind, &status, 0);
  waitpid(sort, &status, 0);
    if (WEXITSTATUS(status) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    printf("Total matches: %d\n", match_counter); 
  return EXIT_SUCCESS;
}
