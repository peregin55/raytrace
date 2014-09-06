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
class Point;
class Scene;
class Surface;
class Vector;
namespace Json { class Value; }

class FileParser {
  public:
    unique_ptr<Scene> parseScene(Json::Value sceneRoot);
    unique_ptr<Camera> parseCamera(Json::Value cameraRoot);
  private:
    vector<Light> parseLights(Json::Value lightNodes);
    vector<shared_ptr<Surface>> parseSurfaces(Json::Value surfacesNode);
    unordered_map<string, Material> parseMaterials(Json::Value materialNode);
    Point parsePoint(const Json::Value& node, const string& nodeName);
    Vector parseVector(const Json::Value& node, const string& nodeName);
    Color parseColor(const Json::Value& node, const string& nodeName);
    double parseDouble(const Json::Value& node, const string& nodeName);
    string parseString(const Json::Value& node, const string& nodeName);
};
#endif
