#include "FileParser.h"
#include <cmath>
#include <limits>
#include <iostream>
#include <fstream>
#include "json/json.h"
#include "Camera.h"
#include "Cartesian.h"
#include "Color.h"
#include "Light.h"
#include "RenderException.h"
#include "Material.h"
#include "Matrix4.h"
#include "Scene.h"
#include "Sphere.h"
#include "Texture.h"
#include "TransformSurface.h"
#include "Triangle.h"
#include "Plane.h"
#include "Cube.h"
#include "Vector.h"
using namespace std;

/** Parse and instantiate Camera from input file. */
unique_ptr<Camera> FileParser::parseCamera(const Json::Value& docNode) const {
  Json::Value cameraNode = docNode["camera"];
  Point position = parsePoint(cameraNode["position"], "camera position");
  Point reference = parsePoint(cameraNode["reference"], "camera reference");
  Vector up = parseVector(cameraNode["up_direction"], "camera up direction");
  double bottom = parseDouble(cameraNode["frame"]["bottom"], "camera frame bottom");
  double top = parseDouble(cameraNode["frame"]["top"], "camera frame top");
  double left = parseDouble(cameraNode["frame"]["left"], "camera frame left");
  double right = parseDouble(cameraNode["frame"]["right"], "camera frame right");
  double near = parseDouble(cameraNode["frame"]["near"], "camera frame near");
  return unique_ptr<Camera>(new Camera(position, reference, up, bottom, top, left, right, near));
}

/** Parse and instantiate Scene from input file. */
unique_ptr<Scene> FileParser::parseScene(const Json::Value& docNode) const {
  unordered_map<string, Json::Value> availableLights = node2namemap(docNode["lights"], "light");
  unordered_map<string, Json::Value> availableSurfaces = node2namemap(docNode["surfaces"], "surface");
  unordered_map<string, shared_ptr<Material>> materials = parseMaterials(docNode["materials"]);
  Json::Value sceneNode = docNode["scene"];
  unsigned int maxTraces = parseUnsignedInt(sceneNode["max_trace"], "max trace");
  Color backgroundColor = parseColor(sceneNode["background_color"], "background color");
  vector<Light> lights;
  for (unsigned int i = 0; i < sceneNode["lights"].size(); i++) {
    string lightName = parseString(sceneNode["lights"][i], "scene light reference");
    lights.push_back(parseLight(findReference(lightName, availableLights)));
  }
  vector<unique_ptr<Surface>> surfaces;
  Matrix4 obj2world = identity();
  // root surfaces won't have any transformations, so build regular Surface instance
  if (sceneNode["surfaces"] != Json::nullValue) {
    for (unsigned int i=0; i < sceneNode["surfaces"].size(); i++) {
      string surfaceName = parseString(sceneNode["surfaces"][i], "scene surface reference");
      surfaces.push_back(parseSurface(findReference(surfaceName, availableSurfaces), materials));
    }
  }
  // surfaces in "context" may have local-object transformations, handle separately
  if (sceneNode["context"] != Json::nullValue) {
    parseContext(surfaces, obj2world, sceneNode["context"], availableSurfaces, materials);
  }
  return unique_ptr<Scene>(new Scene(lights, std::move(surfaces), backgroundColor, maxTraces));
}


Light FileParser::parseLight(const Json::Value& lightNode) const {
  Point p = parsePoint(lightNode["position"], "light position");
  Color c = parseColor(lightNode["color"], "light color");
  return Light(p,c);
}


