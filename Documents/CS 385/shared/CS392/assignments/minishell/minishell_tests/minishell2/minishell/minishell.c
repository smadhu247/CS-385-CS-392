/*******************************************************************************
 * Name        :minishell.c
 * Author      : Sanjana Madhu and Lasya Josyula
 * Date        : 04/13/2021
 * Description :
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <setjmp.h>
#include <signal.h>
#define BRIGHTBLUE "\x1b[34;1m"
#define DEFAULT "\x1b[0m"


volatile sig_atomic_t interrupted;

void catch_signal(int sig) {
    interrupted = true;
    write(STDOUT_FILENO, "\n", 1);
}

int main(int argc, char *argv[]) {

    struct sigaction action;

    //zero out everything in memory
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = catch_signal;

    if(sigaction(SIGINT, &action, NULL) == -1) {
        fprintf(stderr, "Error: Cannot register signal handler. %s.\n",strerror(errno));
        return EXIT_FAILURE;
    }

    if(sigaction(SIGTERM, &action, NULL) == -1) {
        fprintf(stderr, "Error: Cannot register signal handler. %s.\n",strerror(errno));
        return EXIT_FAILURE;
    }
    
    while(true) {
        char *args[2048];
        int j = 0; //counter for while loop
        interrupted = false;

        char current_dir[PATH_MAX];
        char buf[PATH_MAX];
        size_t bytes_read;
        if (getcwd(current_dir, sizeof(current_dir)) != NULL) {
            printf("[%s%s%s]$ ", BRIGHTBLUE, current_dir, DEFAULT);
            fflush(stdout);
        }

        else{
            fprintf(stderr,"Error: Cannot get current working directory. %s.\n", strerror(errno));
            return EXIT_FAILURE;
        }
    
        if ((bytes_read = read(STDIN_FILENO, buf, sizeof(buf)-1)) < 0){
            if (!interrupted){
                fprintf(stderr,"Error: Failed to read from stdin. %s.\n", strerror(errno));
                return EXIT_FAILURE;
            }
            else{
                continue;
            }
        }

        else if (bytes_read == 0) {
            break;
        }
  
        if(interrupted){
            continue;
        }
        
        buf[bytes_read-1] = '\0';
        char *val; //inputs each value in buf into char *val
        val = (char *)strtok(buf, " ");
        
        while(val != NULL){
            args[j] = strdup(val);
            j++;
            val = (char *)strtok(NULL, " ");
        }
        args[j] = NULL;
        
   
    //cd
        if (args[0] != NULL && (strcmp(args[0], "cd")) == 0){
            if(j > 2){
                if (args[1][0] == '\"'){
                    int quotes = 0;
                    int letters = 0;
                    int firstWord = 0;
                    for (int k = 0; k < j; k++){
                        for (int i = 0; i < strlen(args[k]); i++){
                            if (args[k][i] == '\"'){
                                quotes++;
                            }
                        }
                    }
                    
                    for (int k = 1; k < j; k++){
                        for (int i = 0; i < strlen(args[k]); i++){
                            if(args[k][i] != '\"' && args[k][i] != ' '){
                                letters++;
                            }
                        }
                    }
                    
                    for (int k = 1; k < strlen(args[1]); k++){
                        if (args[1][k] == '\"'){
                            break;
                        }
                        firstWord++;
                    }
                    
                    char str[] = "";
                    bool error = false;
                    bool error1 = false;
                    for (int k = 1; k < j; k++){
                        if (k < j-1){
                            if (args[k][strlen(args[k])-1] == '\"' && args[k+1][0] == '\"' && letters > firstWord){
                                error = true;
                                fprintf(stderr, "Error: Too many arguments to cd.\n");
                            }
                        }
                        for (int i = 0; i <= strlen(args[k]); i++){
                            if (args[k][i] == '\"' && quotes == 1){
                                quotes--;
                                if(quotes == 0 && letters > 0){
                                    error1 = true;
                                    fprintf(stderr, "Error: Malformed command.\n");
                                }
                                break;
                            }
                            else if (args[k][i] == '\"'){
                                quotes--;
                                continue;
                            }
                            else{
                                letters--;
                                strncat(str, &args[k][i], 1);
                            }
                        }
                        letters++;
                        if(quotes!=0 && letters!=0){
                            strcat(str, " ");
                        }
                        else{
                            break;
                        }
                    }
                    if (!error1 && !error && chdir(str) == -1) {
                        fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", str, strerror(errno));
                    }
                }
                else{
                    fprintf(stderr, "Error: Too many arguments to cd.\n");
                }
            }
            if (args[1] == NULL || (strcmp(args[1], "~") == 0)){
                struct passwd *pwd = getpwuid(getuid());
                if (pwd == NULL){
                    fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
                }
                char *homedir = pwd->pw_dir;
                if (chdir(homedir) == -1) {
                    fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", args[1], strerror(errno));
                }
            }
            else if (strncmp(args[1], "~/", 2) == 0){
                struct passwd *pwd = getpwuid(getuid());
                if (pwd == NULL){
                    fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
                }
                char *homedir = pwd->pw_dir;
                char *directories;
                //looks for first occurrence of ~/
                directories = (strstr(args[1], "~/"));
                //adds 2 so that you get only the directories after ~/
                directories += 2;
                //now add / to after /home/user
                strcat(homedir, "/");
                //add the directories to /home/user/...
                strcat(homedir, directories);
                if(chdir(homedir) == -1){
                    fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", args[1], strerror(errno));
                }
            }
            else {
                if (j == 2) {
                    if (args[1][0] == '\"'){
                        int quotes = 0;
                        int letters = 0;
                        int firstWord = 0;
                        for (int k = 0; k < j; k++){
                            for (int i = 0; i < strlen(args[k]); i++){
                                if (args[k][i] == '\"'){
                                    quotes++;
                                }
                            }
                        }
                        
                        for (int k = 1; k < j; k++){
                            for (int i = 0; i < strlen(args[k]); i++){
                                if(args[k][i] != '\"' && args[k][i] != ' '){
                                    letters++;
                                }
                            }
                        }
                        
                        
                        char str[] = "";
                        bool error = false;
                        bool error1 = false;
                        for (int k = 1; k < j; k++){
                            if (k < j-1){
                                if (args[k][strlen(args[k])-1] == '\"' && args[k+1][0] == '\"' && letters > firstWord){
                                    error = true;
                                    fprintf(stderr, "Error: Too many arguments to cd.\n");
                                }
                            }
                            for (int i = 0; i <= strlen(args[k]); i++){
                                if (args[k][i] == '\"' && quotes == 1){
                                    quotes--;
                                    if(quotes == 0 && letters > 0){
                                        error1 = true;
                                        fprintf(stderr, "Error: Malformed command.\n");
                                    }
                                    break;
                                }
                                else if (args[k][i] == '\"'){
                                    quotes--;
                                    continue;
                                }
                                else{
                                    letters--;
                                    strncat(str, &args[k][i], 1);
                                }
                            }
                            letters++;
                            if(quotes!=0 && letters!=0){
                                strcat(str, " ");
                            }
                            else{
                                break;
                            }
                        }
                        if (!error1 && !error && chdir(str) == -1) {
                            fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", str, strerror(errno));
                        }
                    }
                    else{
                        if (chdir(args[1]) == -1) {
                            fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", args[1], strerror(errno));
                        }
                    }
                }
                for (int i = 0; i < j; i++){
                    free(args[i]);
                }
                continue;
            }
        }

        //exit
        else if (args[0] != NULL && (strcmp(args[0], "exit")) == 0){
            for (int i = 0; i < j; i++){
                free(args[i]);
            }
            return EXIT_SUCCESS;
        }

         // works on all other commands
        else{
            pid_t pid = fork();
            if (pid < 0) {
                fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
                for (int i = 0; i < j; i++){
                    free(args[i]);
                }
            }
            else if (pid == 0) {
                if (execvp(args[0], args) == -1){
                    fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
                    for (int i = 0; i < j; i++){
                        free(args[i]);
                    }
                    exit(EXIT_FAILURE);
                }
                for (int i = 0; i < j; i++){
                    free(args[i]);
                }
                exit(EXIT_FAILURE);
            }
            else {
                int stat;
                while((pid = wait(&stat)) == -1 && errno == EINTR){
                    continue;
                }
                if (pid < 0) {
                    fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
                    for (int i = 0; i < j; i++){
                        free(args[i]);
                    }
                    return EXIT_FAILURE;
                }
            }
        }

        for (int i = 0; i < j; i++){
            free(args[i]);
        }
    }
    return EXIT_SUCCESS;
}
