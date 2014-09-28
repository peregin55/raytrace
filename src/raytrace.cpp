#include <iostream>
#include <fstream>
#include <memory>
#include <GL/glut.h>
#include "json/json.h"
#include "Camera.h"
#include "FileParser.h"
#include "Renderer.h"
#include "Scene.h"
using namespace std;

// size of image plane, number of pixels
static GLsizei width = 300;
static GLsizei height = 300;
// renderer
static unique_ptr<Renderer> renderer;
// contiguous 1D array of dimensions image[height][width][3]
static unique_ptr<GLubyte[]> image;

static void display() {
  image = std::move(renderer->render(height, width));
  glClear(GL_COLOR_BUFFER_BIT);
  glRasterPos3d(-1.0, -1.0, -1.0);  // x,y,z in world coordinates
  glPixelStorei(GL_PACK_ALIGNMENT, 1);  // use next available byte when packing image
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, image.get());
  glFlush();
}

static void reshape(int w, int h) {
  // store the new screen size
  width = (GLsizei)w;
  height = (GLsizei)h;
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
  Json::Value root;   // will contain the root value after parsing.
  Json::Reader reader;
  if (!reader.parse(ffin, root)) {
    cerr << "Failed to parse " << filename << "\n" << reader.getFormatedErrorMessages();
    return 1;
  }
  FileParser parser;
  unique_ptr<Scene> scene = parser.parseScene(root["scene"]);
  unique_ptr<Camera> camera = parser.parseCamera(root["camera"]);
  renderer = unique_ptr<Renderer>(new Renderer(std::move(scene), std::move(camera)));

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
