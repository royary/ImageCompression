#include "qtcount.h"
#include "compression/RGBAPixel.h"


//A node is pruned if all of the leaves in its subtree have colour-distance 
//within tolerance of its average. Distances between colours are computed as the sum, 
//over each colour channel, of the pixel value differences, squared. 
bool qtcount::prunable(Node *node, const int tol) const {
  // if node is empty, not prunable
  if (node == NULL)
    return false;

  // if come to leave, is prunable
  if (node->NW == NULL)
    return true;

  return checkLeaf(node, node, tol);
} 

bool qtcount::checkLeaf(Node* cur, Node* root, const int tol) const {
  if(cur == NULL) return true;
  //arrive at leave, check
  if(cur->NW == NULL){
    return colrDist(cur->avg, root->avg) <= tol;
  }

  //check recursive
  return checkLeaf(cur->NW, root, tol) && checkLeaf(cur->NE, root, tol)
  && checkLeaf(cur->SE, root, tol) && checkLeaf(cur->SW, root, tol);

}



// Distances between colours are computed as the sum, over each colour channel,
//  of the pixel value differences, squared.
long qtcount::colrDist(RGBAPixel &a1, RGBAPixel &a2) const {
  long r = a1.r - a2.r;
  long g = a1.g - a2.g;
  long b = a1.b - a2.b;
  return (r * r + g * g + b * b);
}