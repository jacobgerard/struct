#ifndef HASHTABLE_H
#define HASHTABLE_H

struct hashtable{
    size_t keysize;
    size_t valsize;
    size_t total_buckets;
    size_t size;
    void (*printkey)(const void *);
    void (*printval)(const void *);
    unsigned int (*hash)(const void *);
    struct llist **buckets;
};

struct hashtable *hashtable_init(size_t keysize, size_t valsize,
                                 size_t total_buckets,
                                 void (*printkey)(const void *),
                                 void (*printval)(const void *),
                                 unsigned int (*hash)(const void *));

void *hashtable_search(struct hashtable *table, void *keyptr);
void *hashtable_insert(struct hashtable *table, void *keyptr, void *valptr);
void hashtable_remove(struct hashtable *table, void *keyptr);

void hashtable_printkeys(struct hashtable *table);
void hashtable_printvals(struct hashtable *table);
void hashtable_free(struct hashtable *table);

#endif //HASHTABLE_H
