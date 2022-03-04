#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alist.h"

void *zero(void *arr, size_t nmemb, size_t size){
    memset(arr, 0, nmemb * size);
    return arr;
}

void *moveleft(void *array, size_t nmemb, size_t size){
    memcpy(array, array + size, nmemb * size);
    return array;
}

void *moveright(void *array, size_t nmemb, size_t size){
    memcpy(array + size, array, nmemb * size);
    return array;
}

struct alist *alist_init(size_t elementsize, size_t maxsize,
                         void (*print)(const void *),
                         int (*compare)(const void *, const void *)){
    struct alist *list = malloc(sizeof(*list));
    if(list == NULL){
        return NULL;
    }
    list->elementsize = elementsize;
    list->maxsize = maxsize;
    list->size = 0;
    list->print = print;
    list->compare = compare;
    list->list = malloc(elementsize * maxsize);
    if(list->list == NULL){
        return NULL;
    }
    return list;
}

struct alist *alist_realloc(struct alist *list){
    list->list = realloc(list->list, list->elementsize * list->maxsize * 2);
    if(list->list == NULL){
        return NULL;
    }
    list->maxsize *= 2;
    return list;
}

void *alist_get(struct alist *list, unsigned int position){
    uint8_t *array = list->list;

    if(position < list->size){
        return &array[position * list->elementsize];
    }
    return NULL;
}

void *alist_insert(struct alist *list, unsigned int position, void *valptr){
    int nmembs;
    uint8_t *array;

    if(list->size >= list->maxsize){
        list = alist_realloc(list);
        if(list == NULL){
            return NULL;
        }
    }

    array = list->list;
    if(position > list->size){
        fprintf(stderr, "ERROR: alist[%d] does not exist\n", position);
        return NULL;
    } else if(position == list->size){
        memcpy(&array[position * list->elementsize], valptr, list->elementsize);
        list->size++;
    } else{
        nmembs = list->size - position;
        moveright(&array[position * list->elementsize], nmembs, list->elementsize);
        memcpy(&array[position * list->elementsize], valptr, list->elementsize);
        list->size++;
    }
    return &array[position * list->elementsize];
}

void *alist_push(struct alist *list, void *valptr){
    return alist_insert(list, 0, valptr);
}

void *alist_append(struct alist *list, void *valptr){
    return alist_insert(list, list->size, valptr);
}

void alist_remove(struct alist *list, unsigned int position){
    int nmembs;
    uint8_t *array = list->list;

    if(position >= list->size){
        fprintf(stderr, "ERROR: alist[%d] does not exist\n", position);
        return;
    } else{
        nmembs = list->size - position - 1;
        moveleft(&array[position * list->elementsize], nmembs, list->elementsize);
    } 
    list->size--;
}

struct alist *alist_sort(struct alist *list){
    qsort(list->list, list->size, list->elementsize, list->compare);    
    return list;
}

void alist_print(struct alist *list){
    uint8_t *array = list->list;

    printf("[");
    for(int i = 0; i < list->size; i++){
        list->print(&array[i * list->elementsize]);
        if(i < (list->size - 1)){
            printf(", ");
        }
    }
    printf("]\n");
}

void alist_free(struct alist *list){
    free(list->list);
    free(list);
}
