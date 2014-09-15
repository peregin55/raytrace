#ifndef CUBE_H
#define CUBE_H

#include <string>
#include <vector>
#include "Surface.h"
class Point;
class Ray;
class Vector;
using namespace std;

class Cube : public Surface {
  public:
    Cube(const string& materialName, const vector<double>& min, const vector<double>& max) :
      Surface(materialName), cubeMin(min), cubeMax(max) { }
    virtual unique_ptr<Hit> intersect(const Ray& ray, double t0, double t1) const;
    bool isBounded(const Ray& ray, double& t0, double& t1) const;
    virtual Vector calculateNormal(const Point& surfacePoint) const;
    vector<double> getCubeMin() { return cubeMin; }
    vector<double> getCubeMax() { return cubeMax; }
  private:
    bool intersect(const Ray& ray, double& tmin, double& tmax);
    vector<double> cubeMin;
    vector<double> cubeMax;
    static const double EPSILON;
};
#endif
