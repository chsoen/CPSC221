/*
*  File:        ptree.cpp
*  Description: Implementation of a partitioning tree class for CPSC 221 PA3
*  Date:        2022-03-03 01:53
*
*               ADD YOUR PRIVATE FUNCTION IMPLEMENTATIONS TO THE BOTTOM OF THIS FILE
*/

#include "ptree.h"
#include "hue_utils.h" // useful functions for calculating hue averages

using namespace cs221util;
using namespace std;

// The following definition may be convenient, but is not necessary to use
typedef pair<unsigned int, unsigned int> pairUI;

/////////////////////////////////
// PTree private member functions
/////////////////////////////////

/*
*  Destroys all dynamically allocated memory associated with the current PTree object.
*  You may want to add a recursive helper function for this!
*  POST: all nodes allocated into the heap have been released.
*/
void PTree::Clear() {
  DeleteNode(root);
}

/*
*  Copies the parameter other PTree into the current PTree.
*  Does not free any memory. Should be called by copy constructor and operator=.
*  You may want a recursive helper function for this!
*  PARAM: other - the PTree which will be copied
*  PRE:   There is no dynamic memory associated with this PTree.
*  POST:  This PTree is a physically separate copy of the other PTree.
*/
void PTree::Copy(const PTree& other) {
  root = CopyNode(other.root);
}

/*
*  Private helper function for the constructor. Recursively builds the tree
*  according to the specification of the constructor.
*  You *may* change this if you like, but we have provided here what we
*  believe will be sufficient to use as-is.
*  PARAM:  im - full reference image used for construction
*  PARAM:  ul - upper-left image coordinate of the currently building Node's image region
*  PARAM:  w - width of the currently building Node's image region
*  PARAM:  h - height of the currently building Node's image region
*  RETURN: pointer to the fully constructed Node
*/
Node* PTree::BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
  double sumHx = 0;
  double sumHy = 0;
  double sumL = 0;
  double sumS = 0;
  double sumA = 0;
  for(unsigned int x=0; x < w; x++) {
    for(unsigned int y=0; y < h; y++) {
      HSLAPixel *curr = im.getPixel(ul.first + x, ul.second + y);
      sumHx += Deg2X(curr->h);
      sumHy += Deg2Y(curr->h);
      sumL += curr->l;
      sumS += curr->s;
      sumA += curr->a;
    }
  }
  unsigned int num = w*h;
  double avgH = XY2Deg(sumHx / num, sumHy / num);
  
  Node *childA = NULL;
  Node *childB = NULL;

  if (w == 1 && h == 1) {
    // stay null
  } else if (w < h) {
    childA = BuildNode(im, make_pair(ul.first, ul.second), w, h/2);
    childB = BuildNode(im, make_pair(ul.first, ul.second + h/2), w, h - h/2);
  } else {
    childA = BuildNode(im, make_pair(ul.first, ul.second), w/2, h);
    childB = BuildNode(im, make_pair(ul.first + w/2, ul.second), w - w/2, h);
  }

  HSLAPixel *avg = new HSLAPixel(avgH, sumS / num, sumL / num, sumA / num);
  Node *node = new Node(ul, w, h, *avg, childA, childB);
  delete avg;
  return node;
}

////////////////////////////////
// PTree public member functions
////////////////////////////////

