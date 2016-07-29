// protoTree.h                                      Stan Eisenstat (04/01/16)
//
// A possible interface specification for a STANDARD binary search tree.

#include <stdbool.h>

// External definition of Tree
typedef struct tnode *Tree;

// Datatype for Key
typedef char *Key;

// Set *T to the empty tree.  Return TRUE if successful, else FALSE.
bool create (Tree *t);

Tree makeNode (Tree *t, Key k);
// Search for Key K in Tree *T and insert K in *T if not found.  Return
// TRUE if successful, else FALSE.  Set *T to root of updated tree.
Tree searchInsert (Tree *t, Key k);

// Delete Key K from Tree *T.  Return TRUE if successful, else FALSE.  Set
// *T to root of updated tree.
Tree delete (Tree *t, Key k);

// Perform an inorder traversal of Tree T, printing one Key per line.
void printTree (Tree t);

// Perform a preorder traversal of Tree T, printing one Key per line.
void epl (Tree t);

void dump (Tree t);

void setIF(int newval);

Tree rotateTree (Tree *t, int direction);
