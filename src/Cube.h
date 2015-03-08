#ifndef CUBE_H
#define CUBE_H

#include <memory>
#include <string>
#include <vector>
#include "Surface.h"
class Color;
class Matrix4;
class Point;
class Ray;
class Vector;
using namespace std;

/** Cube.
 * Axis-aligned cube or bounding box.
 */
class Cube : public Surface {
  public:
    /** Construct cube given name of material, minimum XYZ dimensions contained in min vector,
     * and maximum XYZ dimensions in max vector. (min and max should have 3 components).
     */
    Cube(shared_ptr<Material> material, shared_ptr<Texture> texture, const vector<double>& min, const vector<double>& max) :
      Surface(material, texture), cubeMin(min), cubeMax(max) { }
    /** Surface intersection. */
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    virtual Vector calculateNormal(const Point& hitpoint) const;
    virtual Color textureColor(const Point& hitpoint) const;
    /* returns true if ray intersects this cube, with t0 containing the distance at which
     * ray enters the cube, and t1 the distance at which it exits.
     */
    bool isBounded(const Ray& ray, double& t0, double& t1) const;
    vector<double> getCubeMin();
    vector<double> getCubeMax();
  private:
    vector<double> cubeMin;
    vector<double> cubeMax;
    static const double EPSILON;
};
#endif
