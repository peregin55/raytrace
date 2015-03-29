#include "Texture.h"
#include "Color.h"
#include "RenderException.h"
#include "lodepng.h"
#include <cmath>
#include <string>

const unsigned char Texture::BITDEPTH = 8;
const double Texture::NORM = 255.0;
const double Texture::EPSILON = 1.0e-10;

Texture* Texture::fromFile(const string& filename) {
  vector<unsigned char> image;
  unsigned int width;
  unsigned int height;
  unsigned int error = lodepng::decode(image, width, height, filename, LodePNGColorType::LCT_RGB, BITDEPTH);
  if (error) {
    throw RenderException("Error reading png file: \"" + filename + "\" - error code " + to_string(error));
  }
  return new Texture(image, width, height);
}

Color Texture::fileColorAt(unsigned int x, unsigned int y) const {
  if (x >= width || y >= height) {
    throw RenderException("Invalid color location: (" + to_string(x) + ", " + to_string(y)  + ")");
  }
  unsigned int i = x * 3 + y * width * 3;
  return Color(image[i]/NORM, image[i+1]/NORM, image[i+2]/NORM);
}

/** u,v ranges from [0,1], origin at bottom left of image. Uses bilinear interpolation
 * http://supercomputingblog.com/graphics/coding-bilinear-interpolation/
 */
Color Texture::colorAt(double u, double v) const {
  if (u > (1.0 + EPSILON) || u < -EPSILON) {
    throw RenderException("invalid u = " + to_string(u));
  }
  if (v > (1.0 + EPSILON) || v < -EPSILON) {
    throw RenderException("invalid v = " + to_string(v));
  }
  // orient x,y to upper left of texture
  double x = bound(u) * (width-1);
  double y = bound(1.0-v) * (height-1);
  unsigned int x1 = x;
  unsigned int y1 = y;
  unsigned int x2 = ceil(x);
  unsigned int y2 = ceil(y);
  if (x1 == x2 && y1 == y2) {
    return fileColorAt(x, y);
  }
  Color c11 = fileColorAt(x1, y1);
  Color c12 = fileColorAt(x1, y2);
  Color c21 = fileColorAt(x2, y1);
  Color c22 = fileColorAt(x2, y2);
  if (y1 == y2) {
    double w1 = (x2 - x) / (x2 - x1);
    double w2 = (x - x1) / (x2 - x1);
    return (c11 * w1) + (c21 * w2);
  } else if (x1 == x2) {
    double w1 = (y2 - y) / (y2 - y1);
    double w2 = (y - y1) / (y2 - y1);
    return (c11 * w1) +  (c12 * w2);
  } else {
    double w1 = (x2 - x) / (x2 - x1);
    double w2 = (x - x1) / (x2 - x1);
    Color c1 = (c11 * w1) + (c21 * w2);
    Color c2 = (c12 * w1) + (c22 * w2);
    return c1 * (y2 - y) / (y2 - y1) + c2 * (y - y1) / (y2 - y1);
  }
}

double Texture::bound(double x) const {
  if (x > 0.0) {
    return fmin(1.0, x);
  }
  return 0.0;
}


unsigned int Texture::getWidth() const {
  return width;
}

unsigned int Texture::getHeight() const {
  return height;
}
