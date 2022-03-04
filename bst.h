#ifndef BST_H
#define BST_H

struct bst{
    size_t elementsize;
    size_t size;
    void (*print)(const void *);
    int (*compare)(const void *, const void *);
    struct bstnode *root;
};

struct bstnode{
    size_t size;
    size_t height;
    struct bstnode *left;
    struct bstnode *right;
    void *value;
};

struct bst *bst_init(size_t elementsize, void (*print)(const void *),
                     int (*compare)(const void *, const void *));

size_t bst_height(struct bst *tree);

struct bstnode *bst_search(struct bst *tree, void *valptr);
struct bst *bst_insert(struct bst *tree, void *valptr);
struct bst *bst_remove(struct bst *tree, void *valptr);

void bst_print(struct bst *tree);
void bst_free(struct bst *tree);

#endif //BST_H
