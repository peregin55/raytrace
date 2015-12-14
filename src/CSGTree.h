/* Copyright 2015 Stephen H. Johnson

  This file is part of raytrace.

  raytrace is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  raytrace is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with raytrace.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef CSGTREE_H
#define CSGTREE_H

#include <memory>
#include "BoundingBox.h"
#include "Surface.h"
using namespace std;
class BoundingBox;
class Hit;
class Ray;

class CSGTree : public Entity {
  protected:
    CSGTree(unique_ptr<CSGTree> parent) : parent(std::move(parent)) { }
    unique_ptr<CSGTree> parent;
};

enum CSGOperation {UNION, SUBTRACT, INTERSECT};
class CSGNode : public CSGTree {
  public:
    CSGNode(CSGOperation op, unique_ptr<CSGTree> parent, unique_ptr<CSGTree> left, unique_ptr<CSGTree> right) :
      CSGTree(std::move(parent)), op(op), left(std::move(left)), right(std::move(right)) {
        boundingBox = BoundingBox(-1.0, -1.0, -1.0, 1.0, 1.0, 1.0);
    }
    virtual bool intersect(const Ray& ray, double t0, double t1, Hit& hit) const;
    virtual bool intersectAll(const Ray& ray, Hit& in, Hit& out) const;
    virtual const BoundingBox& getBoundingBox() const;
  private:
    bool applySetOp(const Hit& leftIn, const Hit& leftOut, const Hit& rightIn, const Hit& rightOut, Hit& in, Hit& out) const;
    bool applySetOpLeft(const Hit& leftIn, const Hit& leftOut, Hit& in, Hit& out) const;
    bool applySetOpRight(const Hit& rightIn, const Hit& rightOut, Hit& in, Hit& out) const;
    CSGOperation op;
    unique_ptr<CSGTree> left;
    unique_ptr<CSGTree> right;
    BoundingBox boundingBox;
};

class CSGLeaf : public CSGTree, public Surface {
  public:
    CSGLeaf(unique_ptr<Surface> surface, unique_ptr<CSGTree> parent) :
      CSGTree(std::move(parent)), Surface(nullptr, nullptr), surface(std::move(surface)) { }
    virtual bool intersect(const Ray& ray, double t0, double t1, Hit& hit) const;
    virtual bool intersectAll(const Ray& ray, Hit& in, Hit& out) const;
    virtual const BoundingBox& getBoundingBox() const;
    virtual Vector calculateNormal(const Point& hitpoint) const;
    virtual Color textureColor(const Point& hitpoint) const;
  private:
    unique_ptr<Surface> surface;
};
#endif
