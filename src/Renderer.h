#ifndef RENDERER_H 
#define RENDERER_H
#include <memory>
#include <GL/glut.h>
#include "Camera.h"
#include "Scene.h"

class Color;

/** Renderer.
 * Renders image to image buffer given a camera and scene.
 */
class Renderer {
  public:
    Renderer(unique_ptr<Scene> scene, unique_ptr<Camera> camera) :
      scene(std::move(scene)), camera(std::move(camera)) { }
    unique_ptr<GLubyte[]> render(GLsizei height, GLsizei width) const;

  private:
    Point pixel2world(double x, double y, GLsizei height, GLsizei width) const;
    void setImage(GLubyte* image, int x, int y, GLsizei width, const Color& color) const;
    bool withinDelta(const Color& previousColor, const Color& color) const;
    Color supersample(int x, int y, const Color& origColor, GLsizei height, GLsizei width) const;

    unique_ptr<Scene> scene;
    unique_ptr<Camera> camera;
};

#endif
