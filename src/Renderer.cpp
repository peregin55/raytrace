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
bool isDebug = false;
const double Renderer::AA_COLOR_THRESHOLD = 0.1;
const double Renderer::AA_RECURSE_THRESHOLD = 8;
static double gaussian(int x, unsigned int radius);

unique_ptr<GLubyte[]> Renderer::render(GLsizei height, GLsizei width) const {
  chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
  unique_ptr<GLubyte[]> image(new GLubyte[height*width*3]);
  //renderPart(0, 1, height, width, image.get());
  
  thread t1(&Renderer::renderPart, this, 0, 4, height, width, image.get());
  thread t2(&Renderer::renderPart, this, 1, 4, height, width, image.get());
  thread t3(&Renderer::renderPart, this, 2, 4, height, width, image.get());
  renderPart(3, 4, height, width, image.get());
  t1.join();
  t2.join();
  t3.join();
  
  chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
  cerr << "time: " << chrono::duration_cast<chrono::milliseconds>(end-start).count() << endl;
  //return this->filterImage(image.get(), height, width, 2, &gaussian);
  return image;
}

void Renderer::renderPart(unsigned int start, unsigned int delta, GLsizei height, GLsizei width, GLubyte* image) const {
  for (int y = 0; y < height; y++) {
    for (int x = start; x < width; x+=delta) {
      if (x == 150 && y == 150) {
        isDebug = false;
      } else {
        isDebug = false;
      }
      unordered_map<Point, Color> cache;
      setImage(image, x, y, width, sceneColor(x-0.5, x+0.5, y-0.5, y+0.5, cache, 0, height, width));
    }
  }
}

Color Renderer::sceneColor(double xmin, double xmax, double ymin, double ymax,
  unordered_map<Point, Color>& cache, unsigned int recurseCount, GLsizei height, GLsizei width) const {
  Point cen(xmin+0.5, ymin+0.5, 0.0);
  Point ll(cen[X] - (rand() % 50)/100.0, cen[Y] - (rand() % 50)/100.0, 0.0);
  Point lr(cen[X] + (rand() % 50)/100.0, cen[Y] - (rand() % 50)/100.0, 0.0);
  Point ur(cen[X] + (rand() % 50)/100.0, cen[Y] + (rand() % 50)/100.0, 0.0);
  Point ul(cen[X] - (rand() % 50)/100.0, cen[Y] + (rand() % 50)/100.0, 0.0);

  const Color& cenColor = sample(cen, cache, height, width);
  const Color& llColor  = sample(ll, cache, height, width);
  const Color& lrColor  = sample(lr, cache, height, width);
  const Color& urColor  = sample(ur, cache, height, width);
  const Color& ulColor  = sample(ul, cache, height, width);

  bool shouldTerminate = recurseCount >= AA_RECURSE_THRESHOLD;
  Color color;
  if (shouldTerminate || cenColor.within(llColor, AA_COLOR_THRESHOLD)) {
    color = color + (cenColor+llColor)/2.0;
  } else {
    color = color + sceneColor(ll[X], cen[X], ll[Y], cen[Y], cache, recurseCount+1, height, width);
  }
  if (shouldTerminate || cenColor.within(lrColor, AA_COLOR_THRESHOLD)) {
    color = color + (cenColor+lrColor)/2.0;
  } else {
    color = color + sceneColor(cen[X], lr[X], lr[Y], cen[Y], cache, recurseCount+1, height, width);
  }
  if (shouldTerminate || cenColor.within(urColor, AA_COLOR_THRESHOLD)) {
    color = color + (cenColor+urColor)/2.0;
  } else {
    color = color + sceneColor(cen[X], ur[X], cen[Y], ur[Y], cache, recurseCount+1, height, width);
  }
  if (shouldTerminate || cenColor.within(ulColor, AA_COLOR_THRESHOLD)) {
    color = color + (cenColor+ulColor)/2.0;
  } else {
    color = color + sceneColor(ul[X], cen[X], cen[Y], ul[Y], cache, recurseCount+1, height, width);
  }
  return color/4.0;
}



Color Renderer::sample(const Point& p, unordered_map<Point,Color>& cache, GLsizei height, GLsizei width) const {
  unordered_map<Point, Color>::const_iterator res = cache.find(p);
  Color color;
  if (res == cache.end()) {
    color = sample(p[X], p[Y], height, width);
    cache.emplace(p, color);
  } else {
    color = res->second;
  }
   return color;
}

Color Renderer::sample(double x, double y, GLsizei height, GLsizei width) const {
  Point p = pixel2world(x, y, height, width);
  Vector d = (p - camera.getPosition()).normalize();
  Color color;
  if (scene->calculateColor(Ray(p, d), color)) ;
  else if (backgroundTexture){
    unsigned int texx = fmax(fmin(((x+0.5) / width), 1.0), 0.0) * (backgroundTexture->getWidth() - 1);
    unsigned int texy = fmax(fmin(((y+0.5) / height), 1.0), 0.0) * (backgroundTexture->getHeight() - 1);
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


unique_ptr<GLubyte[]> Renderer::filterImage(GLubyte image[], GLsizei height, GLsizei width, GLsizei radius, double f(int, unsigned int)) const {
  unique_ptr<unique_ptr<Color>[]> cache = unique_ptr<unique_ptr<Color>[]>(new unique_ptr<Color>[width]());
  unique_ptr<GLubyte[]> newImage = unique_ptr<GLubyte[]>(new GLubyte[height * width * 3]());
  for (GLsizei y = 0; y < height; y++) {
    for (GLsizei x = 0; x < width; x++) {
      cache[x] = unique_ptr<Color>(new Color());
      for (GLsizei i = -radius; i <= radius; i++) {
        GLsizei yminusi = abs(y-i);
        if (yminusi >= height) {
          yminusi = height-1 - (yminusi-height);
        }
        const Color& color = getImage(image, x, yminusi, height, width);
        *cache[x] = *cache[x] + color * f(i, radius);
      }
    }
    for (GLsizei x = 0; x < width; x++) {
      for (GLsizei i = -radius; i <= radius; i++) {
        const Color& current = getImage(newImage.get(), x, y, height, width);
        GLsizei xminusi = abs(x-i);
        if (xminusi >= width) {
          xminusi = width-1 - (xminusi-width);
        }
        setImage(newImage.get(), x, y, width, current + (*cache[xminusi]) * f(i, radius));
      }
    }
  }
  return newImage;
}

static double gaussian(int x, unsigned int radius) {
  double stddev = fmax(0.5, radius / 3.0);  // 99.7% within 3 stddevs, so radius == 3*stddev
  double var = stddev * stddev;
  return exp(-x*x/(2.0*var)) / sqrt(2.0*M_PI*var);
}
