#ifndef AVL_H
#define AVL_H

struct avl{
    size_t elementsize;
    size_t size;
    void (*print)(const void *);
    int (*compare)(const void *, const void *);
    struct avlnode *root;
};

struct avlnode{
    size_t size;
    size_t height;
    struct avlnode *left;
    struct avlnode *right;
    void *value;
};

struct avl *avl_init(size_t elementsize, void (*print)(const void *),
                     int (*compare)(const void *, const void *));

size_t avl_height(struct avl *tree);

struct avlnode *avl_search(struct avl *tree, void *valptr);
struct avl *avl_insert(struct avl *tree, void *valptr);
struct avl *avl_remove(struct avl *tree, void *valptr);

void avl_print(struct avl *tree);
void avl_free(struct avl *tree);

#endif //AVL_H
