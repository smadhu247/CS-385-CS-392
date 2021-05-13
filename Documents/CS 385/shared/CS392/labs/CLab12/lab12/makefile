# /*******************************************************************************
 # Name        : sum makefile
 # Author      : Sanjana Madhu and Lasya Josyula
 # Date        : 05/07/2021
 # Description : Lab 12
 # Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 # ******************************************************************************/

CC     = gcc
CFLAGS = -g -Wall -Werror -pedantic-errors

all: main libsum.so

# TODO: build main
main: main.c
	$(CC) $(CFLAGS) -o main main.c -ldl
# TODO: build libsum.so
libsum.so: sum.c main.c
	$(CC) $(CFLAGS) -shared -fPIC sum.c -o libsum.so

clean:
	rm -f libsum.so main main.exe