unique_ptr<Surface> FileParser::parseSurface(const Json::Value& surfaceNode,
  const unordered_map<string, shared_ptr<Material>>& materials) const {
  string name = parseString(surfaceNode["name"], "surface name");
  string type = parseString(surfaceNode["type"], "surface type");
  shared_ptr<Material> material;
  try {
    material = materials.at(parseString(surfaceNode["material"], type + " surface material"));
  } catch (const out_of_range& e) {
    throw RenderException("Unable to find surface material: " + name);
  }
  Json::Value textureNode = surfaceNode["texture"];
  shared_ptr<Texture> texture;
  if (textureNode != Json::nullValue) {
    try {
      texture = Texture::fromFile(dir + textureNode.asString());
    } catch(RenderException e) {
      try {
        texture = Texture::fromFile(textureNode.asString());
      } catch(RenderException e2) {
        cerr << "Unable to load texture (" << e.what() << ", " << e2.what() << ")" << endl;
      }
    }
  }
  if (type == "sphere") {
    Point center = parsePoint(surfaceNode["center"], "sphere center");
    double radius = parseDouble(surfaceNode["radius"], "sphere radius");
    return unique_ptr<Surface>(new Sphere(material, texture, center, radius));
  }
  else if (type == "plane") {
    Point point = parsePoint(surfaceNode["point"], "point on plane");
    Vector normal = parseVector(surfaceNode["normal"], "surface normal");
    return unique_ptr<Surface>(new Plane(material, point, normal));
  }
  else if (type == "triangle") {
    Point p0 = parsePoint(surfaceNode["vertex1"], "vertex 1");
    Point p1 = parsePoint(surfaceNode["vertex2"], "vertex 2");
    Point p2 = parsePoint(surfaceNode["vertex3"], "vertex 3");
    return unique_ptr<Surface>(new Triangle(material, texture, p0, p1, p2));
  }
  else if (type == "cube") {
    double xmin = parseDouble(surfaceNode["xmin"], "x min");
    double xmax = parseDouble(surfaceNode["xmax"], "x max");
    double ymin = parseDouble(surfaceNode["ymin"], "y min");
    double ymax = parseDouble(surfaceNode["ymax"], "y max");
    double zmin = parseDouble(surfaceNode["zmin"], "z min");
    double zmax = parseDouble(surfaceNode["zmax"], "z max");
    return unique_ptr<Surface>(new Cube(material, texture, {xmin, ymin, zmin}, {xmax, ymax, zmax}));
  }
  else {
    throw RenderException("Unknown object: \"" + name + "\" of type: \"" + type + "\"");
  }
}

unordered_map<string, shared_ptr<Material>> FileParser::parseMaterials(const Json::Value& materialsNode) const {
  unordered_map<string, shared_ptr<Material>> materials;
  for (unsigned int i=0; i < materialsNode.size(); i++) {
    string name = parseString(materialsNode[i]["name"], "material name");
    Color ambientColor = parseColor(materialsNode[i]["ambient"]["color"], "ambient color");
    Color diffuseColor = parseColor(materialsNode[i]["diffuse"]["color"], "diffuse color");
    Color specularColor = parseColor(materialsNode[i]["specular"]["color"], "specular color");
    double specularExponent = parseDouble(materialsNode[i]["specular"]["exponent"], "specular exponent");
    double refractiveIndex = parseOptionalDouble(materialsNode[i]["refractive_index"], 1.0);
    Color refractiveAttenuation = parseOptionalColor(materialsNode[i]["refractive_attenuation"],
      "refractive attenuation", numeric_limits<double>::infinity());
    Color reflectiveFraction = parseOptionalColor(materialsNode[i]["reflective_fraction"], "reflective fraction", 0.0);
    materials[name] = shared_ptr<Material>(new Material(name, ambientColor, diffuseColor, specularColor, specularExponent,
      reflectiveFraction, refractiveIndex, refractiveAttenuation));
  }
  return materials;
}

void FileParser::parseContext(vector<unique_ptr<Surface>>& surfaces, Matrix4 obj2world,
  Json::Value contextNode, const unordered_map<string, Json::Value>& availableSurfaces,
  const unordered_map<string, shared_ptr<Material>>& materials) const {
  while (contextNode != Json::nullValue) {
    Json::Value transformNode = contextNode["transform"];
    if (transformNode != Json::nullValue) {
      for (unsigned int i = 0; i < transformNode.size(); i++) {
        obj2world = parseTransformation(transformNode[i]) * obj2world;
      }
    }
    Json::Value surfaceNamesNode = contextNode["surfaces"];
    if (surfaceNamesNode != Json::nullValue) {
      for (unsigned int i = 0; i < surfaceNamesNode.size(); i++) {
        string surfaceName = parseString(surfaceNamesNode[i], "scene surface reference");
        unique_ptr<Surface> surface = parseSurface(findReference(surfaceName, availableSurfaces), materials);
        unique_ptr<Surface> transformSurface(new TransformSurface(obj2world, std::move(surface)));
        surfaces.push_back(std::move(transformSurface));
      }
    }
    contextNode = contextNode["context"];
  }
}

