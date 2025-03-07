#ifndef _QTVAR_H_
#define _QTVAR_H_

#include <cmath>
#include <utility>

#include "compression/PNG.h"
#include "compression/RGBAPixel.h"
#include "quadtree.h"

using namespace std;
using namespace compression;

class qtvar : public quadtree {
public:
    qtvar(PNG& im) : quadtree(im) {}

private:
    bool prunable(Node* node, const int tol) const;
};

#endif
