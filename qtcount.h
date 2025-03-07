#ifndef _QTCOUNT_H_
#define _QTCOUNT_H_

#include <cmath>
#include <utility>

#include "compression/PNG.h"
#include "compression/RGBAPixel.h"
#include "quadtree.h"

using namespace std;
using namespace compression;

class qtcount : public quadtree {
public:
    qtcount(PNG& im) : quadtree(im) {}

private:
    bool prunable(Node* node, const int tol) const;
    //ADD
    long colrDist(RGBAPixel& a1, RGBAPixel& a2) const;
    //ADD
    bool checkLeaf(Node* cur, Node* root, const int tol) const;
};

#endif
