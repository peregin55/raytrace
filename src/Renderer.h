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
#ifndef RENDERER_H 
#define RENDERER_H
#include <memory>
#include <GL/glut.h>
#include "Camera.h"
#include "Scene.h"
#include "Texture.h"

class Color;

/** Renderer.
 * Renders image to image buffer given a camera and scene.
 */
class Renderer {
  public:
    Renderer(unique_ptr<Scene> scene,
             const Camera& camera,
             unique_ptr<Texture> backgroundTexture,
             const Color& backgroundColor) :
      scene(std::move(scene)),
      camera(camera),
      backgroundTexture(std::move(backgroundTexture)),
      backgroundColor(backgroundColor) { }
    unique_ptr<GLubyte[]> render(GLsizei height, GLsizei width) const;

  private:
    void renderPart(GLsizei startX, GLsizei stopX, GLsizei startY, GLsizei stopY, GLsizei hieght, GLsizei width, GLubyte* image) const;
    Color sceneColor(double x, double y, GLsizei height, GLsizei width) const;
    Point pixel2world(double x, double y, GLsizei height, GLsizei width) const;
    void setImage(GLubyte* image, int x, int y, GLsizei width, const Color& color) const;
    Color getImage(GLubyte* image, int x, int y, GLsizei height, GLsizei width) const;
    bool withinDelta(const Color& previousColor, const Color& color) const;
    Color supersample(int x, int y, const Color& origColor, GLsizei height, GLsizei width) const;

    unique_ptr<Scene> scene;
    Camera camera;
    unique_ptr<Texture> backgroundTexture;
    Color backgroundColor;
    const static double DELTA;
};

#endif
