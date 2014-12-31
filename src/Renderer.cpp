#include "Renderer.h"
#include "Color.h"
#include "Ray.h"
#include "Matrix4.h"
#include <cmath>

unique_ptr<GLubyte[]> Renderer::render(GLsizei height, GLsizei width) const {
  GLubyte* image(new GLubyte[height*width*3]);
  Color previousColor;
  for (int y = 0; y < height; y++) {
    Point p = pixel2world(0, y, height, width);
    Vector d = (p - camera->getPosition()).normalize();
    Color color = scene->calculateColor(Ray(p, d));
    setImage(image, 0, y, width, color);
    for (int x = 1; x < width; x++) {
      previousColor = color;
      Point p = pixel2world(x, y, height, width);
      Vector d = (p - camera->getPosition()).normalize();
      Color color = scene->calculateColor(Ray(p, d));
      if (withinDelta(previousColor, color)) {
        setImage(image, x, y, width, color);
      }
      else {
        setImage(image, x, y, width, supersample(x, y, color, height, width));
      }
    }
  }
  return unique_ptr<GLubyte[]>(image);
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
  return rChange < 0.50 && rChange > -0.50 && gChange < 0.50 && gChange > -0.50 &&
    bChange < 0.50 && bChange > -0.50;
}

Color Renderer::supersample(int x, int y, const Color& origColor, GLsizei height, GLsizei width) const {
  Point p1 = pixel2world(x - 0.4, y + 0.4, height, width);
  Vector d1 = (p1 - camera->getPosition()).normalize();
  Color c1 = scene->calculateColor(Ray(p1, d1));

  Point p2 = pixel2world(x + 0.4, y - 0.4, height, width);
  Vector d2 = (p2 - camera->getPosition()).normalize();
  Color c2 = scene->calculateColor(Ray(p2, d2));

  return (origColor + c1 + c2) / 3.0;
}
