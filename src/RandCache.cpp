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
#include "RandCache.h"
#include <random>
using namespace std;

vector<int> RandCache::generateCache(unsigned int size) {
  vector<int> v;
  for (unsigned int i = 0; i < size; i++) {
    v.push_back(rand());
  }
  return v;
}
    
  
int RandCache::next() const {
  unsigned int curr = ++index;
  return cache.at(curr % cache.size());
}
