#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap* nuevo = (TreeMap*)malloc(sizeof(TreeMap));

    nuevo->lower_than = lower_than;
    nuevo->root = NULL;
    nuevo->current = NULL;

    return nuevo;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode* aux = tree->root;
    TreeNode* padre;

    while(aux != NULL){

        padre = aux;
        tree->current = aux;

        if(tree->lower_than(aux->pair->key,key)==1) aux = aux->right;
        else if(tree->lower_than(key,aux->pair->key)==1) aux = aux->left;
        else return;

    }

    
    TreeNode* nodo = createTreeNode(key, value);
    nodo->parent = padre;

    if(tree->lower_than(padre->pair->key,nodo->pair->key)==1) padre->right = nodo;
    else if(tree->lower_than(nodo->pair->key,padre->pair->key)==1) padre->left = nodo;

    tree->current = nodo;

    

}

TreeNode * minimum(TreeNode * x){
    while(x->left!=NULL){
        x=x->left;
    }

    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if(node->left == NULL && node->right == NULL){ //0 hijos
        if(tree->lower_than(node->pair->key,node->parent->pair->key)==1) node->parent->left = NULL;
        else if(tree->lower_than(node->parent->pair->key,node->pair->key)==1) node->parent->right = NULL;

        free(node);
        return;
    }

    if(node->left == NULL){ //1 hijo
        if(node==node->parent->left) node->parent->left=(node->right);
        if(node==node->parent->right) node->parent->right=(node->right);

        node->right->parent = node->parent;
        free(node);
        return;

    } 

    if(node->right == NULL){
        if(node==node->parent->left) node->parent->left=(node->left);
        if(node==node->parent->right) node->parent->right=(node->left);

        node->left->parent = node->parent;

        free(node);
        return;

    } 

    if(node->left != NULL && node->right != NULL){ //2 hijos
     TreeNode* menor = minimum(node->right);

     node->pair->key = menor->pair->key;
     node->pair->value = menor->pair->value;

     if(menor->left != NULL){
        if(menor==menor->parent->left) menor->parent->left=(menor->right);
        if(menor==menor->parent->right) menor->parent->right=(menor->right);

        menor->right->parent = menor->parent;

        free(menor);
        return;
     }

     if(menor->right != NULL){
        if(menor==menor->parent->left) menor->parent->left=(menor->left);
        if(menor==menor->parent->right) menor->parent->right=(menor->left);

        menor->left->parent = menor->parent;

        free(menor);
        return;
     }

    }


}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* aux = tree->root;
    while (aux != NULL){
        if(tree->lower_than(aux->pair->key,key)==1) aux = aux->right;
        else if(tree->lower_than(key,aux->pair->key)==1) aux = aux->left;
        else{
            tree->current = aux;
            return aux->pair;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode* nodo = tree->root;
    TreeNode* UB_node= tree->root;

    while (nodo != NULL){
        if(tree->lower_than(nodo->pair->key,key)==1){
            nodo = nodo->right;
        }

        else if(tree->lower_than(key,nodo->pair->key)==1){
            nodo = nodo->left;
            UB_node = nodo;

        } 
        
        else{
            return nodo->pair;
        }
    }

    return UB_node->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode* first = tree->root;
    while(first->left != NULL){
        first = first->left;
    }
    return first->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode* nodo = tree->current;
    if(nodo->right != NULL){
        nodo = minimum(nodo->right);
        tree->current = nodo;
        return nodo->pair;
    } 
    
    else{
        TreeNode* padre = tree->current->parent;
        while(padre != NULL){
            if(tree->lower_than(nodo->pair->key,padre->pair->key)==1)
            {  
             tree->current = padre;
             return padre->pair;
            } 
            padre = padre->parent;
        }
    }

    return NULL;
}
