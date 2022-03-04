#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "llist.h"

struct lnode *lnode_init(size_t elementsize, void *valptr){
    size_t total = (2 * sizeof(struct lnode *)) + elementsize;
    struct lnode *node = malloc(total);
    node->next = NULL;
    node->prev = NULL;
    memcpy(&node->value, valptr, elementsize);
    return node;
}

struct llist *llist_init(size_t elementsize, void (*print)(const void *)){
    struct llist *list = malloc(sizeof(*list));
    list->elementsize = elementsize;
    list->size = 0;
    list->print = print;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void *llist_get(struct llist *list, unsigned int position){
    struct lnode *node = list->head;
    for(int i = position; i > 0; i--){
        node = node->next;
    }
    return &(node->value);
}

void *llist_insert(struct llist *list, unsigned int position, void *valptr){
    struct lnode *head;
    struct lnode *newnode;

    if(position > list->size){
        fprintf(stderr, "ERROR: alist[%d] does not exist\n", position);
        return NULL;
    } else if(position == 0){
        newnode = llist_push(list, valptr);
    } else if(position == list->size){
        newnode = llist_append(list, valptr);
    } else{
        head = list->head;
        newnode = lnode_init(list->elementsize, valptr);    
        for(int i = position; i > 1; i--){
            head = head->next;
        }
        newnode->next = head->next;
        newnode->prev = head;
        (head->next)->prev = newnode;
        head->next = newnode;
        list->size++;
    }
    return newnode;
}

void *llist_push(struct llist *list, void *valptr){
    struct lnode *head = list->head;
    struct lnode *newnode = lnode_init(list->elementsize, valptr);

    if(head == NULL){
        list->head = newnode;
        list->tail = newnode;
    } else{
        head->prev = newnode;
        newnode->next = head;
        list->head = newnode;
    }
    list->size++;
    return newnode;
}

void *llist_append(struct llist *list, void *valptr){
    struct lnode *tail = list->tail;
    struct lnode *newnode = lnode_init(list->elementsize, valptr);
    
    if(tail == NULL){
        list->head = newnode;
        list->tail = newnode;
    } else{
        tail->next = newnode;
        newnode->prev = tail;
        list->tail = newnode;
    }
    list->size++;
    return newnode;
}

void llist_remove(struct llist *list, unsigned int position){
    struct lnode *tmp;
    struct lnode *head = list->head;

    if(position >= list->size){
        fprintf(stderr, "ERROR: alist[%d] does not exist\n", position);
        return;
    } else if(position == 0){
        llist_pop(list);       
    } else if(position == (list->size - 1)){
        llist_fall(list);        
    } else{
        for(int i = position; i > 1; i--){
            head = head->next;
        }
        tmp = head->next;
        head->next = tmp->next;
        tmp->next->prev = head;
        free(tmp);
        list->size--;
    }
}

void llist_pop(struct llist *list){
    struct lnode *head = list->head;

    if(list->size == 0){
        fprintf(stderr, "ERROR: llist is empty\n");
        return;
    } else if(list->size == 1){
        free(head);
        list->head = NULL;
        list->tail = NULL;
    } else{
        list->head = head->next;
        free(head);
    }
    list->size--;
}

void llist_fall(struct llist *list){
    struct lnode *tail = list->tail;

    if(list->size == 0){
        fprintf(stderr, "ERROR: llist is empty\n");
        return;
    } else if(list->size == 1){
        free(tail);
        list->head = NULL;
        list->tail = NULL;
    } else{
        list->tail = tail->prev;
        free(tail);
    }
    list->size--;
}

struct llist *llist_reverse(struct llist *list){
    struct lnode *tmp;
    struct lnode *cur = list->head;

    if(list->size > 1){
        while(cur != NULL){
            tmp = cur->next;
            cur->next = cur->prev;
            cur->prev = tmp; 
            cur = tmp;
        }
    }
    tmp = list->head;
    list->head = list->tail;
    list->tail = tmp;
    return list;
}

void llist_print(struct llist *list){
    struct lnode *head = list->head;

    printf("[");
    for(int i = 0; i < list->size; i++){
        list->print(&head->value);
        if(i < (list->size - 1)){
            printf(", ");
        }
        head = head->next;
    }    
    printf("]\n");
}

void llist_free(struct llist *list){
    struct lnode *tmp;
    struct lnode *head = list->head;

    for(int i = 0; i < list->size; i++){
        tmp = head->next;
        free(head);
        head = tmp;
    }
    free(list);
}
