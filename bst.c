#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bst.h"

struct bstnode *bstnode_init(size_t elementsize, void *valptr){
    size_t total = (2 * sizeof(size_t)) + (2 * sizeof(struct bstnode *)) + elementsize;
    struct bstnode *node = malloc(total);
    node->size = 1;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;
    memcpy(&node->value, valptr, elementsize); 
    return node;
}

size_t bstnode_height(struct bstnode *node){
    if(node == NULL){
        return 0;
    }
    return node->height;
}

size_t bstnode_subheight(struct bstnode *node){
    size_t lheight = bstnode_height(node->left);
    size_t rheight = bstnode_height(node->right);
    if(lheight > rheight){
        return lheight;
    } else{
        return rheight;
    }
}

size_t bstnode_size(struct bstnode *node){
    if(node == NULL){
        return 0;
    }
    return node->size;
}

size_t bstnode_subsize(struct bstnode *node){
    size_t lsize = bstnode_size(node->left);
    size_t rsize = bstnode_size(node->right);
    if(lsize > rsize){
        return lsize;
    } else{
        return rsize;
    }
}
struct bstnode *bstnode_minimum(struct bstnode *node){
    if(node == NULL){
        return NULL;
    }
    if(node->left == NULL){
        return node;
    }
    return bstnode_minimum(node->left);
}

struct bstnode *bstnode_maximum(struct bstnode *node){
    if(node == NULL){
        return NULL;
    }
    if(node->right == NULL){
        return node;
    }
    return bstnode_maximum(node->right);
}

struct bstnode *bstnode_insert(struct bstnode **rootp, struct bstnode *node,
                               int (*compare)(const void *, const void *)){

    int cmp;
    struct bstnode *root = *rootp;

    if(root == NULL){
        return node;
    }
    cmp = compare(&node->value, &root->value);
    if(cmp < 0){
        root->left = bstnode_insert(&root->left, node, compare);
    } else{
        root->right = bstnode_insert(&root->right, node, compare);
    }
    root->size++;
    root->height = 1 + bstnode_subheight(root);
    *rootp = root;
    return root;
}

struct bstnode *bstnode_remove(struct bstnode **rootp, void *valptr,
                               size_t elementsize,
                               int (*compare)(const void *, const void *)){

    int cmp;
    struct bstnode *root;
    struct bstnode *tmp;

    if((rootp == NULL) || (*rootp == NULL)){
        return NULL;
    }
    root = *rootp;

    cmp = compare(valptr, &root->value);
    if(cmp == 0){
        if((root->left == NULL) && (root->right == NULL)){
            free(root);
            return NULL;
        } else if(root->left == NULL){
            tmp = root->right;
            free(root);
            return tmp;
        } else if(root->right == NULL){
            tmp = root->left;
            free(root);
            return tmp;
        } else{
            // IF TWO CHILDREN
            tmp = bstnode_minimum(root->right);
            memcpy(&root->value, &tmp->value, elementsize);
            root->right = bstnode_remove(&root->right, &tmp->value, elementsize, compare);
        }
    } else if(cmp < 0){
        root->left = bstnode_remove(&root->left, valptr, elementsize, compare);
    } else{
        root->right = bstnode_remove(&root->right, valptr, elementsize, compare);
    }
    root->size = 1 + bstnode_subsize(root);
    root->height = 1 + bstnode_subheight(root);
    *rootp = root;
    return root;
}

void bstnode_print(struct bstnode *root, void (*printer)(const void *)){
    if(root == NULL){
        return;
    }
    bstnode_print(root->left, printer);
    printer(&root->value);
    printf(" ");
    bstnode_print(root->right, printer);
}

void bstnode_free(struct bstnode *node){
    if(node == NULL){
        return;
    }
    bstnode_free(node->left);
    bstnode_free(node->right);
    free(node);
}

struct bst *bst_init(size_t elementsize, void (*print)(const void *),
                     int (*compare)(const void *, const void *)){

    struct bst *tree = malloc(sizeof(*tree));
    tree->elementsize = elementsize;
    tree->size = 0;
    tree->print = print;
    tree->compare = compare;
    tree->root = NULL;
    return tree;
}

size_t bst_height(struct bst *tree){
    return bstnode_height(tree->root);
}

struct bstnode *bst_search(struct bst *tree, void *valptr){
    int cmp;
    struct bstnode *cur = tree->root;
    while(cur != NULL){
        cmp = tree->compare(valptr, &cur->value);
        if(cmp == 0){
            return cur; 
        } else if(cmp < 0){
            cur = cur->left;
        } else{
            cur = cur->right;
        }
    }
    return NULL;
}

struct bst *bst_insert(struct bst *tree, void *valptr){
    struct bstnode *newnode = bstnode_init(tree->elementsize, valptr);
    tree->root = bstnode_insert(&tree->root, newnode, tree->compare);
    tree->size = tree->root->size;
    return tree;
}

struct bst *bst_remove(struct bst *tree, void *valptr){
    tree->root = bstnode_remove(&(tree->root), valptr, tree->elementsize, tree->compare);
    tree->size = tree->root->size;
    return tree;
}

void bst_print(struct bst *tree){
    printf("[ ");
    bstnode_print(tree->root, tree->print);
    printf("]\n");
}

void bst_free(struct bst *tree){
    bstnode_free(tree->root);
    free(tree);    
}
