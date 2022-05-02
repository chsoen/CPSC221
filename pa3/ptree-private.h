/*
*  File:        ptree-private.h
*  Description: Private functions for the PTree class
*  Date:        2022-03-06 03:30
*
*               DECLARE YOUR PTREE PRIVATE MEMBER FUNCTIONS HERE
*/

#ifndef _PTREE_PRIVATE_H_
#define _PTREE_PRIVATE_H_

/////////////////////////////////////////////////
// DEFINE YOUR PRIVATE MEMBER FUNCTIONS HERE
//
// Just write the function signatures.
//
// Example:
//
// Node* MyHelperFunction(int arg_a, bool arg_b);
//
/////////////////////////////////////////////////

Node *CopyNode(const Node *curr);
int Size(const Node *curr) const;
int NumLeaves(const Node *curr) const;
void Prune(Node *curr, double tolerance);
bool Prunable(Node *curr, double tolerance, HSLAPixel subrootAvg);
void DeleteNode(Node *curr);
void RenderNode(PNG &im, Node *curr) const;
void FlipHorizontal(Node *curr);
void FlipVertical(Node *curr);

#endif