#include "qtvar.h"


//A node is pruned if its variance is less than tolerance.
bool qtvar::prunable(Node* node, const int tol) const {
    if(node->var < tol) return true;
    return false;
}
