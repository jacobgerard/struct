#ifndef LLIST_H
#define LLIST_H

struct llist{
    size_t elementsize;
    size_t size;
    void (*print)(const void *);
    int (*compare)(const void *, const void *);
    struct lnode *head;
    struct lnode *tail; 
};

struct lnode{
    struct lnode *next;
    struct lnode *prev;
    void *value;
};

struct llist *llist_init(size_t elementsize, void (*print)(const void *));

void *llist_get(struct llist *list, unsigned int position);

void *llist_insert(struct llist *list, unsigned int position, void *valptr);
void *llist_push(struct llist *list, void *valptr);
void *llist_append(struct llist *list, void *valptr);

void llist_remove(struct llist *list, unsigned int position);
void llist_pop(struct llist *list);
void llist_fall(struct llist *list);

struct llist *llist_reverse(struct llist *list);

void llist_print(struct llist *list);
void llist_free(struct llist *list);

#endif //LLIST_H
