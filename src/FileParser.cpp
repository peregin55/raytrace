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
#include "Renderer.h"
#include "Scene.h"
#include "Sphere.h"
#include "Texture.h"
#include "TransformSurface.h"
#include "Triangle.h"
#include "Plane.h"
#include "Cube.h"
#include "Vector.h"
#include "Entity.h"
#include "CSGTree.h"
using namespace std;

struct EntityStruct {
  Json::Value entityNode;
  Matrix4 obj2world;
};

unique_ptr<Renderer> FileParser::parseRenderer(const Json::Value& root) const {
  Camera camera = parseCamera(root["scene"]["camera"]);
  unique_ptr<Scene> scene = parseScene(root);
  unique_ptr<Texture> envTexture = unique_ptr<Texture>(parseTexture(root["scene"]["environment"]["texture"], "environment texture"));
  Color envColor = parseOptionalColor(root["scene"]["environment"]["color"], "background color", 0.0);
  return unique_ptr<Renderer>(new Renderer(std::move(scene), camera, std::move(envTexture), envColor));
}

/** Parse and instantiate Camera from input file. */
Camera FileParser::parseCamera(const Json::Value& cameraNode) const {
  Point position = parsePoint(cameraNode["position"], "camera position");
  Point reference = parsePoint(cameraNode["reference"], "camera reference");
  Vector up = parseVector(cameraNode["up_direction"], "camera up direction");
  double bottom = parseDouble(cameraNode["frame"]["bottom"], "camera frame bottom");
  double top = parseDouble(cameraNode["frame"]["top"], "camera frame top");
  double left = parseDouble(cameraNode["frame"]["left"], "camera frame left");
  double right = parseDouble(cameraNode["frame"]["right"], "camera frame right");
  double near = parseDouble(cameraNode["frame"]["near"], "camera frame near");
  return Camera(position, reference, up, bottom, top, left, right, near);
}

/** Parse and instantiate Scene from input file. */
unique_ptr<Scene> FileParser::parseScene(const Json::Value& root) const {
  unordered_map<string, shared_ptr<Material>> materials = parseMaterials(root["materials"]);
  unordered_map<string, EntityStruct> entityStructMap;
  buildEntityStructMap(root["surfaces"], identity(), entityStructMap);
  vector<unique_ptr<Entity>> entities;
  string entityName;
  unique_ptr<Entity> entity;
  for (unsigned int i = 0; i < root["scene"]["surfaces"].size(); i++) {
    entityName = parseString(root["scene"]["surfaces"][i], "scene surface references");
    entity = buildEntity(entityName, entityStructMap, materials);
    entities.push_back(std::move(entity));
  }
  unsigned int maxTrace = parseUnsignedInt(root["scene"]["max_trace"], "max trace");
  vector<Light> lights = parseLights(root["scene"]["lights"]);
  return unique_ptr<Scene>(new Scene(lights, std::move(entities), maxTrace));
}



void FileParser::buildEntityStructMap(const Json::Value& node, const Matrix4& matrix, unordered_map<string, EntityStruct> entityStructMap) const {
  Json::Value const* transformNode = nullptr;
  for (unsigned int i = 0; i < node.size(); i++) {
    if (node[i]["transform"] != Json::nullValue) {
      if (transformNode != nullptr) {
        throw RenderException("Cannot have multiple 'transform' nodes in single block");
      }
      transformNode = &node[i];
    } else {
      buildEntityStruct(node[i], matrix, entityStructMap);
    }
  }
  if (transformNode) {
    buildEntityStructMap((*transformNode)["surface"], parseTransformation(*transformNode)*matrix, entityStructMap);
  }
}
    
void FileParser::buildEntityStruct(const Json::Value& node, const Matrix4& matrix, unordered_map<string, EntityStruct> entityStructMap) const {
  const Json::Value::const_iterator& v = node.begin();
  string name = parseString((*v)["name"], "entity name");
  EntityStruct es;
  es.entityNode = node;
  es.obj2world = matrix;
  entityStructMap[name] = es;
}


