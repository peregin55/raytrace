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
#include "CSG.h"
using namespace std;

struct SurfaceStruct {
  Json::Value node;
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
  unordered_map<string, SurfaceStruct> structMap;
  buildSurfaceStructMap(root["surfaces"], identity(), structMap);
  vector<unique_ptr<Surface>> surfaces;
  string surfaceName;
  unique_ptr<Surface> surface;
  for (unsigned int i = 0; i < root["scene"]["surfaces"].size(); i++) {
    surfaceName = parseString(root["scene"]["surfaces"][i], "scene surface references");
    surface = buildSurface(surfaceName, structMap, materials);
    surfaces.push_back(std::move(surface));
  }
  unsigned int maxTrace = parseUnsignedInt(root["scene"]["max_trace"], "max trace");
  vector<Light> lights = parseLights(root["scene"]["lights"]);
  return unique_ptr<Scene>(new Scene(lights, std::move(surfaces), maxTrace));
}



void FileParser::buildSurfaceStructMap(const Json::Value& node, const Matrix4& matrix, unordered_map<string, SurfaceStruct>& structMap) const {
  for (unsigned int i = 0; i < node.size(); i++) {
    if (node[i]["transform"] != Json::nullValue) {
      buildSurfaceStructMap(node[i]["surfaces"], parseTransformation(node[i]["transform"])*matrix, structMap);
    } else {
      buildSurfaceStruct(node[i], matrix, structMap);
    }
  }
}
    
