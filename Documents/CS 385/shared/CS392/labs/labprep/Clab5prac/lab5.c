#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>


int main() {

	int my_fd; 
	
	int bytes_written;
	char *initial_contents = "this is the contents of myfile\n";
	
	
	//set open() equal to an int
	//open() takes in filename, modes, and permissiosn
	//modes include if the file DNE, create it and writeonly
	//permission include read and write permissions for the owner of the file
	if ((my_fd = open("myfile.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) < 0) {
		fprintf(stderr, "Failed to create file: %s\n", strerror(errno));
		return EXIT_FAILURE;
 	}
 	
 	//set write() equal to a DIFFERENT int
 	//pass in int that we set equal to read() 
 	//write takes in the int we set equal to read(), a char* of what we want to print, length of what we want to print
 	bytes_written = write(my_fd, initial_contents, strlen(initial_contents));
 	
 	//closes the int we set equal to open()
	close(my_fd);
	
	//write() returns the number of bytes successfully written into the array. Returns -1 if failed
	if (bytes_written < 0) {
		fprintf(stderr, "Failed to write to file: %s\n", strerror(errno));
		return EXIT_FAILURE;
 	}
 	
 	
	// remaining code snippets will be put in here
	// we created the file some time ago and don't already know everything about it. Suppose we want to know more information about it. Well, we can get that information with the stat function!
	
	struct stat statbuf;
	
	// note that I had to reference statbuf to fill it. Why is this?
	if (stat("myfile.txt", &statbuf) < 0) {
		fprintf(stderr, "Failed to get information on the file: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	
	//At this point, statbuf is populated with information about the file. 
	
	//What is the file size?
	printf("Size of file: %lu characters\n", statbuf.st_size);	
	//Who is the owner of this file?
	printf("File owner's UID: %u\n", statbuf.st_uid);
	
	//When was the file last acces
	//what is the ownsed? Modified?
	printf("Last access time: %s", ctime(&statbuf.st_atime));
	printf("Last modification time: %s", ctime(&statbuf.st_mtime));
	
	//Lastly, how can I find the permissions of a file? but this just prints out some random numbers
	printf("Permissions of file: %d\n", statbuf.st_mode);
	
	int user_read_permissions =  statbuf.st_mode & S_IRUSR;
	int a = 33152 & 256;
	printf("%d\n", user_read_permissions);
	printf("%d\n", a);
	
	
	int perms[] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
	int permission_valid;
	printf("-"); // this is to print the file type bit, as displayed in `$ ls -l`.
	for (int i = 0; i < 9; i += 3) {
 		permission_valid = statbuf.st_mode & perms[i];
		if (permission_valid) {
			printf("r");
 		} 
 		else {
			printf("-");
 		}
 		permission_valid = statbuf.st_mode & perms[i+1];
		if (permission_valid) {
			printf("w");
 		} 
 		else {
			printf("-");
 		}
 		permission_valid = statbuf.st_mode & perms[i+2];
		if (permission_valid) {
			printf("x");
 		} 
 		else {
			printf("-");
 		}
	}
	printf("\n");
}