/*
*  Constructor that builds the PTree using the provided PNG.
*
*  The PTree represents the sub-image (actually the entire image) from (0,0) to (w-1, h-1) where
*  w-1 and h-1 are the largest valid image coordinates of the original PNG.
*  Each node corresponds to a rectangle of pixels in the original PNG, represented by
*  an (x,y) pair for the upper-left corner of the rectangle, and two unsigned integers for the
*  number of pixels on the width and height dimensions of the rectangular sub-image region the
*  node defines.
*
*  A node's two children correspond to a partition of the node's rectangular region into two
*  equal (or approximately equal) size regions which are either tiled horizontally or vertically.
*
*  If the rectangular region of a node is taller than it is wide, then its two children will divide
*  the region into vertically-tiled sub-regions of equal height:
*  +-------+
*  |   A   |
*  |       |
*  +-------+
*  |   B   |
*  |       |
*  +-------+
*
*  If the rectangular region of a node is wider than it is tall, OR if the region is exactly square,
*  then its two children will divide the region into horizontally-tiled sub-regions of equal width:
*  +-------+-------+
*  |   A   |   B   |
*  |       |       |
*  +-------+-------+
*
*  If any region cannot be divided exactly evenly (e.g. a horizontal division of odd width), then
*  child B will receive the larger half of the two subregions.
*
*  When the tree is fully constructed, each leaf corresponds to a single pixel in the PNG image.
* 
*  For the average colour, this MUST be computed separately over the node's rectangular region.
*  Do NOT simply compute this as a weighted average of the children's averages.
*  The functions defined in hue_utils.h and implemented in hue_utils.cpp will be very useful.
*  Computing the average over many overlapping rectangular regions sounds like it will be
*  inefficient, but as an exercise in theory, think about the asymptotic upper bound on the
*  number of times any given pixel is included in an average calculation.
* 
*  PARAM: im - reference image which will provide pixel data for the constructed tree's leaves
*  POST:  The newly constructed tree contains the PNG's pixel data in each leaf node.
*/
PTree::PTree(PNG& im) {
  // add your implementation below
  root = BuildNode(im, make_pair(0, 0), im.width(), im.height());
}

/*
*  Copy constructor
*  Builds a new tree as a copy of another tree.
*
*  PARAM: other - an existing PTree to be copied
*  POST:  This tree is constructed as a physically separate copy of other tree.
*/
PTree::PTree(const PTree& other) {
  Copy(other);
}

/*
*  Assignment operator
*  Rebuilds this tree as a copy of another tree.
*
*  PARAM: other - an existing PTree to be copied
*  POST:  If other is a physically different tree in memory, all pre-existing dynamic
*           memory in this tree is deallocated and this tree is reconstructed as a
*           physically separate copy of other tree.
*         Otherwise, there is no change to this tree.
*/
PTree& PTree::operator=(const PTree& other) {
  if(this->root != other.root) {
    Clear();
    Copy(other);
  }
  return *this;
}

/*
*  Destructor
*  Deallocates all dynamic memory associated with the tree and destroys this PTree object.
*/
PTree::~PTree() {
  Clear();
}

/*
*  Traverses the tree and puts the leaf nodes' color data into the nodes'
*  defined image regions on the output PNG.
*  For non-pruned trees, each leaf node corresponds to a single pixel that will be coloured.
*  For pruned trees, each leaf node may cover a larger rectangular region that will be
*  entirely coloured using the node's average colour attribute.
*
*  You may want to add a recursive helper function for this!
*
*  RETURN: A PNG image of appropriate dimensions and coloured using the tree's leaf node colour data
*/
PNG PTree::Render() const {
  PNG result = PNG(root->width, root->height);
  RenderNode(result, root);
  return result;
}

/*
*  Trims subtrees as high as possible in the tree. A subtree is pruned
*  (its children are cleared/deallocated) if ALL of its leaves have colour
*  within tolerance of the subtree root's average colour.
*  Pruning criteria should be evaluated on the original tree, and never on a pruned
*  tree (i.e. we expect that Prune would be called on any tree at most once).
*  When processing a subtree, you should determine if the subtree should be pruned,
*  and prune it if possible before determining if it has subtrees that can be pruned.
* 
*  You may want to add (a) recursive helper function(s) for this!
*
*  PRE:  This tree has not been previously pruned (and is not copied/assigned from a tree that has been pruned)
*  POST: Any subtrees (as close to the root as possible) whose leaves all have colour
*        within tolerance from the subtree's root colour will have their children deallocated;
*        Each pruned subtree's root becomes a leaf node.
*/
void PTree::Prune(double tolerance) {
  Prune(root, tolerance);
}

