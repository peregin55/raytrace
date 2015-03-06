#include "Texture.h"
#include "Color.h"
#include "RenderException.h"
#include "lodepng.h"
#include <cmath>

const unsigned char Texture::BITDEPTH = 8;
const double Texture::NORM = 255.0;

unique_ptr<Texture> Texture::fromFile(const string& filename) {
  vector<unsigned char> image;
  unsigned int width;
  unsigned int height;
  unsigned int error = lodepng::decode(image, width, height, filename, LodePNGColorType::LCT_RGB, BITDEPTH);
  if (error) {
    throw RenderException("Error reading png file: \"" + filename + "\" - error code " + to_string(error));
  }
  return unique_ptr<Texture>(new Texture(image, width, height));
}

Color Texture::pixelColor(unsigned int x, unsigned int y) const {
  if (x >= width || y >= height) {
    throw RenderException("Invalid color location: (" + to_string(x) + ", " + to_string(y)  + ")");
  }
  unsigned int i = x * 3 + y * width * 3;
  return Color(image[i]/NORM, image[i+1]/NORM, image[i+2]/NORM);
}

/** u,v ranges from [0,1]. Uses bilinear interpolation
 * http://supercomputingblog.com/graphics/coding-bilinear-interpolation/
 */
Color Texture::colorAt(double u, double v) const {
  double x = u * (width-1);
  double y = v * (height-1);
  unsigned int x1 = x;
  unsigned int y1 = y;
  unsigned int x2 = ceil(x);
  unsigned int y2 = ceil(y);
  if (x1 == x2 || y1 == y2) {
    return pixelColor(x, y);
  }
  Color c11 = pixelColor(x1, y1);
  Color c12 = pixelColor(x1, y2);
  Color c21 = pixelColor(x2, y1);
  Color c22 = pixelColor(x2, y2);
  double w1 = (x2 - x) / (x2 - x1);
  double w2 = (x - x1) / (x2 - x1);
  Color c1 = (c11 * w1) + (c21 * w2);
  Color c2 = (c12 * w1) + (c22 * w2);
  return c1 * (y2 - y) / (y2 - y1) + c2 * (y - y1) / (y2 - y1);
}

unsigned int Texture::getWidth() const {
  return width;
}

unsigned int Texture::getHeight() const {
  return height;
}
