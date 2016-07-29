// protoTree.c                                      
// An implementation of a 2016 WEPL balancing tree
// Used Stan Eisenstat's model protoTree.c
// 
// Allen Wang
//
// 4/20/16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "protoTree.h"

static int IF = 0;

struct tnode {
    Key key;  
    int weight; 
    int WEPL;                  // Each Tree node contains a KEY and
    Tree left, right;             // ... has a LEFT and a RIGHT subtree
} tnode;
// Set *T to the empty tree.  Return TRUE if successful, else FALSE.
bool create (Tree *t)
{
    *t = NULL;
    return true;
}
void setIF(int newval){
    IF = newval;
}
int changewepl(Tree t)
{
    int wepl;
    wepl = t->weight + (t->right)->WEPL + (t->left)->WEPL;
    return wepl;
}
// Set *T to a one-node tree containing Key K.  Return TRUE if successful,
// FALSE otherwise.
Tree makeNode (Tree *t, Key k) //only used for making leaf nodes.
{
    Tree temp = malloc(sizeof(tnode));
    char * newkey = malloc(sizeof(char) * (strlen(k) + 1));
    strcpy(newkey, k);
    temp->key = newkey;
    temp->left = NULL;
    temp->right = NULL;
    temp->weight = 1;
    temp->WEPL = 0;
    *t = temp;
    return temp;
}

Tree rotateTree (Tree *t, int direction)// 0 = rotating to the left, 1 = rotating to the right
{
    if (direction == 1) //rotating to the right
    {
        Tree B = (*t)->left;
        Tree D = *t;
        Tree C = ((*t)->left)->right;
        Tree A = B->left;
        Tree E = D->right;

        D->left = C;
        D->weight = C->weight + E->weight;
        D->WEPL = changewepl(D);
        B->right = D;
        B->weight = A->weight + D->weight;
        B->WEPL = changewepl(B);
        (*t) = B;
        return *t;
    }
    else //rotating to the left
    {
        Tree D = (*t)->right;
        Tree B = *t;
        Tree C = ((*t)->right)->left;
        Tree A = B->left;
        Tree E = D->right;

        B->right = C;
        B->weight = C->weight + A->weight;
        B->WEPL = changewepl(B);
        D->left = B;
        D->weight = B->weight + E->weight;
        D->WEPL = changewepl(D);
        (*t) = D;
        return *t;
    }
}


// Search for Key K in Tree *T and insert K in *T if not found.  Set *T to
// root of updated tree.  Return TRUE if successful, else FALSE.
Tree searchInsert (Tree *t, Key k) //true means we found it already, false means we didn't find it, and thus inserted it
{
    if (*t == NULL)
	return makeNode (t, k);                 // Create single-node tree
    
    else if (strcmp(k, (*t)->key) == 0 && (*t)->right == NULL)//found the key, increment weight + 1
    {
        (*t)->weight += 1;
        return *t;
    }
    else if ( (strcmp (k, (*t)->key) < 0) && (*t)->left == NULL ) //reached a leaf, where we have to insert a new internal node.
    {
        Tree newinternal;
        Tree newleaf;
        makeNode(&newleaf, k);
        makeNode(&newinternal, k);

        newinternal->right = (*t);
        newinternal->left = newleaf;
        newinternal->weight = 1 + (*t)->weight;
        *t = newinternal;
        (*t)->WEPL = changewepl(*t);
        return *t;
    }
    else if ( (strcmp (k, (*t)->key) > 0) && (*t)->right == NULL ) //insert into the right
    {
        Tree newinternal;
        Tree newleaf;
        makeNode(&newleaf, k);
        makeNode(&newinternal, (*t)->key);

        newinternal->right = newleaf;
        newinternal->left = *t;
        newinternal->weight = 1 + (*t)->weight;
        *t = newinternal;
        (*t)->WEPL = changewepl(*t);
        return *t;
    }
    else if ( strcmp(k, (*t)->key )<= 0)
    {
        (*t)->weight += 1; //ADD IN ALGORITHM TO UPDATE WEPL
        (*t)->left = searchInsert (&(*t)->left, k); //go search in the left subtree
        (*t)->WEPL = changewepl(*t);
        Tree E = (*t)->right;
        if ((*t)->left != NULL){
            Tree A = ((*t)->left)->left;
            if ( A != NULL && (A->weight - E->weight) > IF ) //heavier tree has a depth that can be rotated to right improve WEPL
            {
                (*t) = rotateTree(t, 1);
            }
        }
        return *t;
    }
    else if (strcmp (k, (*t)->key) > 0)
    {
        (*t)->weight += 1;
        (*t)->right = searchInsert (&(*t)->right, k); //go search in right subtree
        (*t)->WEPL = changewepl(*t);
        Tree A = (*t)->left;
        if ((*t)->right != NULL){
            Tree E = ((*t)->right)->right;
            if ( E != NULL && (E->weight - A->weight) > IF ) //heavier tree has a depth that can be rotated to right improve WEPL
            {
                (*t) = rotateTree(t, 0);
            }
        }
        return *t;
    }
    
    return *t;
}

