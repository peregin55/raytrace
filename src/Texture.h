#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
#include <vector>
using namespace std;

class Color;

/** Texture.
 */
class Texture {
  public:
    Texture(vector<unsigned char> image,
            unsigned int width,
            unsigned int height) :
        image(image), width(width), height(height) {}

    static Texture fromFile(const string& filename);
    Color colorAt(int u, int v) const;

  private:
    static const unsigned char BITDEPTH;
    static const double NORM;
    vector<unsigned char> image;
    unsigned int width;
    unsigned int height;
};
#endif
