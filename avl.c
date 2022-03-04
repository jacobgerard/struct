#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"

struct avlnode *avlnode_init(size_t elementsize, void *valptr){
    size_t total = (2 * sizeof(size_t)) + (2 * sizeof(struct avlnode *)) + elementsize;
    struct avlnode *node = malloc(total);
    node->size = 1;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;
    memcpy(&node->value, valptr, elementsize); 
    return node;
}

size_t avlnode_height(struct avlnode *node){
    if(node == NULL){
        return 0;
    }
    return node->height;
}

size_t avlnode_subheight(struct avlnode *node){
    size_t lheight = avlnode_height(node->left);
    size_t rheight = avlnode_height(node->right);
    if(lheight > rheight){
        return lheight;
    } else{
        return rheight;
    }
}

size_t avlnode_size(struct avlnode *node){
    if(node == NULL){
        return 0;
    }
    return node->size;
}

size_t avlnode_subsize(struct avlnode *node){
    size_t lsize = avlnode_size(node->left);
    size_t rsize = avlnode_size(node->right);
    if(lsize > rsize){
        return lsize;
    } else{
        return rsize;
    }
}

int avlnode_bfactor(struct avlnode *root){
    return avlnode_height(root->left) - avlnode_height(root->right);
}

void avlnode_rotate_rr(struct avlnode **rootptr){
    struct avlnode *tmp = *rootptr;

    *rootptr = (*rootptr)->right;
    tmp->right = (*rootptr)->left;
    tmp->height = 1 + avlnode_subheight(tmp);
    tmp->size = 1 + avlnode_subsize(tmp);
    (*rootptr)->left = tmp;
    (*rootptr)->height = 1 + avlnode_subheight(*rootptr);
    (*rootptr)->size = 1 + avlnode_subsize(*rootptr);
}

void avlnode_rotate_ll(struct avlnode **rootptr){
    struct avlnode *tmp = *rootptr;

    *rootptr = (*rootptr)->left;
    tmp->left = (*rootptr)->right;
    tmp->height = 1 + avlnode_subheight(tmp);
    tmp->size = 1 + avlnode_subsize(tmp);
    (*rootptr)->right = tmp;
    (*rootptr)->height = 1 + avlnode_subheight(*rootptr);
    (*rootptr)->size = 1 + avlnode_subsize(*rootptr);
}

void avlnode_rotate_lr(struct avlnode **rootptr){
    avlnode_rotate_rr(&((*rootptr)->left));
    avlnode_rotate_ll(rootptr);
}

void avlnode_rotate_rl(struct avlnode **rootptr){
    avlnode_rotate_ll(&((*rootptr)->right));
    avlnode_rotate_rr(rootptr);
}

struct avlnode *avlnode_rebalance(struct avlnode **rootptr){
    int bf = avlnode_bfactor(*rootptr);
    if(bf < -1){
        bf = avlnode_bfactor((*rootptr)->right);
        if(bf == -1){
            avlnode_rotate_rr(rootptr);
        } else if(bf == 1){
            avlnode_rotate_rl(rootptr);
        }
    } else if(bf > 1){
        bf = avlnode_bfactor((*rootptr)->left);
        if(bf == -1){
            avlnode_rotate_lr(rootptr);
        } else if(bf == 1){
            avlnode_rotate_ll(rootptr);
        }
    }
    return *rootptr;
}

struct avlnode *avlnode_minimum(struct avlnode *node){
    if(node == NULL){
        return NULL;
    }
    if(node->left == NULL){
        return node;
    }
    return avlnode_minimum(node->left);
}

struct avlnode *avlnode_maximum(struct avlnode *node){
    if(node == NULL){
        return NULL;
    }
    if(node->right == NULL){
        return node;
    }
    return avlnode_maximum(node->right);
}

struct avlnode *avlnode_insert(struct avlnode **rootp, struct avlnode *node,
                               int (*compare)(const void *, const void *)){

    int cmp;
    struct avlnode *root = *rootp;

    if(root == NULL){
        return node;
    }
    cmp = compare(&node->value, &root->value);
    if(cmp < 0){
        root->left = avlnode_insert(&root->left, node, compare);
    } else{
        root->right = avlnode_insert(&root->right, node, compare);
    }
    root->size++;
    root->height = 1 + avlnode_subheight(root);
    *rootp = root;
    *rootp = avlnode_rebalance(rootp);
    return *rootp;
}

struct avlnode *avlnode_remove(struct avlnode **rootp, void *valptr,
                               size_t elementsize,
                               int (*compare)(const void *, const void *)){

    int cmp;
    struct avlnode *root;
    struct avlnode *tmp;

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
            tmp = avlnode_minimum(root->right);
            memcpy(&root->value, &tmp->value, elementsize);
            root->right = avlnode_remove(&root->right, &tmp->value, elementsize, compare);
        }
    } else if(cmp < 0){
        root->left = avlnode_remove(&root->left, valptr, elementsize, compare);
    } else{
        root->right = avlnode_remove(&root->right, valptr, elementsize, compare);
    }
    root->size = 1 + avlnode_subsize(root);
    root->height = 1 + avlnode_subheight(root);
    *rootp = root;
    *rootp = avlnode_rebalance(rootp);
    return *rootp;
}

void avlnode_print(struct avlnode *root, void (*printer)(const void *)){
    if(root == NULL){
        return;
    }
    avlnode_print(root->left, printer);
    printer(&root->value);
    printf(" ");
    avlnode_print(root->right, printer);
}

void avlnode_free(struct avlnode *node){
    if(node == NULL){
        return;
    }
    avlnode_free(node->left);
    avlnode_free(node->right);
    free(node);
}

struct avl *avl_init(size_t elementsize, void (*print)(const void *),
                     int (*compare)(const void *, const void *)){

    struct avl *tree = malloc(sizeof(*tree));
    tree->elementsize = elementsize;
    tree->size = 0;
    tree->print = print;
    tree->compare = compare;
    tree->root = NULL;
    return tree;
}

size_t avl_height(struct avl *tree){
    return avlnode_height(tree->root);
}

struct avlnode *avl_search(struct avl *tree, void *valptr){
    int cmp;
    struct avlnode *cur = tree->root;
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

struct avl *avl_insert(struct avl *tree, void *valptr){
    struct avlnode *newnode = avlnode_init(tree->elementsize, valptr);
    tree->root = avlnode_insert(&tree->root, newnode, tree->compare);
    tree->size = tree->root->size;
    return tree;
}

struct avl *avl_remove(struct avl *tree, void *valptr){
    tree->root = avlnode_remove(&(tree->root), valptr, tree->elementsize, tree->compare);
    tree->size = tree->root->size;
    return tree;
}

void avl_print(struct avl *tree){
    printf("[ ");
    avlnode_print(tree->root, tree->print);
    printf("]\n");
}

void avl_free(struct avl *tree){
    avlnode_free(tree->root);
    free(tree);    
}
