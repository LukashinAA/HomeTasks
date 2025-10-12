#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct tree {
    int key;
    struct tree* left;
    struct tree* right;
};

struct tree* CreateTree(int key) {
    struct tree* node = malloc(sizeof(struct tree));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void insert(struct tree** root, int key) {
    struct tree** tmp = root;
    while ((*tmp) != NULL) {
        if (key < (*tmp)->key) 
            tmp = &((*tmp)->left);   
        else if (key > (*tmp)->key) 
            tmp = &((*tmp)->right);
        else return;
    }
    *tmp = CreateTree(key);
}

struct tree* FindMin(struct tree* root) {
    while (root->left != NULL) 
        root = root->left;
    return root;
}

void delete(struct tree** root, int key) {
    if (*root == NULL) 
        return;
    struct tree* tmp = *root;
    if (key < tmp->key) 
        delete(&(tmp->left),key);
    else if (key > tmp->key) 
        delete(&(tmp->right),key);
    else {
        if (tmp->left == NULL) {
            *root = tmp->right;
            free(tmp);
        }   
        else if (tmp->right == NULL) {
            *root = tmp->left;
            free(tmp);
        }
        else {
            struct tree* min = FindMin(tmp->right);
            tmp->key = min->key;
            delete(&(tmp->right), min->key);
        }
    }
}

int search(struct tree* root, int key) {
    if (root == NULL)
        return 0;
    while (root != NULL) {
        if (key < root->key) 
            root = root->left;
        else if (key > root->key)
            root = root->right;
        else         
            return 1;
    }
        return 0;
}

int main() {
    char operation;
    int x;
    struct tree* root = NULL;
    while (scanf(" %c%d", &operation, &x) == 2) {
        if (operation == '+') 
            insert(&root,x);
        else if (operation == '-')
            delete(&root, x);
        else if (operation == '?') {
            printf("%d ", x);
            if (search(root,x)) 
                printf("yes\n");
            else
                printf("no\n");
        }
    }
}
