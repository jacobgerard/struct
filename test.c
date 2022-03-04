#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>

#include "struct.h"

void printint(const void *ptr){
    printf("%d", *((int *) ptr));
}

void printstr(const void *ptr){
    printf("%s", (char *)ptr);
}

int compareint(const void *one, const void *two){
    return *((int *)one) - *((int *)two);
}

unsigned int hash(const void *ptr){
    char *str = ((char *)ptr);

    unsigned int hash = 5381;
    int c;
    while((c = *str++) != 0){
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

int main(int argc, char **argv){
    time_t begin = time(NULL);
    struct timeval start, stop;
    gettimeofday(&start, NULL);
    int i;

    // TEST HASHTABLE
    int tmpage;
    char *jake = "jake";
    char *kaile = "kaile";
    char *jason = "jason";
    char *tracy = "tracy";
    char *grant = "grant";
    char *trent = "trent";
    char *kirra = "kirra";
    char *shelly = "shelly";
    char *mike = "mike";
    char *kea = "kea";
    char *nick = "nick";
    struct hashtable *map = hashtable_init(15, sizeof(int), 10, printstr, printint, hash);
    tmpage = 25;
    hashtable_insert(map, jake, &tmpage);
    tmpage = 23;
    hashtable_insert(map, kaile, &tmpage);
    tmpage = 18;
    hashtable_insert(map, kirra, &tmpage);
    tmpage = 55;
    hashtable_insert(map, mike, &tmpage);
    tmpage = 55;
    hashtable_insert(map, shelly, &tmpage);
    tmpage = 52;
    hashtable_insert(map, jason, &tmpage);
    tmpage = 52;
    hashtable_insert(map, tracy, &tmpage);
    tmpage = 21;
    hashtable_insert(map, trent, &tmpage);
    tmpage = 23;
    hashtable_insert(map, grant, &tmpage);
    tmpage = 22;
    hashtable_insert(map, nick, &tmpage);
    tmpage = 32;
    hashtable_insert(map, kea, &tmpage);
    hashtable_printkeys(map);
    hashtable_printvals(map);
    int *ret = hashtable_search(map, "trent");
    if(ret != NULL){
        printint(ret);
        printf("\n");
    } else{
        printf("NO VALUE\n");
    }
    hashtable_remove(map, "trent");
    ret = hashtable_search(map, "trent");
    if(ret != NULL){
        printint(ret);
        printf("\n");
    } else{
        printf("NO VALUE\n");
    }
    hashtable_printkeys(map);
    hashtable_printvals(map);
    hashtable_free(map);

    // TEST BST
    struct bst *bsttree = bst_init(sizeof(int), printint, compareint);
    for(i = 0; i < 100000; i++){
        bst_insert(bsttree, &i);
    }
    printf("ROOTVAL(bsttree) = %d\n", *((int *)&bsttree->root->value));
    bst_print(bsttree);
    i = 0;
    bst_remove(bsttree, &i);
    printf("height(bsttree) = %lu\n", bst_height(bsttree));
    bst_print(bsttree);
    bst_free(bsttree);

    // TEST AVL
    struct avl *avltree = avl_init(sizeof(int), printint, compareint);
    for(i = 0; i < 100000; i++){
        avl_insert(avltree, &i);
    }
    printf("ROOTVAL(avltree) = %d\n", *((int *)&avltree->root->value));
    avl_print(avltree);
    i = 0;
    avl_remove(avltree, &i);
    printf("height(avltree) = %lu\n", avl_height(avltree));
    avl_print(avltree);
    avl_free(avltree);

    time_t end = time(NULL);
    gettimeofday(&stop, NULL);
    printf("TOTAL EXECUTION TIME = %ld\n", end - begin);
    printf("TOTAL EXECUTION TIME = %ld\n", stop.tv_usec - start.tv_usec);

    return 0;
}
