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
#ifndef RANDCACHE_H
#define RANDCACHE_H
#include <atomic>
#include <vector>
using namespace std;

/** RandCache.
 */
class RandCache {
  public:
    RandCache() : index(0), cache{generateCache(500)} { }
    explicit RandCache(unsigned int size) : index(0), cache{generateCache(size)} { }
    int next() const;
  private:
    mutable atomic<unsigned int> index;
    vector<int> cache;
    static vector<int> generateCache(unsigned int size);
};
#endif
