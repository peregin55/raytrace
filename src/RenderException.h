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
#ifndef RENDEREXCEPTION_H
#define RENDEREXCEPTION_H

#include <exception>
#include <string>
using namespace std;

class RenderException : public exception {
  public:
    RenderException(string msg) : msg(msg) { }
    virtual ~RenderException() throw() { }
    virtual const char* what() const throw() { return msg.c_str(); }
  private:
    const string msg;
};
#endif
