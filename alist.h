#ifndef ALIST_H
#define ALIST_H

struct alist{
    size_t elementsize;
    size_t maxsize;
    size_t size;
    void (*print)(const void *);
    int (*compare)(const void *, const void *);
    void *list;
};

struct alist *alist_init(size_t elementsize, size_t maxsize,
                         void (*print)(const void *),
                         int (*compare)(const void *, const void *));

void *alist_get(struct alist *list, unsigned int position);

void *alist_insert(struct alist *list, unsigned int position, void *valptr);
void *alist_push(struct alist *list, void *valptr);
void *alist_append(struct alist *list, void *valptr);

void alist_remove(struct alist *list, unsigned int position);

struct alist *alist_sort(struct alist *list);

void alist_print(struct alist *list);
void alist_free(struct alist *list);

#endif //ALIST_H