// Delete Key K from Tree *T and set *T to root of updated tree.  Return
// TRUE if successful, FALSE otherwise.
Tree delete (Tree *t, Key k) // deletions must also update the WEPL
{
    if (*t == NULL)
	return *t;   
    else if ( (strcmp(k, (*t)->key) == 0) && (*t)->left == NULL ) //found the element as a leaf
    {
        free((*t)->key);
        free(*t);
        return NULL;
    }                        // Tree is empty
    else if (strcmp (k, (*t)->key) <= 0)//found internal node that k is equal to or less than, move down left 
    {
        int leftoldweight;
        if ((*t)->left == NULL) //reached end of leaf, it is not found.
        {
            return *t;
        }
        leftoldweight = ((*t)->left)->weight;
        (*t)->left = delete (&(*t)->left, k);         // else delete from left subtree
        if ((*t)->left == NULL) //the left child is now null, so must delete this node and promote the right child
        {
            Tree temp = *t;
            *t = (*t)->right;
            free(temp->key);
            free(temp);
            return *t;
        }
        else //left child was not null, re adjust weights accordingly
        {
            (*t)->weight = (*t)->weight - leftoldweight +  ((*t)->left)->weight; //accounts for difference in updating weights.
            (*t)->WEPL = changewepl(*t);
            Tree A = (*t)->left;
            if ((*t)->right != NULL){
                Tree E = ((*t)->right)->right;
                if ( E != NULL && (E->weight - A->weight) > IF ) //heavier tree has a depth that can be rotated to right improve WEPL
                {
                    (*t) = rotateTree(t, 0);
                }
            }
            return *t;
        }
    }
    else if (strcmp (k, (*t)->key) > 0)//found internal node that k greater than, move down right
    {
        int rightoldweight;
        if ((*t)->right == NULL) //reached end of leaf, it is not found.
        {
            return *t;
        }
        rightoldweight = ((*t)->right)->weight;
        (*t)->right = delete (&(*t)->right, k);         // else delete from right subtree
        if ((*t)->right == NULL) //the left child is now null, so must delete this node and promote the left child
        {
            Tree temp = *t;
            *t = (*t)->left;
            free(temp->key);
            free(temp);
            return *t;
        }
        else //right child was not null, re adjust weights accordingly
        {
            (*t)->weight = (*t)->weight - rightoldweight +  ((*t)->right)->weight; //accounts for difference in updating weights.
            (*t)->WEPL = changewepl(*t);
            Tree E = (*t)->right;
            if ((*t)->left != NULL){
                Tree A = ((*t)->left)->left;
                if ( A != NULL && (A->weight - E->weight) > IF ) //heavier tree has a depth that can be rotated to right improve WEPL
                {
                    (*t) = rotateTree(t, 1);
                }
            }
            return *t;
        }
    }
    return *t;
}


// print out the leaves and their counts
void printTree (Tree t)
{
    if (t == NULL)
    {
        return;
    }
    else if (t->right == NULL)//Reached a leaf, print the count and pair
    {
        printf("%3d %s\n", t->weight, t->key);
        return;
    }
    else //at a node, so run print on the left and right
    {
        printTree(t->left);
        printTree(t->right);
        return;
    }
}



void dump(Tree t)
{
    if (t == NULL)
    {
        return;
    }
    else if (t->right == NULL)//at a leaf, print the key
    {
        printf("%s\n", t->key);
    }
    else //node recursive call, print the root, then run it down the left, then right.
    {
        printf("%s\n", t->key);
        dump(t->left);
        dump(t->right);
        return;
    }
}

void epl(Tree t)
{
    if (t != NULL)
    {
        printf("%d, %d\n", t->weight, t->WEPL);
    }
}
