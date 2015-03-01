#include "Texture.h"
#include "Color.h"
#include "RenderException.h"
#include "lodepng.h"

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


Color Texture::colorAt(int u, int v) const {
  int umod = u < 0 ? -(-u % width) : u % width;
  int vmod = v < 0 ? -(-v % height) : v % height;
  unsigned int utex = umod < 0 ? width + umod : umod;
  unsigned int vtex = vmod < 0 ? height + vmod : vmod;
  unsigned int i = utex * 3 + vtex * width * 3;
  //cout << "i is " + to_string(i) + " and vtex is " + to_string(vtex) + + " and v is " + to_string(v) << endl;
  return Color(image[i]/NORM, image[i+1]/NORM, image[i+2]/NORM);
}
