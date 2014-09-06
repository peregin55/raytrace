#ifndef HIT_H
#define HIT_H

class Surface;

class Hit {
  public:
    Hit(const Surface& s, double param) : surface(s), t(param) { }
    const Surface& getSurface() const;
    double getT() const;
  private:
    const Surface& surface;
    const double t;
};

#endif
