#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
#include <memory>
#include <vector>
using namespace std;

class Color;

/** Texture.
 */
class Texture {
  public:
    Texture(const vector<unsigned char>& image,
            unsigned int width,
            unsigned int height) :
        image(image), width(width), height(height) {}
    static shared_ptr<Texture> fromFile(const string& filename);
    Color pixelColor(unsigned int x, unsigned int y) const;
    Color colorAt(double u, double v) const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
  private:
    static const unsigned char BITDEPTH;
    static const double NORM;
    static const double EPSILON;
    double bound(double x) const;
    vector<unsigned char> image;
    unsigned int width;
    unsigned int height;
};
#endif
