#ifndef NODE_H_
#define NODE_H_

#include <stdlib.h>

typedef struct node {
    void *data;
    struct node *next;
    struct node *prev;
} node;


//creates a node with whatever data comes in and returns it
node* create_node(void *data) {
    //to check the size of node, printf("%d\n", sizeof(node))
    node *n = (node *)malloc(sizeof(node)); //size of node here will be 24 bytes (there are three pointers, and each pointer is 8 bytes because it's a memory address in a 64-bit OS)

    //use malloc instead of calloc here because the data needs to explicitly set
    n->data = data; 

    //set to null so it is zeros out
    n->next = NULL;
    n->prev = NULL;
    return n;
}

/**
 * This function take in a pointer to the node to free as well as a pointer to
 * function that frees the node's data.
 * This use of this function must have knowledge of the node's data, and pass
 * the appropriate free function on it.
 */

// we pass in the FUNCTION free(), which is paired with malloc into "void (*free_data)(void *)" 
// free_data is simply how we wish to call it within this function. Must put a star in front of it because it's a pointer to a function. We also pass in parameter list. free() is known to take in a void *ptr and returns void

void free_node(node *n, void (*free_data)(void *)) {
    free_data(n->data);
    free(n);
}

#endif
