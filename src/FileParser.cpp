#include "FileParser.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "json/json.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "RenderException.h"
#include "Material.h"
#include "Scene.h"
#include "Sphere.h"
#include "Vector.h"
using namespace std;

unique_ptr<Camera> FileParser::parseCamera(Json::Value root) {
  Point position = parsePoint(root["position"], "camera position");
  Point reference = parsePoint(root["reference"], "camera reference");
  Vector up = parseVector(root["upDirection"], "camera upDirection");
  double bottom = parseDouble(root["frame"]["bottom"], "camera frame bottom");
  double top = parseDouble(root["frame"]["top"], "camera frame top");
  double left = parseDouble(root["frame"]["left"], "camera frame left");
  double right = parseDouble(root["frame"]["right"], "camera frame right");
  double near = parseDouble(root["frame"]["near"], "camera frame near");
  return unique_ptr<Camera>(new Camera(position, reference, up, bottom, top, left, right, near));
}

unique_ptr<Scene> FileParser::parseScene(Json::Value root) {
  vector<Light> lights = parseLights(root["lights"]);
  vector<shared_ptr<Surface>> surfaces = parseSurfaces(root["surfaces"]);
  unordered_map<string, Material> materialMap = parseMaterials(root["materials"]);
  return unique_ptr<Scene>(new Scene(lights, surfaces, materialMap));
}

unordered_map<string, Material> FileParser::parseMaterials(Json::Value materialsNode) {
  unordered_map<string, Material> map;
  for (int i=0; i < materialsNode.size(); i++) {
    string name = parseString(materialsNode[i]["name"], "material name");
    Color ambientColor = parseColor(materialsNode[i]["ambient"]["color"], "ambient color");
    Color diffuseColor = parseColor(materialsNode[i]["diffuse"]["color"], "diffuse color");
    Color specularColor = parseColor(materialsNode[i]["specular"]["color"], "specular color");
    double specularExponent = parseDouble(materialsNode[i]["specular"]["exponent"], "specular exponent");
    Material m(name, ambientColor, diffuseColor, specularColor, specularExponent);
    map[name] = m;
  }
  return map;
}

vector<Light> FileParser::parseLights(Json::Value lightNodes) {
  vector<Light> v;
  for (int i=0; i < lightNodes.size(); i++) {
    Point p = parsePoint(lightNodes[i]["position"], "light position");
    Color c = parseColor(lightNodes[i]["color"], "light color");
    v.push_back(Light(p,c));
  }
  return v;
}

vector<shared_ptr<Surface>> FileParser::parseSurfaces(Json::Value surfacesNode) {
  vector<shared_ptr<Surface>> surfaces;
  for (int i=0; i< surfacesNode.size(); i++) {
    string type = parseString(surfacesNode[i]["type"], "surface type");
    string materialName = parseString(surfacesNode[i]["material"], type + " surface material");
    if (type == "sphere") {
      Point center = parsePoint(surfacesNode[i]["center"], "sphere center");
      double radius = parseDouble(surfacesNode[i]["radius"], "sphere radius");
      surfaces.push_back(shared_ptr<Surface>(new Sphere(materialName, center, radius)));
    }
  }
  return surfaces;
}

Point FileParser::parsePoint(const Json::Value& node, const string& nodeName) {
  int i = 0;
  if (node == Json::nullValue ||
    node[i] == Json::nullValue ||
    node[1] == Json::nullValue ||
    node[2] == Json::nullValue) {
     throw RenderException("unable to parse Point for " + nodeName);
   }
  return Point(node[i].asDouble(),
               node[1].asDouble(),
               node[2].asDouble());
}

Vector FileParser::parseVector(const Json::Value& node, const string& nodeName) {
  int i = 0;
  if (node == Json::nullValue ||
    node[i] == Json::nullValue ||
    node[1] == Json::nullValue ||
    node[2] == Json::nullValue) {
     throw RenderException("unable to parse Vector for " + nodeName);
   }
  return Vector(node[i].asDouble(),
                node[1].asDouble(),
                node[2].asDouble());
}
 
Color FileParser::parseColor(const Json::Value& node, const string& nodeName) {
  int i = 0;
  if (node == Json::nullValue ||
    node[i] == Json::nullValue ||
    node[1] == Json::nullValue ||
    node[2] == Json::nullValue) {
     throw RenderException("unable to parse Color for " + nodeName);
   }
  return Color(node[i].asDouble(),
               node[1].asDouble(),
               node[2].asDouble());
}

double FileParser::parseDouble(const Json::Value& node, const string& nodeName) {
  if (node == Json::nullValue) {
    throw RenderException("unable to parse double for " + nodeName);
  }
  return node.asDouble();
}

string FileParser:: parseString(const Json::Value& node, const string& nodeName) {
  if (node == Json::nullValue) {
    throw RenderException("unable to parse string for " + nodeName);
  }
  return node.asString();
}
