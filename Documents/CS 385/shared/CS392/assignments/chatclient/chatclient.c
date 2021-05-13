/*******************************************************************************
 * Name        : chatclient.c
 * Author      : Sanjana Madhu and Lasya Josyula
 * Date        : 05/07/2021
 * Description :
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <limits.h>
#include <sys/select.h>
#include "util.h"

int client_socket = -1;
char username[MAX_NAME_LEN + 1];
char inbuf[BUFLEN + 1];
char outbuf[MAX_MSG_LEN + 1];

int handle_stdin(){
    strcpy(outbuf, inbuf);
    int stringLen = get_string(outbuf, MAX_MSG_LEN);
    if(stringLen == TOO_LONG){
        printf("Sorry, limit your message to %d characters.\n", MAX_MSG_LEN);
    }
    else{
        if(send(client_socket, outbuf, strlen(outbuf),0) < 0){
            fprintf(stderr, "Warning: Failed to receive incoming message. %s.\n", strerror(errno));
        }
        else{
            if (strcmp(outbuf, "bye") == 0){
                printf("Goodbye.\n");
                close(client_socket);
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}

int handle_client_socket(){
    int bytes_recvd = recv(client_socket, inbuf, BUFLEN, 0);
    if(bytes_recvd == -1){
        fprintf(stderr, "Warning: Failed to receive incoming message. %s.\n", strerror(errno));
    }
    else if(bytes_recvd == 0){
        fprintf(stderr, "\nConnection to server has been lost.\n");
        close(client_socket);
        return EXIT_FAILURE;
    }
    else{
        inbuf[bytes_recvd] = '\0';

        if (strcmp(inbuf, "bye") == 0){
            printf("\nServer initiated shutdown.\n");
            return EXIT_FAILURE;
        }
        else{
            printf("\n%s \n", inbuf);
        }
    }
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]){
    int retval;
    // parsing command line arguments

    if(argc != 3){
        fprintf(stderr, "Usage: %s <server IP> <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr;
    socklen_t addressLen = sizeof(struct sockaddr_in);
    memset(&server_addr, 0, addressLen);
    int ip_conversion = inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    if(ip_conversion == 0){
        fprintf(stderr, "Error: Invalid IP address '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }

    //parse_int takes in the input string, integer, and a message
    //if it returns false, means it printed an error so get out of program.
    int i;
    char usageMessage[4096] = "port number";
    if(parse_int(argv[2], &i, usageMessage) == false){
        return EXIT_FAILURE;
    }
    //check if the int  i is in range
    if(i < 1024 || i > 65535){
        fprintf(stderr, "Error: Port must be in range [1024, 65535].\n");
        return EXIT_FAILURE;
    }

    // prompting for a username

    while(true){
        printf("Username: ");
        fflush(stdout);
        int get_string_val = get_string(username, MAX_NAME_LEN+1);
        if(get_string_val == NO_INPUT){
            continue;
        }
        else if(get_string_val == TOO_LONG){
            fprintf(stderr, "Sorry, limit your username to %d characters.\n", MAX_NAME_LEN);
        }
        else{
            //means ok which means that username accepted.
            break;
        }
    }
    retval = EXIT_SUCCESS;
    printf("Hello, %s. Let's try to connect to the server.\n", username);

    // establishing connection

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(i);
    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "Error: Failed to create socket. %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto EXIT;
    }

    if(connect(client_socket, (struct sockaddr *)&server_addr, addressLen) < 0){
        fprintf(stderr, "Error: Failed to connect to server. %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto EXIT;
    }

    int bytes_recvd = recv(client_socket, inbuf, BUFLEN + 1, 0);
    if(bytes_recvd == -1){
        fprintf(stderr, "Error: Failed to receive message from server. %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto EXIT;
    }
    else if (bytes_recvd == 0){
        fprintf(stderr, "All connections are busy. Try again later.\n");
        retval = EXIT_FAILURE;
        goto EXIT;
    }

    inbuf[bytes_recvd] = '\0';

    printf("\n");
    printf("%s", inbuf);
    printf("\n");
    printf("\n");

   fd_set sock;

    if(send(client_socket, username, strlen(username) ,0) < 0){
        fprintf(stderr, "Error: Failed to send username to server. %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto EXIT;
    }

    printf("[%s]: ", username);
    fflush(stdout);

   while (true) {
        FD_ZERO(&sock);
        FD_SET(client_socket, &sock);
        FD_SET(STDIN_FILENO, &sock);

        if (select(client_socket + 1, &sock, NULL, NULL, NULL) < 0  && errno != EINTR) {
            fprintf(stderr, "Error: select() failed. %s.\n", strerror(errno));
            retval = EXIT_FAILURE;
            goto EXIT;
        }

        if (FD_ISSET(client_socket, &sock)) {
            if (handle_client_socket() == EXIT_FAILURE) {
               break;
            }
        }

        if (FD_ISSET(STDIN_FILENO, &sock)) {
           if (handle_stdin() == EXIT_FAILURE) {
              break;
           }
        }

        printf("[%s]: ", username);
        fflush(stdout);
    }

    EXIT:
        if (fcntl(client_socket, F_GETFD) >= 0) {
            close(client_socket);
        }
        return retval;
}
