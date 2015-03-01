#ifndef FILEPARSER_H
#define FILEPARSER_H
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class Camera;
class Color;
class Light;
class Material;
class Matrix4;
class Point;
class Scene;
class Surface;
class Vector;
namespace Json { class Value; }

/** FileParser.
 * Parse JSON input file into domain objects.
 */
class FileParser {
  public:
    /** Parse scene node (and supporting objects, lights, materials). */
    unique_ptr<Scene> parseScene(const Json::Value& docNode) const;
    /** Parse camera node. */
    unique_ptr<Camera> parseCamera(const Json::Value& docNode) const;
  private:
    Light parseLight(const Json::Value& lightNode) const;
    unique_ptr<Surface> parseSurface(const Json::Value& surfacesNode, const unordered_map<string, shared_ptr<Material>>& materials) const;
    unordered_map<string, shared_ptr<Material>> parseMaterials(const Json::Value& materialNode) const;
    void parseContext(vector<unique_ptr<Surface>>& surfaces, Matrix4 obj2world,
      Json::Value contextNode, const unordered_map<string, Json::Value>& availableSurfaces,
      const unordered_map<string, shared_ptr<Material>>& materials) const;
    Matrix4 parseTransformation(const Json::Value& node) const;
    Json::Value findReference(const string& name, const unordered_map<string, Json::Value>& map) const;
    double deg2rad(double degrees) const;
    unordered_map<string, Json::Value> node2namemap(const Json::Value& node, const string& errStr) const;
    Point parsePoint(const Json::Value& node, const string& nodeName) const;
    Vector parseVector(const Json::Value& node, const string& nodeName) const;
    Color parseColor(const Json::Value& node, const string& nodeName) const;
    double parseDouble(const Json::Value& node, const string& nodeName) const;
    string parseString(const Json::Value& node, const string& nodeName) const;
    unsigned int parseUnsignedInt(const Json::Value& node, const string& nodeName) const;
    Color parseOptionalColor(const Json::Value& node, const string& nodeName, double defaultValue) const;
    double parseOptionalDouble(const Json::Value& node, double defaultValue) const;
};
#endif