vector<Light> FileParser::parseLights(const Json::Value& lightsNode) const {
  vector<Light> lights;
  for (unsigned int i = 0; i < lightsNode.size(); i++) {
    Point p = parsePoint(lightsNode[i]["position"], "light position");
    Color c = parseColor(lightsNode[i]["color"], "light color");
    lights.emplace_back(p,c);
  }
  return lights;
}

unique_ptr<Entity> FileParser::buildEntity(const string& name, 
                                           const unordered_map<string, EntityStruct>& entityStructMap,
                                           const unordered_map<string, shared_ptr<Material>>& materials) const {
  const EntityStruct& es = entityStructMap.at(name);
  unique_ptr<Entity> surface = buildLocalEntity(es.entityNode, materials);
  if (es.obj2world == identity()) {
    return surface;
  } else {
    return unique_ptr<Entity>(new TransformSurface(es.obj2world, surface));
  }
}

unique_ptr<Surface> FileParser::buildLocalEntity(const Json::Value& surfaceNode,
  const unordered_map<string, shared_ptr<Material>>& materials) const {
  string name = parseString(surfaceNode["name"], "surface name");
  string type = parseString(surfaceNode["type"], "surface type");
  shared_ptr<Material> material;
  try {
    material = materials.at(parseString(surfaceNode["material"], type + " surface material"));
  } catch (const out_of_range& e) {
    throw RenderException("Unable to find surface material: " + name);
  }
  shared_ptr<Texture> texture = shared_ptr<Texture>(parseTexture(surfaceNode["texture"], "surface texture"));
  if (type == "sphere") {
    Point center = parsePoint(surfaceNode["center"], "sphere center");
    double radius = parseDouble(surfaceNode["radius"], "sphere radius");
    return unique_ptr<Surface>(new Sphere(material, texture, center, radius));
  }
  else if (type == "plane") {
    Point p0 = parsePoint(surfaceNode["vertex0"], "vertex0");
    Point p1 = parsePoint(surfaceNode["vertex1"], "vertex1");
    Point p2  = parsePoint(surfaceNode["vertex2"], "vertex2");
    return unique_ptr<Surface>(new Plane(material, texture, p0, p1, p2));
  }
  else if (type == "triangle") {
    Point p0 = parsePoint(surfaceNode["vertex0"], "vertex0");
    Point p1 = parsePoint(surfaceNode["vertex1"], "vertex1");
    Point p2 = parsePoint(surfaceNode["vertex2"], "vertex2");
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

void FileParser::parseContext(vector<unique_ptr<Entity>>& surfaces, Matrix4 obj2world,
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
  Matrix4 matrix = identity();
  for (unsigned int i = 0; i < transformNode.size(); i++) {
    if (transformNode["scale"] != Json::nullValue) {
      Json::Value node = transformNode["scale"];
      matrix = scale(node[(Json::Value::UInt)0].asDouble(),
        node[1].asDouble(), node[2].asDouble()) * matrix;
    } else if (transformNode["rotateX"] != Json::nullValue) {
      matrix = rotate(X, deg2rad(parseDouble(transformNode["rotateX"], "rotateX"))) * matrix;
    } else if (transformNode["rotateY"] != Json::nullValue) {
      matrix = rotate(Y, deg2rad(parseDouble(transformNode["rotateY"], "rotateY"))) * matrix;
    } else if (transformNode["rotateZ"] != Json::nullValue) {
      matrix = rotate(Z, deg2rad(parseDouble(transformNode["rotateZ"], "rotateZ"))) * matrix;
    } else if (transformNode["translate"] != Json::nullValue) {
      Json::Value node = transformNode["translate"];
      matrix = translate(node[(Json::Value::UInt)0].asDouble(), node[1].asDouble(), node[2].asDouble()) * matrix;
    } else {
      throw RenderException("unrecognized scene transformation");
    }
  }
  return matrix;
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

Texture* FileParser::parseTexture(const Json::Value& textureNode, const string& nodeName) const {
  Texture* texture = nullptr;
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
  return texture;
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
