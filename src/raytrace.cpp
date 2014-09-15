#include <iostream>
#include <fstream>
#include <math.h>
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

// projections from point of view of camera
// far plane has to be negative (ie in front of camera)
// note that projF and camera[Z] are related:
// the image plane is at Z=0, which is against the far side
// of the canonical-view-volume (aligned with pos-x,pos-y).
// when this is translated to camera-space, the far side 
// becomes the far plane
//static const double projL(-5.0),projB(-5.0),projF(5.0);
//static const double projR(5.0),projT(5.0),projN(6.0);
static GLsizei width = 300;
static GLsizei height = 300;
// contiguous 1D array of dimensions image[height][width][3]
static GLubyte* image;
static unique_ptr<Scene> scene;
static unique_ptr<Camera> camera;

bool shouldDebug = false;

// do inverse orthographic projection, ignoring z since this is a plane
// as last step, translate plane to correct z-value
static Point pixel2world(int x, int y) {
  Point pixel = Point(x, y, 0);
  Matrix4 inverseViewport = translate(-1.0, -1.0, 0.0) * scale(2.0/width, 2.0/height, 1.0) * translate(0.5, 0.5, 0.0);
  Matrix4 inverseProj = translate(camera->getFrameLeft(), camera->getFrameBottom(), 0.0) *
                        scale((camera->getFrameRight() - camera->getFrameLeft())/2.0, 
                              (camera->getFrameTop() - camera->getFrameBottom())/2.0, 1.0) * translate(1.0, 1.0, 0.0);
  Vector w = -(camera->getReference() - camera->getPosition()).normalized();
  Vector u = (camera->getUp().cross(w)).normalized();
  Vector v = w.cross(u);
  Matrix4 inverseCam = translate(camera->getPosition()[X], camera->getPosition()[Y], camera->getPosition()[Z]) *
     Matrix4(u[X], v[X], w[X], 0,
            u[Y], v[Y], w[Y], 0,
            u[Z], v[Z], w[Z], 0, 
            0,    0,    0,    1);
  // treated inverseViewport, inverseProj as 2-dimensions, now take near(Z) into account
  Matrix4 translateToNear = translate(0.0, 0.0, camera->getFrameNear());
  return translateToNear * inverseCam * inverseProj * inverseViewport * pixel;
}
                     
/*
static Point pixel2world(int x, int y, const Point& camera, const Point& gaze, const Vector& camUp) {
  Point pixel = Point(x, y, 0.0);
  Matrix4 inverseViewport = translate(-1.0, -1.0, 0.0) * scale(2.0/width, 2.0/height, 1.0) * translate(0.5, 0.5, 0.0);
  Matrix4 inverseProj = translate(projL, projB, projF) * scale((projR-projL)/2.0, (projT-projB)/2.0, (projN-projF)/2.0) * translate(1.0, 1.0, 1.0);
  Vector w = -(gaze-camera).normalized();
  Vector u = (camUp.cross(w)).normalized();
  Vector v = w.cross(u);
  Matrix4 inverseCam = translate(camera[X], camera[Y], camera[Z]) *
    Matrix4(u[X], v[X], w[X], 0,
            u[Y], v[Y], w[Y], 0,
            u[Z], v[Z], w[Z], 0, 
            0,    0,    0,    1);
  return inverseCam * inverseProj * inverseViewport * pixel;
}
*/

static void render() {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (x == 150 && y == 150) {
        shouldDebug = true;
      }
      else {
        shouldDebug = false;
      }
      Point p = pixel2world(x, y);

      if (shouldDebug) {
        cerr << "camera point is " << camera->getPosition() << "\n";
        cerr << "screen (0, 150)  in world is " << pixel2world(0, 150) << "\n";
        cerr << "screen (" << width << ", " << "150) in world is " << pixel2world(width, 150) << "\n";
        cerr << "screen (" << x << ", " << y << ") in world is " << p << "\n";
      }

      Vector d = (p - camera->getPosition()).normalized();
      Ray ray(p, d);
      Color color = scene->render(ray);
      /*
      if (color.getRed() == 0.4 && color.getGreen() == 0.4 && color.getBlue() == 0.4) {
        cerr << x << ", " << y << "\n";
      }
      */
      if (shouldDebug) {
        color = Color(0.0, 0.0, 0.0);
      }
      GLubyte* pixel = image + (y*width + x) * 3;
      *pixel++ = fmin(255, 255 * color.getRed());
      *pixel++ = fmin(255, 255 * color.getGreen());
      *pixel = fmin(255, 255 * color.getBlue());
    }
  }
 
  /*
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      Point p(x, y, 0.0);
      Vector d = p - camera;
      Ray ray(p, d);
      Hit h = sphere.intersect(ray, 0, 1000.0);
      GLubyte* pixel = image + (y*width + x) * 3;
      if (h.getT() > 0) {
        *pixel++ = 255;
        *pixel++ = 255;
        *pixel = 255;
      }
      else {
        *pixel++ = 0;
        *pixel++ = 0;
        *pixel = 0;
      }
    }
  }
  */
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
