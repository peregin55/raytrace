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
#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <vector>
class Point;
class Ray;
using namespace std;

/** Bounding Box.
 * Axis-aligned bounding box.
 */
class BoundingBox {
  public:
    /** Construct bounding box, minimum XYZ dimensions contained in min vector,
     * and maximum XYZ dimensions in max vector. (min and max should have 3 components).
     */
    BoundingBox() { }
    BoundingBox(double minX, double minY, double minZ, double maxX, double maxY, double maxZ) :
      boxMin{minX, minY, minZ}, boxMax{maxX, maxY, maxZ} { }
    BoundingBox(const vector<double>& minXYZ, const vector<double>& maxXYZ) :
      boxMin(minXYZ), boxMax(maxXYZ) { }
    /** Surface intersection. */
    bool intersect(const Ray& ray, double t0, double t1) const;
    bool intersect(const Ray& ray, double t0, double t1, double* hitT) const;
    const vector<double>& getMin() const;
    const vector<double>& getMax() const;
    vector<Point> calculateEndpoints() const;
  private:
    bool calculateBothIntersects(const Ray& ray, double intersects[]) const;
    vector<double> boxMin;
    vector<double> boxMax;
};
#endif