void FileParser::buildSurfaceStruct(const Json::Value& node, const Matrix4& matrix, unordered_map<string, SurfaceStruct>& structMap) const {
  const Json::Value::const_iterator& v = node.begin();
  string name = parseString((*v)["name"], "entity name");
  SurfaceStruct s;
  s.node = node;
  s.obj2world = matrix;
  structMap[name] = s;
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

unique_ptr<Surface> FileParser::buildSurface(const string& name, 
                                             const unordered_map<string, SurfaceStruct>& structMap,
                                             const unordered_map<string, shared_ptr<Material>>& materials) const {
  try {
    const SurfaceStruct& ss = structMap.at(name);
    unique_ptr<Surface> surface = buildLocalSurface(ss.node, materials, structMap);
    if (ss.obj2world == identity()) {
      return surface;
    } else {
      return unique_ptr<Surface>(new TransformSurface(ss.obj2world, std::move(surface)));
    }
  } catch (const out_of_range& e) {
    throw RenderException("Unable to find surface: \"" + name + "\"");
  }
}



unique_ptr<Surface> FileParser::buildLocalSurface(const Json::Value& surfaceNode,
  const unordered_map<string, shared_ptr<Material>>& materials,
  const unordered_map<string, SurfaceStruct>& structMap) const {
  Json::Value::const_iterator first = surfaceNode.begin();
  string name = parseString((*first)["name"], "surface name");
  shared_ptr<Texture> texture = shared_ptr<Texture>(parseTexture((*first)["texture"], "surface texture"));
  if (surfaceNode["sphere"] != Json::nullValue) {
    Json::Value dataNode = surfaceNode["sphere"];
    shared_ptr<Material> material = fetchMaterial(dataNode["material"], materials);
    Point center = parsePoint(dataNode["center"], "sphere center");
    double radius = parseDouble(dataNode["radius"], "sphere radius");
    return unique_ptr<Surface>(new Sphere(material, texture, center, radius));
  }
  else if (surfaceNode["plane"] != Json::nullValue) {
    Json::Value dataNode = surfaceNode["plane"];
    shared_ptr<Material> material = fetchMaterial(dataNode["material"], materials);
    Point p0 = parsePoint(dataNode["vertex0"], "vertex0");
    Point p1 = parsePoint(dataNode["vertex1"], "vertex1");
    Point p2  = parsePoint(dataNode["vertex2"], "vertex2");
    return unique_ptr<Surface>(new Plane(material, texture, p0, p1, p2));
  }
  else if (surfaceNode["triangle"] != Json::nullValue) {
    Json::Value dataNode = surfaceNode["triangle"];
    shared_ptr<Material> material = fetchMaterial(dataNode["material"], materials);
    Point p0 = parsePoint(dataNode["vertex0"], "vertex0");
    Point p1 = parsePoint(dataNode["vertex1"], "vertex1");
    Point p2 = parsePoint(dataNode["vertex2"], "vertex2");
    return unique_ptr<Surface>(new Triangle(material, texture, p0, p1, p2));
  }
  else if (surfaceNode["cube"] != Json::nullValue) {
    Json::Value dataNode = surfaceNode["cube"];
    shared_ptr<Material> material = fetchMaterial(dataNode["material"], materials);
    double xmin = parseDouble(dataNode["xmin"], "x min");
    double xmax = parseDouble(dataNode["xmax"], "x max");
    double ymin = parseDouble(dataNode["ymin"], "y min");
    double ymax = parseDouble(dataNode["ymax"], "y max");
    double zmin = parseDouble(dataNode["zmin"], "z min");
    double zmax = parseDouble(dataNode["zmax"], "z max");
    return unique_ptr<Surface>(new Cube(material, texture, {xmin, ymin, zmin}, {xmax, ymax, zmax}));
  }
  else if (surfaceNode["csg"] != Json::nullValue) {
    Json::Value op = surfaceNode["csg"]["op"];
    return buildCSG(op, name, materials, structMap);
  } 
  else {
    throw RenderException("Unknown object: " + name);
  }
}

unique_ptr<Surface> FileParser::buildCSGChild(const Json::Value& node,
                                              const string& name,
                                              const unordered_map<string, shared_ptr<Material>>& materials,
                                              const unordered_map<string, SurfaceStruct>& structMap) const {
  if (node.type() == Json::stringValue) {
    return buildSurface(parseString(node, "csg surface element " + name), structMap, materials);
  } else {
    return buildCSG(node, name, materials, structMap);
  }
}

unique_ptr<Surface> FileParser::buildCSG(const Json::Value& node,
                                         const string& name,
                                         const unordered_map<string, shared_ptr<Material>>& materials,
                                         const unordered_map<string, SurfaceStruct>& structMap) const {
  if (node["subtract"] != Json::nullValue) {
    unique_ptr<Surface> left = buildCSGChild(node["subtract"][(Json::Value::UInt)0], name, materials, structMap);
    unique_ptr<Surface> right = buildCSGChild(node["subtract"][1], name, materials, structMap);
    unique_ptr<CSG> leftCSG(new CSG(NONE, std::move(left), nullptr, nullptr));
    unique_ptr<CSG> rightCSG(new CSG(NONE, std::move(right), nullptr, nullptr));
    return unique_ptr<Surface>(new CSG(SUBTRACT, nullptr, std::move(leftCSG), std::move(rightCSG)));
  }
  else if (node["intersect"] != Json::nullValue) {
    unique_ptr<Surface> left = buildCSGChild(node["intersect"][(Json::Value::UInt)0], name, materials, structMap);
    unique_ptr<Surface> right = buildCSGChild(node["intersect"][1], name, materials, structMap);
    unique_ptr<CSG> leftCSG(new CSG(NONE, std::move(left), nullptr, nullptr));
    unique_ptr<CSG> rightCSG(new CSG(NONE, std::move(right), nullptr, nullptr));
    return unique_ptr<Surface>(new CSG(INTERSECT, nullptr, std::move(leftCSG), std::move(rightCSG)));
  }
  else if (node["union"] != Json::nullValue) {
    unique_ptr<Surface> left = buildCSGChild(node["union"][(Json::Value::UInt)0], name, materials, structMap);
    unique_ptr<Surface> right = buildCSGChild(node["union"][1], name, materials, structMap);
    unique_ptr<CSG> leftCSG(new CSG(NONE, std::move(left), nullptr, nullptr));
    unique_ptr<CSG> rightCSG(new CSG(NONE, std::move(right), nullptr, nullptr));
    return unique_ptr<Surface>(new CSG(UNION, nullptr, std::move(leftCSG), std::move(rightCSG)));
  } 
  else {
    throw RenderException("Unknown csg operation for: " + name);
  }
}

shared_ptr<Material> FileParser::fetchMaterial(const Json::Value& materialNode, const unordered_map<string, shared_ptr<Material>>& materials) const {
  string name = parseString(materialNode, "material name");
  try {
    return materials.at(name);
  } catch (const out_of_range& e) {
    throw RenderException("Unable to find material: \"" + name + "\"");
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


Matrix4 FileParser::parseTransformation(const Json::Value& transformNode) const {
  Matrix4 matrix = identity();
  if (transformNode.type() != Json::arrayValue) {
    throw RenderException("transform node must be array");
  }
  for (unsigned int i = 0; i < transformNode.size(); i++) {
    if (transformNode[i]["scale"] != Json::nullValue) {
      Json::Value node = transformNode[i]["scale"];
      matrix = scale(node[(Json::Value::UInt)0].asDouble(),
        node[1].asDouble(), node[2].asDouble()) * matrix;
    } else if (transformNode[i]["rotateX"] != Json::nullValue) {
      matrix = rotate(X, deg2rad(parseDouble(transformNode[i]["rotateX"], "rotateX"))) * matrix;
    } else if (transformNode[i]["rotateY"] != Json::nullValue) {
      matrix = rotate(Y, deg2rad(parseDouble(transformNode[i]["rotateY"], "rotateY"))) * matrix;
    } else if (transformNode[i]["rotateZ"] != Json::nullValue) {
      matrix = rotate(Z, deg2rad(parseDouble(transformNode[i]["rotateZ"], "rotateZ"))) * matrix;
    } else if (transformNode[i]["translate"] != Json::nullValue) {
      Json::Value node = transformNode[i]["translate"];
      matrix = translate(node[(Json::Value::UInt)0].asDouble(), node[1].asDouble(), node[2].asDouble()) * matrix;
    } else {
      throw RenderException("unrecognized scene transformation");
    }
  }
  return matrix;
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
