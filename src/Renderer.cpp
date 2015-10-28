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
#include "Renderer.h"
#include "Color.h"
#include "Ray.h"
#include "Matrix4.h"
#include "RenderException.h"
#include <cmath>
#include <string>
#include <time.h>

unique_ptr<GLubyte[]> Renderer::render(GLsizei height, GLsizei width) const {
  time_t start = time(NULL);
  GLubyte* image(new GLubyte[height*width*3]);
  Color previousColor;
  for (int y = 0; y < height; y++) {
    Color color = sceneColor(0, y, height, width);
    setImage(image, 0, y, width, color);
    for (int x = 1; x < width; x++) {
      previousColor = color;
      color = sceneColor(x, y, height, width);
      if (withinDelta(previousColor, color)) {
        setImage(image, x, y, width, color);
      }
      else {
        setImage(image, x, y, width, supersample(x, y, color, height, width));
      }
    }
  }
  time_t end = time(NULL);
  fprintf(stderr, "Total render time: %lf\n", difftime(end, start) * 1000.0);
  return unique_ptr<GLubyte[]>(image);
}  

Color Renderer::sceneColor(double x, double y, GLsizei height, GLsizei width) const {
  Point p = pixel2world(x, y, height, width);
  Vector d = (p - camera->getPosition()).normalize();
  unique_ptr<Color> color = scene->calculateColor(Ray(p, d));
  if (color) {
    return *color;
  } else if (backgroundTexture &&
            (unsigned int)x < backgroundTexture->getWidth() &&
            (unsigned int)y < backgroundTexture->getHeight()) {
    return backgroundTexture->fileColorAt((unsigned int)x,
      backgroundTexture->getHeight() - 1 - (unsigned int)y);
  } else {
    return backgroundColor;
  }
}
 
// convert pixel coordinates to world coordinates.
// conceptually similar to walking backwards through graphics pipeline,
// but no canonical view volume/projection needed
Point Renderer::pixel2world(double x, double y, GLsizei height, GLsizei width) const {
  // screen coordinates (x,y) to camera coordinates
  double frameWidth = camera->getFrameRight() - camera->getFrameLeft();
  double frameHeight = camera->getFrameTop() - camera->getFrameBottom();
  double xcamera = x/width * frameWidth + camera->getFrameLeft();
  double ycamera = y/height * frameHeight + camera->getFrameBottom();
  Point pixel(xcamera, ycamera, camera->getFrameNear());
  // camera coordinates to world coordinates
  // use crossproduct+normalize to ensure orthonormal basis
  Vector w = (camera->getPosition() - camera->getReference()).normalize();
  Vector u = (camera->getUp().cross(w)).normalize();
  Vector v = w.cross(u).normalize();
  const Point& o = camera->getPosition();
  Matrix4 cam2world(u[X], v[X], w[X], o[X],
                    u[Y], v[Y], w[Y], o[Y],
                    u[Z], v[Z], w[Z], o[Z],
                    0,    0,    0,    1);
  return cam2world * pixel;
}

void Renderer::setImage(GLubyte* image, int x, int y, GLsizei width, const Color& color) const {
  GLubyte* pixel = image + (y*width + x) * 3;
  *pixel++ = fmin(255, 255 * color.getRed());
  *pixel++ = fmin(255, 255 * color.getGreen());
  *pixel = fmin(255, 255 * color.getBlue());
}

bool Renderer::withinDelta(const Color& previousColor, const Color& color) const {
  double rChange = (color.getRed() - previousColor.getRed()) / previousColor.getRed();
  double gChange = (color.getGreen() - previousColor.getGreen()) / previousColor.getGreen();
  double bChange = (color.getBlue() - previousColor.getBlue()) / previousColor.getBlue();
  return rChange < 0.30 && rChange > -0.30 && gChange < 0.30 && gChange > -0.30 &&
    bChange < 0.30 && bChange > -0.30;
}

Color Renderer::supersample(int x, int y, const Color& origColor, GLsizei height, GLsizei width) const {
  Color c1 = sceneColor(x -0.4, y + 0.4, height, width);
  Color c2 = sceneColor(x + 0.4, y - 0.4, height, width);
  return (origColor + c1 + c2) / 3.0;
}
