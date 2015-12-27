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
#ifndef FILEPARSER_H
#define FILEPARSER_H
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

struct SurfaceStruct;
class Camera;
class Color;
class Light;
class Material;
class Matrix4;
class Point;
class Renderer;
class Scene;
class Surface;
class Texture;
class Vector;
namespace Json { class Value; }

/** FileParser.
 * Parse JSON input file into domain objects.
 */
class FileParser {
  public:
    FileParser(const string& dir) : dir(dir) { }
    unique_ptr<Renderer> parseRenderer(const Json::Value& root) const;
  private:
    unique_ptr<Scene> parseScene(const Json::Value& root) const;
    Camera parseCamera(const Json::Value& cameraNode) const;
    vector<Light> parseLights(const Json::Value& lightsNode) const;
    void buildSurfaceStruct(const Json::Value& node, const Matrix4& matrix, unordered_map<string, SurfaceStruct>& structMap) const;
    Matrix4 buildTransformMatrix(const Json::Value& node) const;
    void buildSurfaceStructMap(const Json::Value& node, const Matrix4& matrix, unordered_map<string, SurfaceStruct>& structMap) const;
    unique_ptr<Surface> buildSurface(const string& name, const unordered_map<string, SurfaceStruct>& structMap, const unordered_map<string, shared_ptr<Material>>& materials) const;
    unique_ptr<Surface> buildLocalSurface(const Json::Value& surfacesNode, const unordered_map<string, shared_ptr<Material>>& materials, const unordered_map<string, SurfaceStruct>& structMap) const;
    shared_ptr<Material> fetchMaterial(const Json::Value& materialNode, const unordered_map<string, shared_ptr<Material>>& materials) const;
    unordered_map<string, shared_ptr<Material>> parseMaterials(const Json::Value& materialNode) const;
    unique_ptr<Surface> buildCSGChild(const Json::Value& node, const string& name, const unordered_map<string, shared_ptr<Material>>& materials, const unordered_map<string, SurfaceStruct>& structMap) const;
    unique_ptr<Surface> buildCSG(const Json::Value& node, const string& name, const unordered_map<string, shared_ptr<Material>>& materials, const unordered_map<string, SurfaceStruct>& structMap) const;
    Matrix4 parseTransformation(const Json::Value& node) const;
    double deg2rad(double degrees) const;
    Point parsePoint(const Json::Value& node, const string& nodeName) const;
    Vector parseVector(const Json::Value& node, const string& nodeName) const;
    Color parseColor(const Json::Value& node, const string& nodeName) const;
    Color parseOptionalColor(const Json::Value& node, const string& nodeName, double defaultValue) const;
    Texture* parseTexture(const Json::Value& node, const string& nodename) const;
    double parseDouble(const Json::Value& node, const string& nodeName) const;
    string parseString(const Json::Value& node, const string& nodeName) const;
    unsigned int parseUnsignedInt(const Json::Value& node, const string& nodeName) const;
    double parseOptionalDouble(const Json::Value& node, double defaultValue) const;
    string dir;
};
#endif
