#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/glut.h>
#include "json/json.h"
#include "Camera.h"
#include "Color.h"
#include "Point.h"
#include "FileParser.h"
#include "Hit.h"
#include "Material.h"
#include "Matrix4.h"
#include "Ray.h"
#include "Sphere.h"
#include "Scene.h"
#include "Vector.h"
using namespace std;

// size of image plane, number of pixels
static GLsizei width = 300;
static GLsizei height = 300;
// contiguous 1D array of dimensions image[height][width][3]
static GLubyte* image;
static unique_ptr<Scene> scene;
static unique_ptr<Camera> camera;

bool shouldDebug = false;

// convert pixel coordinates to world coordinates.
// conceptually similar to walking backwards through graphics pipeline,
// but no canonical view volume/projection needed
static Point pixel2world(int x, int y) {
  // screen coordinates (x,y) to camera coordinates
  double frameWidth = camera->getFrameRight() - camera->getFrameLeft();
  double frameHeight = camera->getFrameTop() - camera->getFrameBottom();
  double xcamera = ((double)x)/width * frameWidth + camera->getFrameLeft();
  double ycamera = ((double)y)/height * frameHeight + camera->getFrameBottom();
  Point pixel(xcamera, ycamera, camera->getFrameNear());
  // camera coordinates to world coordinates
  Vector w = (camera->getPosition() - camera->getReference()).normalized();
  Vector u = (camera->getUp().cross(w)).normalized();
  Vector v = w.cross(u).normalized();
  const Point& o = camera->getPosition();
  Matrix4 cam2world(u[X], v[X], w[X], o[X],
                    u[Y], v[Y], w[Y], o[Y],
                    u[Z], v[Z], w[Z], o[Z],
                    0,    0,    0,    1);
  return cam2world * pixel;
}
                     
static void render() {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      /*
      if (x == 150 && y == 150) {
        shouldDebug = true;
      }
      else {
        shouldDebug = false;
      }
      */
      Point p = pixel2world(x, y);
      if (shouldDebug) {
        cerr << "camera point is " << camera->getPosition() << "\n";
        cerr << "screen (0, 150)  in world is " << pixel2world(0, 150) << "\n";
        cerr << "screen (" << x << ", " << y << ") in world is " << p << "\n";
        cerr << "screen (" << width << ", " << "150) in world is " << pixel2world(width, 150) << "\n";
      }
      Vector d = (p - camera->getPosition()).normalized();
      Ray ray(p, d);
      Color color = scene->render(ray);
      if (shouldDebug) {
        color = Color(0.0, 0.0, 0.0);
      }
      GLubyte* pixel = image + (y*width + x) * 3;
      *pixel++ = fmin(255, 255 * color.getRed());
      *pixel++ = fmin(255, 255 * color.getGreen());
      *pixel = fmin(255, 255 * color.getBlue());
    }
  }
}

static void display() {
  render();
  glClear(GL_COLOR_BUFFER_BIT);
  glRasterPos3d(-1.0, -1.0, -1.0);  // x,y,z in world coordinates
  glPixelStorei(GL_PACK_ALIGNMENT, 1);  // use next available byte when packing image
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
  glFlush();
}

static void reshape(int w, int h) {
  width = (GLsizei)w;
  height = (GLsizei)h;

  delete[] image;
  image = new GLubyte[width*height*3];

  // set clear color to black
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // setup orthogonal view clipping box
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, 0.1, 2.0);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "raytrace <scene-file>\n";
    return 1;
  }
  char* filename = *(argv+1);
  ifstream ffin(filename);
  if (!ffin.is_open()) {
    cerr << "Cannot open file " << filename << "\n";
    return 1;
  }
  Json::Value root;   // will contains the root value after parsing.
  Json::Reader reader;
  if (!reader.parse(ffin, root)) {
    cerr << "Failed to parse " << filename << "\n" << reader.getFormatedErrorMessages();
    return 1;
  }

  FileParser parser;
  scene = parser.parseScene(root["scene"]);
  camera = parser.parseCamera(root["camera"]);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("raytrace");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}
