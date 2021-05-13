#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"

void print_str(void *str) {
    printf("\"%s\"", (char *)str);
}

void insert_word(linked_list *list, char *word, bool at_end) {
    if (at_end) {

        //when we create a node, take a duplicate of the data because we want a COPY of the data in the linked list
        //create a node with the copy of the data that goes into it. create_node returns a node*, so we put in a node* into the linked list
        push_back(list, create_node((void *)strdup(word)));
    } else {
        push_front(list, create_node((void *)strdup(word)));
    }
}

int str_cmp(const void *a, const void *b){
    return strcmp(*(const char **)a, *(const char **)b);
}

int main() {
    linked_list *list = create_linked_list();
    // insert_word(list, "hello", true);
    // insert_word(list, "CS392", true);
    // insert_word(list, "Dr. B", false);


    insert_word(list, "apples", true);
    insert_word(list, "oranges", true);
    insert_word(list, "zebras", false);
    insert_word(list, "horses", false);
    insert_word(list, "horses", false);
    insert_word(list, "cats", false);
    insert_word(list, "sharks", false);

    puts("Original list:");
    print_list(list, print_str);

    linked_list *less = find_less(list, "horses", str_cmp);
    puts("Less Search results:");
    print_list(less, print_str);

    linked_list *greater = find_greater(list, "horses", str_cmp);
    puts("Greater Search results:");
    print_list(greater, print_str);

    linked_list *reverse = reverse_list(list);
    puts("Reversed List");
    print_list(reverse, print_str);

    char* cmp = "horses";
    int count_same_values = find_matching(list, cmp, str_cmp);
    printf("The number match values for %s is: %d\n",cmp, count_same_values);


    while (list->num_nodes > 0) {
        node *front = pop_front(list);
        printf("Popped value: %s\n", (char *)front->data);
        free_node(front, free);
    }
    free(list);

    //free up values in less
    if (less){ //checks that there is data in less
        while (less->num_nodes > 0) {
           free_node(pop_front(less), free);
        }
        free(less);
    }

    if (greater){ //checks that there is data in less
        while (greater->num_nodes > 0) {
           free_node(pop_front(greater), free);
        }
        free(greater);
    }

    if(reverse){
        while (reverse->num_nodes > 0) {
           free_node(pop_front(reverse), free);
        }
        free(reverse);
    }

    return 0;
}
