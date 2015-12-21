#include "CSG.h"
#include "Ray.h"
#include "Hit.h"
#include "RenderException.h"
#include <vector>

bool CSG::intersect(const Ray& ray, double t0, double t1, Hit& hit) const {
  Hit in;
  Hit out;
  if (intersectAll(ray, in, out)) {
    if (in.getT() >= t0 && in.getT() <= t1) {
      hit = in;
      return true;
    } else if (out.getT() >= t0 && out.getT() <= t1) {
      hit = out;
      return true;
    }
  }
  return false; 
}

bool CSG::intersectAll(const Ray& ray, Hit& in, Hit& out) const {
  Hit leftIn;
  Hit leftOut;
  Hit rightIn;
  Hit rightOut;
  if (surface) {
    return surface->intersectAll(ray, in, out);
  } else {
    bool isLeft = left->intersectAll(ray, leftIn, leftOut);
    bool isRight = right->intersectAll(ray, rightIn, rightOut);
    if (isLeft && isRight) {
      return applySetOp(leftIn, leftOut, rightIn, rightOut, in, out);
    } else if (isLeft) {
      return applySetOpLeft(leftIn, leftOut, in, out);
    } else if (isRight) {
      return applySetOpRight(rightIn, rightOut, in, out);
    } else {
      return false;
    }
  }
}

bool CSG::applySetOp(const Hit& leftIn, const Hit& leftOut, const Hit& rightIn, const Hit& rightOut, Hit& in, Hit& out) const {
  if (op == UNION) {
    in = Hit::min(leftIn, rightIn);
    out = Hit::max(leftOut, rightOut);
  } else if (op == SUBTRACT) {
    in = rightOut;
    out = leftOut;
  } else if (op == INTERSECT) {
    in = Hit::max(leftIn, rightIn);
    out = Hit::min(leftOut, rightOut);
  } else {
    throw RenderException("Unknown CSGOperation " + op);
  }
  return true;
}
  
bool CSG::applySetOpLeft(const Hit& leftIn, const Hit& leftOut, Hit& in, Hit& out) const {
  if (op == UNION || op == SUBTRACT) {
    in = leftIn;
    out = leftOut;
    return true;
  } else if (op == INTERSECT) {
    return false;
  } else {
    throw new RenderException("Unknown CSGOperation " + op);
  }
}

bool CSG::applySetOpRight(const Hit& rightIn, const Hit& rightOut, Hit& in, Hit& out) const {
  if (op == UNION) {
    in = rightIn;
    out = rightOut;
    return true;
  } else if (op == SUBTRACT || op == INTERSECT) {
    return false;
  } else {
    throw new RenderException("Unknown CSGOperation " + op);
  }
}


const BoundingBox& CSG::getBoundingBox() const {
  return boundingBox;
}


Vector CSG::calculateNormal(const Point& hitpoint, const Hit& hit) const {

  const CSG* p = this;
  int sign = 1;
   
  for(auto it = hit.getSurfacePath().rbegin(); it != hit.getSurfacePath().rend(); it++) {
    if ((p->right).get() == *it) {
      if (p->op == SUBTRACT) {
        cerr << "reversing sign" << endl;
        sign *= -1;
      }
      p = (p->right).get();
    } else if ((p->left).get() == *it) {
      p = (p->left).get();
    }
  }
  return hit.getSurfacePath().front()->calculateNormal(hitpoint, hit) * sign;

  
/*   
  // fix
  if (!surface) {
    return left->calculateNormal(hitpoint, hit);
  } else {
    return surface->calculateNormal(hitpoint, hit);
  }
 */ 
}

Color CSG::textureColor(const Point& hitpoint, const Hit& hit) const {
  // fix
  if (!surface) {
    return left->textureColor(hitpoint, hit);
  } else {
    return surface->textureColor(hitpoint, hit);
  }
}
