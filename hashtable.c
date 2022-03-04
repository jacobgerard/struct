#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "llist.h"

struct hashtable *hashtable_init(size_t keysize, size_t valsize,
                                 size_t total_buckets,
                                 void (*printkey)(const void *),
                                 void (*printval)(const void *),
                                 unsigned int (*hash)(const void *)){

    struct hashtable *table = malloc(sizeof(*table));
    if(table == NULL){
        return NULL;
    }
    table->keysize = keysize;
    table->valsize = valsize;
    table->total_buckets = total_buckets;
    table->size = 0;
    table->printkey = printkey;
    table->printval = printval;
    table->hash = hash;
    table->buckets = malloc(sizeof(*(table->buckets)) * total_buckets);
    if(table->buckets == NULL){
        return NULL;
    }
    for(int i = 0; i < total_buckets; i++){
        table->buckets[i] = llist_init(sizeof(unsigned int) + keysize + valsize, printkey);
    }
    return table;
}

void *hashtable_search(struct hashtable *table, void *keyptr){
    unsigned int hash = table->hash(keyptr);
    struct llist *bucket = table->buckets[hash % table->total_buckets];
    for(int i = 0; i < bucket->size; i++){
        if(memcmp(&hash, llist_get(bucket, i), sizeof(unsigned int)) == 0){
            return (char *)llist_get(bucket, i) + sizeof(unsigned int) + table->keysize;
        }
    }
    return NULL;
}

void *hashtable_insert(struct hashtable *table, void *keyptr, void *valptr){
    unsigned int hash = table->hash(keyptr);
    char tmp[table->keysize + table->valsize];

    *((unsigned int *)tmp) = hash;
    memcpy(tmp + sizeof(unsigned int), keyptr, table->keysize);
    memcpy(tmp + sizeof(unsigned int) + table->keysize, valptr, table->valsize);
    struct llist *bucket = table->buckets[hash % table->total_buckets];
    table->size++;
    return llist_push(bucket, tmp);
}

void hashtable_remove(struct hashtable *table, void *keyptr){
    unsigned int hash = table->hash(keyptr);
    struct llist *bucket = table->buckets[hash % table->total_buckets];
    for(int i = 0; i < bucket->size; i++){
        if(memcmp(&hash, llist_get(bucket, i), sizeof(unsigned int)) == 0){
            llist_remove(bucket, i);
        }
    }
}

void hashtable_printkeys(struct hashtable *table){
    printf("[\n");
    for(int i = 0; i < table->total_buckets; i++){
        printf(" - bucket[%d] -> ", i);
        printf("[");
        struct lnode *head = table->buckets[i]->head;
        for(int j = 0; j < table->buckets[i]->size; j++){
            table->printkey(((char *)&head->value + sizeof(unsigned int)));
            if(j < (table->buckets[i]->size - 1)){
                printf(", ");
            }
            head = head->next;
        }
        printf("]\n");
    }
    printf("]\n");
}

void hashtable_printvals(struct hashtable *table){
    printf("[\n");
    for(int i = 0; i < table->total_buckets; i++){
        printf(" - bucket[%d] -> ", i);
        printf("[");
        struct lnode *head = table->buckets[i]->head;
        for(int j = 0; j < table->buckets[i]->size; j++){
            table->printval(((char *)&head->value + sizeof(unsigned int) + table->keysize));
            if(j < (table->buckets[i]->size - 1)){
                printf(", ");
            }
            head = head->next;
        }
        printf("]\n");
    }
    printf("]\n");
}

void hashtable_free(struct hashtable *table){
    for(int i = 0; i < table->total_buckets; i++){
        llist_free(table->buckets[i]);
    }
    free(table->buckets);
    free(table);
}