/*
*  Returns the total number of nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::Size() const {
  return Size(root);
}

/*
*  Returns the total number of leaf nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::NumLeaves() const {
  return NumLeaves(root);
}

/*
*  Rearranges the nodes in the tree, such that a rendered PNG will be flipped horizontally
*  (i.e. mirrored over a vertical axis).
*  This can be achieved by manipulation of the nodes' member attribute(s).
*  Note that this may possibly be executed on a pruned tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*
*  POST: Tree has been modified so that a rendered PNG will be flipped horizontally.
*/
void PTree::FlipHorizontal() {
  FlipHorizontal(root);
}

/*
*  Like the function above, rearranges the nodes in the tree, such that a rendered PNG
*  will be flipped vertically (i.e. mirrored over a horizontal axis).
*  This can be achieved by manipulation of the nodes' member attribute(s).
*  Note that this may possibly be executed on a pruned tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*
*  POST: Tree has been modified so that a rendered PNG will be flipped vertically.
*/
void PTree::FlipVertical() {
  FlipVertical(root);
}

/*
    *  Provides access to the root of the tree.
    *  Dangerous in practice! This is only used for testing.
    */
Node* PTree::GetRoot() {
  return root;
}

//////////////////////////////////////////////
// PERSONALLY DEFINED PRIVATE MEMBER FUNCTIONS
//////////////////////////////////////////////

Node *PTree::CopyNode(const Node *curr) {
  if(!curr) {
    return NULL;
  }

  Node *node = new Node();
  node->upperleft = curr->upperleft;
  node->width = curr->width;
  node->height = curr->height;
  node->avg = curr->avg;

  node->A = CopyNode(curr->A);
  node->B = CopyNode(curr->B);
  return node;
}

int PTree::Size(const Node *curr) const {
  if(!curr) {
    return 0;
  }
  return Size(curr->A) + Size(curr->B) + 1;
}

int PTree::NumLeaves(const Node *curr) const {
  if(!curr) {
    return 0;
  }
  return NumLeaves(curr->A) + NumLeaves(curr->B) + (!curr->A && !curr->B);
}

void PTree::Prune(Node *curr, double tolerance) {
  if(!curr) {
    return;
  }
  if(Prunable(curr, tolerance, curr->avg)) {
    DeleteNode(curr->A);
    DeleteNode(curr->B);
    curr->A = NULL;
    curr->B = NULL;
  }
  Prune(curr->A, tolerance);
  Prune(curr->B, tolerance);
}

bool PTree::Prunable(Node *curr, double tolerance, HSLAPixel subrootAvg) {
  if(!curr) {
    return true;
  }
  return Prunable(curr->A, tolerance, subrootAvg) &&
          Prunable(curr->B, tolerance, subrootAvg) &&
          curr->avg.dist(subrootAvg) < tolerance;
}

void PTree::DeleteNode(Node *curr) {
  if(!curr) {
    return;
  }
  DeleteNode(curr->A);
  DeleteNode(curr->B);
  delete curr;
}

void PTree::RenderNode(PNG &im, Node *curr) const {
  if(!curr) {
    return;
  }
  if(curr->A || curr->B) {
    RenderNode(im, curr->A);
    RenderNode(im, curr->B);
    return;
  }
  for(unsigned int x=0; x<curr->width; x++) {
    for(unsigned int y=0; y<curr->height; y++) {
      HSLAPixel *pixel = im.getPixel(curr->upperleft.first + x, curr->upperleft.second + y);
      pixel->h = curr->avg.h;
      pixel->s = curr->avg.s;
      pixel->l = curr->avg.l;
      pixel->a = curr->avg.a;
    }
  }
}

void PTree::FlipHorizontal(Node *curr) {
  if(!curr || !curr->A || !curr->B) {
    return;
  }
  curr->A->upperleft.first = root->width - curr->A->width - curr->A->upperleft.first;
  curr->B->upperleft.first = root->width - curr->B->width - curr->B->upperleft.first;
  FlipHorizontal(curr->A);
  FlipHorizontal(curr->B);
}

void PTree::FlipVertical(Node *curr) {
  if(!curr || !curr->A || !curr->B) {
    return;
  }
  curr->A->upperleft.second = root->height - curr->A->height - curr->A->upperleft.second;
  curr->B->upperleft.second = root->height - curr->B->height - curr->B->upperleft.second;
  FlipVertical(curr->A);
  FlipVertical(curr->B);
}