Matrix4 FileParser::parseTransformation(const Json::Value& transformNode) const {
  if (transformNode["scale"] != Json::nullValue) {
    Json::Value node = transformNode["scale"];
    return scale(node[(Json::Value::UInt)0].asDouble(),
      node[1].asDouble(), node[2].asDouble());
  } else if (transformNode["rotateX"] != Json::nullValue) {
    return rotate(X, deg2rad(parseDouble(transformNode["rotateX"], "rotateX")));
  } else if (transformNode["rotateY"] != Json::nullValue) {
    return rotate(Y, deg2rad(parseDouble(transformNode["rotateY"], "rotateY")));
  } else if (transformNode["rotateZ"] != Json::nullValue) {
    return rotate(Z, deg2rad(parseDouble(transformNode["rotateZ"], "rotateZ")));
  } else if (transformNode["translate"] != Json::nullValue) {
    Json::Value node = transformNode["translate"];
    return translate(node[(Json::Value::UInt)0].asDouble(),
      node[1].asDouble(), node[2].asDouble());
  } else {
    throw RenderException("unrecognized scene transformation");
  }
}
 

Json::Value FileParser::findReference(const string& name, const unordered_map<string, Json::Value>& map) const {
  unordered_map<string, Json::Value>::const_iterator it = map.find(name);
  if (it == map.end()) {
    throw RenderException("unable to find reference \"" + name + "\"");
  }
  return it->second;
}

unordered_map<string, Json::Value> FileParser::node2namemap(const Json::Value& root, const string& errStr) const {
  unordered_map<string, Json::Value> map;
  for (unsigned int i = 0; i < root.size(); i++) {
    string name = parseString(root[i]["name"], errStr);
    map[name] = root[i];
  }
  return map;
}

double FileParser::deg2rad(double degrees) const {
  return degrees * M_PI / 180.0;
}

Point FileParser::parsePoint(const Json::Value& node, const string& nodeName) const {
  if (node == Json::nullValue ||
    node[(Json::Value::UInt)0] == Json::nullValue ||
    node[1] == Json::nullValue ||
    node[2] == Json::nullValue) {
     throw RenderException("unable to parse Point for \"" + nodeName + "\"");
   }
  return Point(node[(Json::Value::UInt)0].asDouble(),
               node[1].asDouble(),
               node[2].asDouble());
}

Vector FileParser::parseVector(const Json::Value& node, const string& nodeName) const {
  if (node == Json::nullValue ||
    node[(Json::Value::UInt)0] == Json::nullValue ||
    node[1] == Json::nullValue ||
    node[2] == Json::nullValue) {
     throw RenderException("unable to parse Vector for \"" + nodeName + "\"");
   }
  return Vector(node[(Json::Value::UInt)0].asDouble(),
                node[1].asDouble(),
                node[2].asDouble());
}
 
Color FileParser::parseColor(const Json::Value& node, const string& nodeName) const {
  if (node == Json::nullValue ||
    node[(Json::Value::UInt)0] == Json::nullValue ||
    node[1] == Json::nullValue ||
    node[2] == Json::nullValue) {
     throw RenderException("unable to parse Color for \"" + nodeName + "\"");
   }
  return Color(node[(Json::Value::UInt)0].asDouble(),
               node[1].asDouble(),
               node[2].asDouble());
}

Color FileParser::parseOptionalColor(const Json::Value& node, const string& nodeName, double defaultValue) const {
  if (node == Json::nullValue) {
    return Color(defaultValue, defaultValue, defaultValue);
  }
  else if (node[(Json::Value::UInt)0] == Json::nullValue ||
           node[1] == Json::nullValue ||
           node[2] == Json::nullValue) {
    throw RenderException("unable to parse Color for \"" + nodeName + "\"");
  }
  else {
    return Color(node[(Json::Value::UInt)0].asDouble(),
      node[1].asDouble(), node[2].asDouble());
  }
}

double FileParser::parseOptionalDouble(const Json::Value& node, double defaultValue) const {
  if (node == Json::nullValue) {
    return defaultValue;
  }
  else {
    return node.asDouble();
  }
}
    
double FileParser::parseDouble(const Json::Value& node, const string& nodeName) const {
  if (node == Json::nullValue) {
    throw RenderException("unable to parse double for \"" + nodeName + "\"");
  }
  return node.asDouble();
}

string FileParser::parseString(const Json::Value& node, const string& nodeName) const {
  if (node == Json::nullValue) {
    throw RenderException("unable to parse string for \"" + nodeName + "\"");
  }
  return node.asString();
}

unsigned int FileParser::parseUnsignedInt(const Json::Value& node, const string& nodeName) const {
  if (node == Json::nullValue) {
    throw RenderException("unable to parse string for \"" + nodeName + "\"");
  }
  return node.asUInt();
}
