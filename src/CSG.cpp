#include "CSG.h"
#include "Ray.h"
#include "Hit.h"
#include "RenderException.h"
#include "Shading.h"
#include <vector>

bool CSG::intersect(const Ray& ray, double t0, double t1, Hit& hit) const {
  Hit in;
  Hit out;
  if (boundingBox.intersect(ray, t0, t1)) {
    if (intersectAll(ray, in, out)) {
      if (in.getT() >= t0 && in.getT() <= t1) {
        hit = in;
        return true;
      } else if (out.getT() >= t0 && out.getT() <= t1) {
        hit = out;
        return true;
      }
    }
  }
  return false; 
}

bool CSG::intersectAll(const Ray& ray, Hit& in, Hit& out) const {
  Hit leftIn;
  Hit leftOut;
  Hit rightIn;
  Hit rightOut;
  bool isIntersect = false;
  if (surface) {
    isIntersect = surface->intersectAll(ray, in, out);
  } else {
    bool isLeft = left->intersectAll(ray, leftIn, leftOut);
    bool isRight = right->intersectAll(ray, rightIn, rightOut);
    if (isLeft && isRight) {
      isIntersect = applySetOp(leftIn, leftOut, rightIn, rightOut, in, out);
    } else if (isLeft) {
      isIntersect = applySetOpLeft(leftIn, leftOut, in, out);
    } else if (isRight) {
      isIntersect = applySetOpRight(rightIn, rightOut, in, out);
    }
  }
  if (isIntersect) {
    in = in.pushSurface(this);
    out = out.pushSurface(this);
  }
  return isIntersect;
}

bool CSG::applySetOp(const Hit& leftIn, const Hit& leftOut, const Hit& rightIn, const Hit& rightOut, Hit& in, Hit& out) const {
  if (op == UNION) {
    in = Hit::min(leftIn, rightIn);
    out = Hit::max(leftOut, rightOut);
  } else if (op == SUBTRACT) {
    if ((leftIn < rightIn && leftOut < rightIn) || (leftIn > rightOut && leftOut > rightOut)) {
      // no intersection, so just return left
      in = leftIn;
      out = leftOut;
    } else if (leftIn < rightIn && leftOut > rightIn && leftOut < rightOut) {
      // overlap, left in front
      in = leftIn;
      out = rightIn;
    } else if (leftIn < rightIn && leftOut > rightOut) {
      // left completely contains right
      in = leftIn;
      out = leftOut;
    } else if (leftIn >= rightIn && leftOut <= rightOut) {
      // right completely contains left
      return false;
    } else if (leftIn > rightIn && leftIn < rightOut && leftOut > rightOut) {
      // overlap, right in front
      in = rightOut;
      out = leftOut;
    }
    else {
      throw RenderException("Invalid CSG SUBTRACT");
    }
  } else if (op == INTERSECT) {
    if ((leftIn < rightIn && leftOut < rightIn) || (leftIn > rightOut && leftOut > rightOut)) {
      return false;  // 2 objects don't actually intersect
    } else {
      in = Hit::max(leftIn, rightIn);
      out = Hit::min(leftOut, rightOut);
    }
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

Shading CSG::shading(const Point& hitpoint, vector<const Surface*> surfaceStack) const {
  const CSG* p = this;
  int sign = 1;
  if (p != surfaceStack.back()) {
    throw RenderException("Invalid hit-surfaces passed to CSG");
  }
  surfaceStack.pop_back();
  while(!p->surface.get()) {
    if ((p->right).get() == surfaceStack.back()) {
      if (p->op == SUBTRACT) {
        sign *= -1;
      }
      p = (p->right).get();
    } else if ((p->left).get() == surfaceStack.back()) {
      p = (p->left).get();
    } else {
      printTree(this);
      cerr << endl;
      for (auto it = surfaceStack.rbegin(); it != surfaceStack.rend(); it++) {
        cerr << (*it) << " ";
      }
      cerr << endl;
      throw RenderException("unrecognized CSG surface");
    }
    surfaceStack.pop_back();
  }
  const Shading& shading = p->surface->shading(hitpoint, surfaceStack);
  return Shading(shading.getNormal() * sign, shading.getTextureColor(), shading.getMaterial());
}

void CSG::printTree(const CSG* p) const {
  if (p->left) printTree(p->left.get());
  cerr << p;
  if (p->surface) {
    cerr << "(" << p->surface.get() << ") ";
  } else {
    cerr << " ";
  }
  if (p->right) printTree(p->right.get());
}
