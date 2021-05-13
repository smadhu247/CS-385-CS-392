# /*******************************************************************************
 # Name        : quicksort.h
 # Author      : Sanjana Madhu and Lasya Josyula
 # Date        : 03/02/2021
 # Description : Makefile
 # Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 # ******************************************************************************/

sort: quicksort.o sort.o
	gcc quicksort.o sort.o -o sort
quicksort.o: quicksort.c quicksort.h
	gcc -Wall -Werror -g -pedantic-errors -c quicksort.c
sort.o: sort.c quicksort.h
	gcc -Wall -Werror -g -pedantic-errors -c sort.c
clean:
	rm -f *.o sort sort.exe