#include "importer.hpp"

#include <glm/glm.hpp>

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <sstream>
#include <utility>
#include <vector>

#include "timer.hpp"

// return the directory with a trailing '/'
auto get_directory(const std::string& file_path) -> std::string
{
  auto index{ file_path.find_last_of('/') };
  return (index == std::string::npos) ? "./" : file_path.substr(0, index + 1);
}

// Wavefront obj importer
//   accepts triangulated faces
auto import_model(const std::string& obj_path) -> std::optional<Model>
{
  auto obj = std::ifstream{ obj_path };
  if (!obj) {
    std::cerr << "Could not open the file: " << obj_path << '\n';
    return {};
  }
  auto positions = std::vector<glm::vec3>{};
  auto normals = std::vector<glm::vec3>{};
  auto output = Model{};
  while (obj) {
    auto line = std::string{};
    std::getline(obj, line);
    if (line.starts_with('#') || line.length() == 0) continue;
    auto line_stream = std::stringstream{ line };
    auto head = std::string{};
    std::getline(line_stream, head, ' ');
    if (head == "v") {
      auto position = glm::vec3{};
      line_stream >> position.x >> position.y >> position.z;
      if (!line_stream) {
        std::cerr << "Could not parse the geometric vertex on line: " << line << '\n';
        return {};
      }
      positions.push_back(position);
    }
    else if (head == "vn") {
      auto normal = glm::vec3{};
      line_stream >> normal.x >> normal.y >> normal.z;
      if (!line_stream) {
        std::cerr << "Could not parse the vertex normal on line: " << line << '\n';
        return {};
      }
      normals.push_back(normal);
    }
    else if (head == "f") {
      auto vertices = std::vector<Vertex>{};
      while (line_stream) {
        auto position_index = std::size_t{};
        line_stream >> position_index;

        line_stream.get(); // skip the '/'
        line_stream.ignore(std::numeric_limits<std::streamsize>::max(), '/'); // skip the texture index

        auto normal_index = std::size_t{};
        line_stream >> normal_index;

        if (!line_stream) {
          std::cerr << "Could not parse indices on line: " << line << '\n';
          return {};
        }

        if (position_index > positions.size() || normal_index > normals.size()) {
          std::cerr << "Invalid indices on line: " << line << '\n';
          return {};
        }

        line_stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');

        vertices.push_back(Vertex{
          positions.at(position_index - 1),
          normals.at(normal_index - 1)
        });
      }

      // triangulate the face
      for (auto i = 1u; i < vertices.size() - 1; ++i) {
        output.faces.push_back({ vertices.at(0), vertices.at(i), vertices.at(i + 1) });
      }
    }
  }
  return output;
}