/**
 * @file binarytree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file, as well as binarytree.h.
 */

#include <iostream>

using namespace std;

/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T>
int BinaryTree<T>::height() const
{
    // Call recursive helper function on root
    return height(root);
}

/**
 * Private helper function for the public height function, with an additional
 * Node* parameter to allow for recursive calls. NOTE: use this as an example
 * for writing your own private helper functions.
 * @param subRoot
 * @return The height of the subtree.
 */
template <typename T>
int BinaryTree<T>::height(const Node* subRoot) const
{
    // Base case
    if (subRoot == NULL)
        return -1;

    // Recursive definition
    return 1 + max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T>
void BinaryTree<T>::printLeftToRight() const
{
    printLeftToRight(root);
    // Do not remove this line - used for correct print output
    cout << endl;
}

// Helper for printLeftToRight
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node *subRoot) const
{
  if (!subRoot) {
    return;
  }

  printLeftToRight(subRoot->left);
  cout << to_string(subRoot->elem) << " ";
  printLeftToRight(subRoot->right);
}

/**
 * Flips the tree over a vertical axis, modifying the tree itself
 * (i.e. not creating a flipped copy).
 */
template <typename T>
void BinaryTree<T>::mirror()
{
    mirrorHelper(root);
}

// Helper for mirror
template <typename T>
void BinaryTree<T>::mirrorHelper(Node *&subRoot)
{
  if (!subRoot) {
    return;
  }
  swap(subRoot->left, subRoot->right);
  mirrorHelper(subRoot->left);
  mirrorHelper(subRoot->right);
}

/**
 * Prints out all the possible paths from the root of the tree to any leaf node.
 * That is, all sequences starting at the root node and continuing downwards,
 * ending at a leaf node. Paths ending in a left node should be printed before
 * paths ending in a node further to the right.
 */
template <typename T>
void BinaryTree<T>::printPaths() const
{
  printPaths(root, "");
}

// Helper for printPaths
template <typename T>
void BinaryTree<T>::printPaths(const Node *subRoot, string path) const
{
  if (!subRoot) {
    return;
  }
  path = path + to_string(subRoot->elem) + " ";
  if (!subRoot->left && !subRoot->right) {
    cout << "Path: " << path << "\n";
    return;
  }
  printPaths(subRoot->left, path);
  printPaths(subRoot->right, path);
}

/**
 * Each node in a tree has a distance from the root node - the depth of that
 * node, or the number of edges along the path from that node to the root. This
 * function returns the sum of the distances of all nodes to the root node (the
 * sum of the depths of all the nodes). Your solution should take O(n) time,
 * where n is the number of nodes in the tree.
 * @return The sum of the distances of all nodes to the root.
 */
template <typename T>
int BinaryTree<T>::sumDistances() const
{
  return sumDistances(root, 0);
}

// Helper for sumDistances
template <typename T>
int BinaryTree<T>::sumDistances(const Node *subRoot, int depth) const
{
  if (!subRoot) {
    return 0;
  }
  return depth + sumDistances(subRoot->left, depth + 1) + sumDistances(subRoot->right, depth + 1);
}

/**
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrdered() const
{
  int rootNum = stoi(to_string(root->elem));
  return isOrdered(root->left, rootNum, "l")
  && isOrdered(root->right, rootNum, "r");
}

// Helper for isOrdered

template <typename T>
bool BinaryTree<T>::isOrdered(const Node *subRoot, int rootNum, string dir) const
{
  if (!subRoot) {
    return true;
  }
  int thisNum = stoi(to_string(subRoot->elem));
  if (dir == "l") {
    if (thisNum < rootNum) {
      return isOrdered(subRoot->left, rootNum, "l")
      && isOrdered(subRoot->right, rootNum, "l")
      && isOrdered(subRoot->left, thisNum, "l")
      && isOrdered(subRoot->right, thisNum, "g");
    } else {
      return false;
    }
  }
  if (thisNum > rootNum) {
    return isOrdered(subRoot->left, rootNum, "g")
    && isOrdered(subRoot->right, rootNum, "g")
    && isOrdered(subRoot->left, thisNum, "l")
    && isOrdered(subRoot->right, thisNum, "g");
  } else {
    return false;
  }
}