#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

int count = 0;
int perms[] = {S_IRUSR, S_IWUSR, S_IXUSR,
               S_IRGRP, S_IWGRP, S_IXGRP,
               S_IROTH, S_IWOTH, S_IXOTH};

//verifys that the permission string is valid
bool verify_permission_string(char* perm_string){
    if(strlen(perm_string) != 9) {
        return false;
    }
    if ((perm_string[0] != 'r' && perm_string[0] != '-') || (perm_string[1] != 'w' && perm_string[1] != '-') ||
        (perm_string[2] != 'x' && perm_string[2] != '-') || (perm_string[3] != 'r' && perm_string[3] != '-') || 
        (perm_string[4] != 'w' && perm_string[4] != '-') || (perm_string[5] != 'x' && perm_string[5] != '-') ||
        (perm_string[6] != 'r' && perm_string[6] != '-') || (perm_string[7] != 'w' && perm_string[7] != '-') ||
        (perm_string[8] != 'x' && perm_string[8] != '-')) {
        return false;
    }
    return true;
}

//returns true if the directory is unsafe
bool is_invalid_permissions(char* perm_string){
    int p_count = 0;
    for (int i = 0; i < 9; i++){
        if (perm_string[i] == '-'){
            p_count++;
        }
    }
    if(p_count == 9){
        return true;
    }
    return false;
}

//returns true if the two permission strings match
bool is_matching(char* perm_string, char* file_perms){
    for (int i = 0; i < 9; i++){
        if (perm_string[i] != file_perms[i]){
            return false;
        }
    }
    return true;
}

char* permission_string(struct stat *statbuf) {  
 	char * strin = (char *)malloc((10+1) *sizeof(char));
    int permission_valid;
    
    for (int i = 0; i < 9; i += 3) {
        permission_valid = statbuf->st_mode & perms[i];
        if (permission_valid) {
            strin[i] = 'r';
        } else {
            strin[i] = '-';
        }
        permission_valid = statbuf->st_mode & perms[i+1];
        if (permission_valid) {
            strin[i+1] = 'w';
        } else {
            strin[i+1] = '-';
        }
        permission_valid = statbuf->st_mode & perms[i+2];
        if (permission_valid) {
            strin[i+2] = 'x';
        } else {
            strin[i+2] = '-';
        }
    }
    return strin;
}

int recursive_pass(char *filename, char *permissions) {

    char path[PATH_MAX];
    if (realpath(filename, path) == NULL) {
        fprintf(stderr, "Error: Cannot stat '%s'. %s.\n", filename, strerror(errno));
        return -1;
    }

    DIR *dir;
    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "Error: Cannot open directory '%s'. %s.\n",
                path, strerror(errno));
        return -1;
    }

    struct dirent *entry; 
    struct stat sb;
    char full_filename[PATH_MAX];
    size_t pathlen = 0;

    full_filename[0] = '\0'; 

    if (strcmp(path, "/")) { 
        strncpy(full_filename, path, PATH_MAX);
    }  
    pathlen = strlen(full_filename) + 1;
    full_filename[pathlen - 1] = '/';
    full_filename[pathlen] = '\0';

     while ((entry = readdir(dir)) != NULL) { 
         if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        strncpy(full_filename + pathlen, entry->d_name, PATH_MAX - pathlen);
        
        //changed this from lstat to stat
        if (stat(full_filename, &sb) < 0) {
        	return -1;
        }
          
        //converts the permissions of full_filename to a readable format (ex: rwxrwxrwx)
        char *p_string = permission_string(&sb);
        
        //if the directory does not have valid permissions, then enters here (but i think its being done in dir = opendir(path)
        
        
        //if direcotry, then call recursively on the file
        if (S_ISDIR(sb.st_mode)) {    
        	if (is_invalid_permissions(p_string)){
        		fprintf(stderr, "Error: Cannot open directory '%s'. Permission denied.\n", full_filename);
        		return -1;
       		}
       		else {
       			recursive_pass(full_filename, permissions);
       		}    
        } 
        
        //if not a direcotry, then call recursively on the file
		if (!S_ISDIR(sb.st_mode)) {
			if (is_matching(permissions, p_string)){
				printf("%s\n", full_filename);
            }
		}
		free(p_string);
    }
  
    closedir(dir);
    return 0;
}

int main(int argc, char *argv[]) {

    extern char *optarg;
    extern int optind, opterr, optopt;
    int opt;
    opterr = 0;

    char* filename;
    char* permissions;

    int d_count = 0;
    int h_count = 0;
    int p_count = 0;

    if (argc == 1) {
        printf("Usage: ./pfind -d <directory> -p <permission string> [-h]\n");
        return EXIT_FAILURE;
    }
    
    while ((opt = getopt(argc, argv, ":dhp")) != -1){
        switch (opt){
            case 'd':
                d_count = 1;
                break;
            case 'h':
                h_count = 1;
                break;
            case 'p':
                p_count = 1;
                break;
            case '?':
                printf("Error: Unknown option '-%c' received.\n", optopt);
                return EXIT_FAILURE;
        }
    }

    if(h_count == 1){
        printf("Usage: ./pfind -d <directory> -p <permission string> [-h]\n");
        return EXIT_SUCCESS;
    }

    if(d_count == 1){
        int count = 0;
        while (argv[count]){
            if (strcmp(argv[count], "-d")) {
                filename = argv[optind];
                break;
            }
            count++;
        }
    }

    if(d_count != 1){
        printf("Error: Required argument -d <directory> not found.\n");
        return EXIT_FAILURE;
    }

    if(p_count == 1){
        int count = 0;
        while (argv[count] != NULL){
           if (strcmp(argv[count], "-p")) {
                permissions = argv[optind + 1]; //will it always be -d ___ -p ___? 
                break;
            }
            count++;
        } 
    }

    if(p_count != 1){
        printf("Error: Required argument -p <permissions string> not found.\n");
        return EXIT_FAILURE;
    }
    
    char path[PATH_MAX];
    if (realpath(filename, path) == NULL) {
        fprintf(stderr, "Error: Cannot stat '%s'. %s.\n", filename, strerror(errno));
        return EXIT_FAILURE;
    }
    
    if(!verify_permission_string(permissions)){
        printf("Error: Permissions string '%s' is invalid.\n", permissions);
        return EXIT_FAILURE;
    }

   if (recursive_pass(filename, permissions) == 0) {
      return EXIT_SUCCESS;
   }

   return EXIT_SUCCESS;
}
