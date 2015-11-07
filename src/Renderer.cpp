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
#include <chrono>
#include <thread>

const double Renderer::DELTA = 0.1;

unique_ptr<GLubyte[]> Renderer::render(GLsizei height, GLsizei width) const {
  chrono::time_point<chrono::system_clock> start = chrono::high_resolution_clock::now();
  GLsizei midx = width / 2;
  GLsizei midy = height / 2;
  unique_ptr<GLubyte[]> image(new GLubyte[height*width*3]);
  thread t1(&Renderer::renderPart, this, 0, midx, 0, midy, height, width, image.get());
  thread t2(&Renderer::renderPart, this, midx, width, 0, midy, height, width, image.get());
  thread t3(&Renderer::renderPart, this, 0, midx, midy, height, height, width, image.get());
  renderPart(midx, width, midy, height, height, width, image.get());
  t1.join();
  t2.join();
  t3.join();
  chrono::time_point<chrono::system_clock> end = chrono::high_resolution_clock::now();
  cerr << "time: " << chrono::duration_cast<chrono::milliseconds>(end-start).count() << "\n";
  return image;
}

void Renderer::renderPart(GLsizei startX, GLsizei stopX, GLsizei startY, GLsizei stopY, GLsizei height, GLsizei width, GLubyte* image) const {
  Color color;
  for (int y = startY; y < stopY; y++) {
    for (int x = startX; x < stopX; x++) {
      const Color& color = sceneColor(x, y, height, width);
      if (withinDelta(getImage(image, x-1, y, height, width), color) ||
          withinDelta(getImage(image, x, y-1, height, width), color)) {
        setImage(image, x, y, width, color);
      }
      else {
        setImage(image, x, y, width, supersample(x, y, color, height, width));
      }
    }
  }
}  

Color Renderer::sceneColor(double x, double y, GLsizei height, GLsizei width) const {
  Point p = pixel2world(x, y, height, width);
  Vector d = (p - camera.getPosition()).normalize();
  Color color;
  if (scene->calculateColor(Ray(p, d), color)) ;
  else if (backgroundTexture){
    unsigned int texx = fmax(fmin((x / width), 1.0), 0.0) * backgroundTexture->getWidth();
    unsigned int texy = fmax(fmin((y / height), 1.0), 0.0) * backgroundTexture->getHeight();
    color = backgroundTexture->fileColorAt(texx, backgroundTexture->getHeight() - 1 - texy);
  } else {
    color = backgroundColor;
  }
  return color;
}
 
// convert pixel coordinates to world coordinates.
// conceptually similar to walking backwards through graphics pipeline,
// but no canonical view volume/projection needed
Point Renderer::pixel2world(double x, double y, GLsizei height, GLsizei width) const {
  // screen coordinates (x,y) to camera coordinates
  double frameWidth = camera.getFrameRight() - camera.getFrameLeft();
  double frameHeight = camera.getFrameTop() - camera.getFrameBottom();
  double xcamera = x/width * frameWidth + camera.getFrameLeft();
  double ycamera = y/height * frameHeight + camera.getFrameBottom();
  Point pixel(xcamera, ycamera, camera.getFrameNear());
  // camera coordinates to world coordinates
  // use crossproduct+normalize to ensure orthonormal basis
  Vector w = (camera.getPosition() - camera.getReference()).normalize();
  Vector u = (camera.getUp().cross(w)).normalize();
  Vector v = w.cross(u).normalize();
  const Point& o = camera.getPosition();
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

Color Renderer::getImage(GLubyte* image, int x, int y, GLsizei height, GLsizei width) const {
  x = fmin(width, fmax(0.0, x));
  y = fmin(height, fmax(0.0, y));
  GLubyte* r = image + (y*width + x) * 3;
  return Color(*r / 255.0, *(r+1) / 255.0, *(r+2) / 255.0);
}

bool Renderer::withinDelta(const Color& previousColor, const Color& color) const {
  double rChange = (color.getRed() - previousColor.getRed()) / previousColor.getRed();
  double gChange = (color.getGreen() - previousColor.getGreen()) / previousColor.getGreen();
  double bChange = (color.getBlue() - previousColor.getBlue()) / previousColor.getBlue();
  return rChange < DELTA && rChange > -DELTA && gChange < DELTA && gChange > -DELTA &&
    bChange < DELTA && bChange > -DELTA;
}

Color Renderer::supersample(int x, int y, const Color& origColor, GLsizei height, GLsizei width) const {
  const Color& c1 = sceneColor(x - 0.75, y + 0.75, height, width);
  const Color& c2 = sceneColor(x + 0.75, y + 0.75, height, width);
  const Color& c3 = sceneColor(x - 0.75, y - 0.75, height, width);
  const Color& c4 = sceneColor(x + 0.75, y - 0.75, height, width);
  return (origColor + c1 + c2 + c3 + c4) / 5.0;
}
