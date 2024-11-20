#ifndef CG_MODEL_HPP
#define CG_MODEL_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <array>
#include <vector>

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
};

struct Model {
  using Face = std::array<Vertex, 3>;

  std::vector<Face> faces{};
  glm::vec3 position{ 0.0F };
  glm::vec3 rotation{ 0.0F };
  glm::vec3 scale{ 1.0F };
};

#endif