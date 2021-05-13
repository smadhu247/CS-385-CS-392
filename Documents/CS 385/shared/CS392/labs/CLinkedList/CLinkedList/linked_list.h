#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "node.h"

typedef struct linked_list { //linked list is also 24 bytes. 2 pointers = 16 bytes, size_t = 8 bytes;
    node *head;
    node *tail;
    size_t num_nodes;
} linked_list;

linked_list* create_linked_list() { //acts as a constructor and allocates enough memory
    //calloc zeros out block of memory. All bits will be zero
        //if you were to reference head, or tail it would be null instead of some random memory address, and num_nodes would be zero
    //if we used malloc, the arb. values would be what would be inside head tail and num_nodes, head and tail would ahve non-zero value in memory
    //if we want, we can use malloc and then set head == null, tail == null, etc
    return (linked_list *)calloc(1, sizeof(linked_list)); // one block of memory * (size of linked list)
}

//adds element to the end of the list

void push_back(linked_list *list, node *node) {

    //if head is null, then the head and the tail will point to the same node
    if (list->head == NULL) {
        list->head = list->tail = node;
    } 
    
    //if the head is not null, we add the node we want to insert to the end of the list
    // set the previous of the node to the current tail
    //then move the tail of the list to the node we inserted. 
    //increase num_nodes to continue to keep track of the number of nodes in the list
    else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    list->num_nodes++;
}


void push_front(linked_list *list, node *node) {

    //if head is null, then the head and the tail will point to the same node
    if (list->head == NULL) {
        list->head = list->tail = node;
    } 
    
    //set node to be before the current head
    //the next of the node will be the current head
    //the actual list "head" value must now point to the node
     //increase num_nodes to continue to keep track of the number of nodes in the list
    else {
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
    }
    list->num_nodes++;
}

void *pop_front(linked_list *list) {

    //create a node and point it to the head
    //if the head is null, there's nothing to pop so just return null
    node *cur = list->head;
    if (cur == NULL) {
        return NULL;
    }

    //if the enxt of the head is null, then there's only one element in the list. When this elelment get's popped, the head and tail will both be null in the list
    if (cur->next == NULL) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        //if there is more than one element, the the head of the list moves up one
        //the previous of the head becomes null, and the cur.next becomes null because we need to cut off the link to the rest of the list
        list->head = list->head->next;
        list->head->prev = NULL;
        cur->next = NULL;
    }
     //decrease num_nodes to continue to keep track of the number of nodes in the list
    list->num_nodes--;
    return cur;
}

//takes in fucntion pointer for a print function
void print_list(linked_list *list, void (*print_function)(void *)) {
    putchar('[');

    //start cur at the head of the list
    node *cur = list->head;

    //if cur is not null, print data and advance to the next one in the list
    if (cur != NULL) {
        print_function(cur->data);
        cur = cur->next;
    }

    //for the rest, precede everything with a " ," and print it 
    for ( ; cur != NULL; cur = cur->next) {
        printf(", ");
        print_function(cur->data);
    }
    puts("]");
}


/** 
 * Returns a new linked list with all the elements from the original list that are less than the supplied key
 */

linked_list *find_less(linked_list *list, const void *key, int (*cmp) (const void *, const void *)) {
    //must have * after linked_list because it's a pointer to the linked list
    linked_list *results = NULL;
    for (node *cur = list->head; cur != NULL; cur = cur->next){
        if(cmp(&cur->data, &key) < 0){
            //make a copy of cur bc we can't have it point to the same things
            //if we just put in cur, we'd be attempting to free things that had already been freed -> double free corruption error
            if(!results){
                results = create_linked_list();
            }
            push_back(results, create_node((void *) strdup(cur->data))); //this will fail if results is null
        }
    }
    if (results){
        char **array = (char **)malloc(results->num_nodes * sizeof(char *)), 
        **p;

        p = array; 
        //p must be equal to the head of the array. If we move the head by advancing it, then we lose the head for when we need to copy values back

        //putting all data into array p
        for (node *cur = results->head; cur != NULL; cur = cur->next){
            *p++ = cur->data;
        }
        //qsort on array with num_nodes number of nodes, size of each data type
        //sorts the mem addresses
        qsort(array, results->num_nodes, sizeof(char *), cmp);

        //sets p back to head
        p = array;

        //copies elements of p back to results 
        for (node *cur = results->head; cur != NULL; cur = cur->next){
            cur->data = *p++;
        }

        free(array);
    }
    return results;
}

linked_list *find_greater(linked_list *list, const void *key, int (*cmp) (const void *, const void *)) {
    linked_list *results = NULL;
    for (node *cur = list->head; cur != NULL; cur = cur->next){
        if(cmp(&cur->data, &key) > 0){
            if(!results){
                results = create_linked_list();
            }
            push_back(results, create_node((void *) strdup(cur->data))); //this will fail if results is null
        }
    }
    if (results){
        char **array = (char **)malloc(results->num_nodes * sizeof(char *)), 
        **p;
        p = array;
        for (node *cur = results->head; cur != NULL; cur = cur->next){
            *p++ = cur->data;
        }
        qsort(array, results->num_nodes, sizeof(char *), cmp);
        p = array;
        for (node *cur = results->head; cur != NULL; cur = cur->next){
            cur->data = *p++;
        }
        free(array);
    }
    return results;
}

int find_matching (linked_list *list, const void *key, int (*cmp) (const void *, const void *) ){
    int count = 0;
    for (node* cur = list->head; cur != NULL; cur = cur-> next){
        if(cmp(&cur->data, &key) == 0) {
            count++;
        }
    }
    return count;
}

linked_list* reverse_list(linked_list *list){
    linked_list *results = NULL;
    for(node* cur = list->tail; cur != NULL; cur = cur->prev){
        if(!results){
            results = create_linked_list();
        }
        push_back(results, create_node((void *) strdup(cur->data)));
    }
    return results;
}


//qsort in reverse order
//linked list of ints!


#endif