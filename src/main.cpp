#include "timer.hpp"
#include "model.hpp"
#include "importer.hpp"
#include "camera.hpp"
#include "shader.hpp"

#include "glad/glad.h"
#include <SFML/Window.hpp>

#include <iostream>

auto setup_model(Model& model) -> std::pair<unsigned int, unsigned int> {
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, model.faces.size() * sizeof(Model::Face), model.faces.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

  return { vao, vbo };
}

auto draw_model(const Model& model, unsigned int vao) -> void {
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, model.faces.size() * 3);
}

auto get_model_matrix(const Model& model) -> glm::mat4 {
  auto model_matrix = glm::mat4{ 1.0F };
  model_matrix = glm::translate(model_matrix, model.position);
  model_matrix = glm::rotate(model_matrix, model.rotation.x, { 1.0F, 0.0F, 0.0F });
  model_matrix = glm::rotate(model_matrix, model.rotation.y, { 0.0F, 1.0F, 0.0F });
  model_matrix = glm::rotate(model_matrix, model.rotation.z, { 0.0F, 0.0F, 1.0F });
  model_matrix = glm::scale(model_matrix, model.scale);
  return model_matrix;
}

auto main() -> int {
  constexpr auto screen_width = 1280;
  constexpr auto screen_height = 720;
  auto window = sf::Window{ 
    sf::VideoMode{ { screen_width, screen_height} }, 
    "Trabalho 2 CG",
    sf::Style::Default,
    sf::ContextSettings{ 24, 8, 0, 4, 5 }};

  window.setVerticalSyncEnabled(true);

  if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  auto camera = Camera{ glm::vec3{ 0.0F, 0.0F, 25.0F } };
  auto shader = Shader{ "../shaders/vertex.glsl", "../shaders/fragment.glsl" };

  auto falcon = import_model("../models/falcon.obj");
  if (!falcon) {
    std::cerr << "Failed to import model\n";
    return 1;
  }
  falcon->scale = glm::vec3{ 0.7F };
  falcon->position = glm::vec3{ 18.0F, 0.0F, -2.0F };
  falcon->rotation.y = glm::radians(-90.0F);
  auto [vao, vbo] = setup_model(*falcon);

  auto ball = import_model("../models/ball.obj");
  if (!ball) {
    std::cerr << "Failed to import model\n";
    return 1;
  }
  ball->scale = glm::vec3{ 0.3F };
  ball->position = glm::vec3{ -15.0F, 1.0F, 0.0F };
  auto [vao2, vbo2] = setup_model(*ball);
  auto ball_to_second = true;
  auto ball_moving = true;

  auto hangar = import_model("../models/hangar.obj");
  if (!hangar) {
    std::cerr << "Failed to import model\n";
    return 1;
  }
  hangar->position = glm::vec3{ -20.0f, 0.0f, 0.0F };
  hangar->scale = glm::vec3{ 0.05F };
  auto [vao3, vbo3] = setup_model(*hangar);

  auto hangar2 = *hangar;
  hangar2.position = glm::vec3{ 20.0f, 0.0f, 0.0F };
  hangar2.rotation.y = glm::radians(180.0F);

  while (window.isOpen()) {
    auto event = sf::Event{};
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::MouseWheelScrolled) {
        camera.ProcessMouseScroll(event.mouseWheelScroll.delta);
      }
      }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      camera.ProcessMouseMovement(0.0F, 1.0F);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      camera.ProcessMouseMovement(0.0F, -1.0F);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      camera.ProcessMouseMovement(-1.0F, 0.0F);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      camera.ProcessMouseMovement(1.0F, 0.0F);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      camera.ProcessKeyboard(FORWARD, 0.1F);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      camera.ProcessKeyboard(BACKWARD, 0.1F);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      camera.ProcessKeyboard(LEFT, 0.1F);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      camera.ProcessKeyboard(RIGHT, 0.1F);
        }

    // move ball from one hangar to the other, and so on
    if (ball_to_second) {
      if (ball_moving) {
        ball->position.x += 0.1F;
      }
      else {
        ball->rotation.y += 0.01F;
        if (ball->rotation.y >= glm::radians(360.0F)) {
          ball->rotation.y = 0.0F;
          ball_moving = true;
        }
      }
      if (ball->position.x >= 20.0F) {
        ball_to_second = false;
        ball_moving = false;
      }
    } else {
      if (ball_moving) {
        ball->position.x -= 0.1F;
      }
      else {
        ball->rotation.y += 0.01F;
        if (ball->rotation.y >= glm::radians(180.0F)) {
          ball_moving = true;
        }
      }
      if (ball->position.x <= -20.0F) {
        ball_to_second = true;
        ball_moving = false;
      }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    shader.setMat4("view", camera.GetViewMatrix());
    shader.setMat4("projection", glm::perspective(glm::radians(camera.Zoom), static_cast<float>(screen_width) / screen_height, 0.1f, 100.0f));
    shader.setVec3("camera_position", camera.Position);

    shader.setMat4("model", get_model_matrix(*falcon));
    draw_model(*falcon, vao);

    shader.setMat4("model", get_model_matrix(*ball));
    draw_model(*ball, vao2);

    shader.setMat4("model", get_model_matrix(*hangar));
    draw_model(*hangar, vao3);

    shader.setMat4("model", get_model_matrix(hangar2));
    draw_model(hangar2, vao3);

    window.display();
  }

  return 0;
}