
/**
 *
 * quadtree 
 * quadtree.cpp
 *
 */
#include <iostream>
#include "quadtree.h"
#include "compression/RGBAPixel.h"
using namespace std;

// Node constructor
quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
    : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr),
      SW(nullptr) {}

// quadtree destructor
quadtree::~quadtree() { clear(); }
// quadtree copy constructor
quadtree::quadtree(const quadtree &other) { copy(other); }
// quadtree assignment operator
quadtree &quadtree::operator=(const quadtree &rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

// Node* buildTree(stats& s, pair<int, int> ul, int dim);
quadtree::quadtree(PNG &imIn) {
  // Find the smaller dimension, becasue the image maybe not a square
  edge = min(imIn.width(), imIn.height());
  // Find the largest power of 2 that fits within this dimension
  int dim = log2(edge);
   // Set edge to be 2^dim
  edge = pow(2, dim);
  stats s(imIn);
  root = buildTree(s, make_pair(0, 0), dim);
}

quadtree::Node *quadtree::buildTree(stats &s, pair<int, int> ul, int dim) {
  RGBAPixel avg = s.getAvg(ul, dim);
  double var = s.getVar(ul, dim);
  Node *node = new Node(ul, dim, avg, var);
  if (dim == 0)
    return node;
  int childrenDim = dim - 1;
  int num = pow(2, childrenDim);

  node->NW = buildTree(s, ul, childrenDim);
  node->NE = buildTree(s, make_pair(ul.first + num, ul.second), childrenDim);
  node->SE =
      buildTree(s, make_pair(ul.first + num, ul.second + num), childrenDim);
  node->SW = buildTree(s, make_pair(ul.first, ul.second + num), childrenDim);

  return node;
}

PNG quadtree::render() const {
  PNG ret(edge, edge);
  renderHelper(root, ret);
  return ret;
}

void quadtree::renderHelper(Node *node, PNG &img) const {
  if (node == NULL)
    return;
 if (node->NW == NULL) {
    int size = pow(2, node->dim);
    for (int x = node->upLeft.first; x < (node->upLeft.first + size); x++) {
      for (int y = node->upLeft.second; y < (node->upLeft.second + size); y++) {
        *img.getPixel(x, y) = node->avg;
      }
    }

  } else {
    renderHelper(node->NW, img);
    renderHelper(node->NE, img);
    renderHelper(node->SE, img);
    renderHelper(node->SW, img);
  }
}

// binary search
// the inverse of the pruneSize function.
int quadtree::idealPrune(const int leaves) const {
  int low = 0;
  int high = 255 * 255 * 3;
  while (low < high) {
    int mid = (low + high) / 2;

    // Current tolerance too high, try lower half
    if (pruneSize(mid) <= leaves) {
      high = mid;
    } else {
      // Current tolerance too low, try upper half
      low = mid + 1;
    }
  }
  return low;
}

int quadtree::pruneSize(const int tol) const {
  return pruneSizeHelper(root, tol);
}

int quadtree::pruneSizeHelper(Node *node, const int tol) const {
  if (node == NULL)
    return 0;
  if (prunable(node, tol)) {
    return 1;
  } else {
    return pruneSizeHelper(node->NW, tol) + pruneSizeHelper(node->NE, tol) +
           pruneSizeHelper(node->SE, tol) + pruneSizeHelper(node->SW, tol);
  }
}

void quadtree::prune(const int tol) {
  pruneHelper(root, tol);
}

void quadtree::pruneHelper(Node *node, const int tol) {
  if (node == NULL)
    return;
  if (prunable(node, tol)) {
    clearHelper(node->NW);
    clearHelper(node->NE);
    clearHelper(node->SE);
    clearHelper(node->SW);

    node->NW = NULL;
    node->NE = NULL;
    node->SE = NULL;
    node->SW = NULL;
  } else {
    pruneHelper(node->NW, tol);
    pruneHelper(node->NE, tol);
    pruneHelper(node->SE, tol);
    pruneHelper(node->SW, tol);
  }
}

void quadtree::clear() {
  clearHelper(root);
  root = NULL;
  edge = 0;
}

void quadtree::clearHelper(Node *root) {
  if (root == NULL)
    return;
  clearHelper(root->NW);
  clearHelper(root->NE);
  clearHelper(root->SE);
  clearHelper(root->SW);
  delete root;
  // root = NULL;
}

void quadtree::copy(const quadtree &orig) {
  edge = orig.edge;
  root = copyHelper(orig.root);
}

quadtree::Node *quadtree::copyHelper(Node *node) {
  if (node == NULL)
    return NULL;
  Node *newNode = new Node(node->upLeft, node->dim, node->avg, node->var);
  newNode->NW = copyHelper(node->NW);
  newNode->NE = copyHelper(node->NE);
  newNode->SE = copyHelper(node->SE);
  newNode->SW = copyHelper(node->SW);

  return newNode;
}
