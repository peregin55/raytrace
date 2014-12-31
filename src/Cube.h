#ifndef CUBE_H
#define CUBE_H

#include <memory>
#include <string>
#include <vector>
#include "Surface.h"
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
    Cube(const string& materialName, const vector<double>& min, const vector<double>& max) :
      Surface(materialName), cubeMin(min), cubeMax(max) { }
    /** Surface intersection. */
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    /* returns true if ray intersects this cube, with t0 containing the distance at which
     * ray enters the cube, and t1 the distance at which it exits.
     */
    bool isBounded(const Ray& ray, double& t0, double& t1) const;
    vector<double> getCubeMin();
    vector<double> getCubeMax();
  private:
    Vector calculateNormal(const Point& surfacePoint) const;
    unique_ptr<Hit> buildHit(const Ray& ray, double t) const;
    vector<double> cubeMin;
    vector<double> cubeMax;
    static const double EPSILON;
};
#endif
