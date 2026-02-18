#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <numbers>
#include <vector>

#include "maze3d.hh"
#include "maze3d_generator.hh"
#include "maze3d_renderer.hh"
#include "maze3d_solver.hh"

const sf::Vector3i maze_size{4, 4, 4};
const sf::Vector3f cell_size{4.0f, 4.0f, 4.0f};

std::vector<sf::Vector3f> plane_pos = {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
                                       {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}};

std::vector<sf::Vector3f> front_plane_pos = {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f},
                                             {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}};

std::vector<sf::Vector3f> corner_points = {{-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f},
                                           {-1.0f, 1.0f, 1.0f},   {1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, -1.0f},
                                           {1.0f, -1.0f, 1.0f},   {1.0f, 1.0f, 1.0f}};

sf::Clock deltaClock;
float total_time = 0.0f;

sf::Vector3f Right{1.0f, 0.0f, 0.0f};
sf::Vector3f Up{0.0f, 1.0f, 0.0f};
sf::Vector3f Front{0.0f, 0.0f, 1.0f};

const float movement_speed = 6.0f;
const float rotation_speed = std::numbers::pi_v<float> / 3.0f;

int main() {
  srand(time(nullptr));

  sf::RenderWindow window(sf::VideoMode({1000, 1000}), "Maze Generator");
  window.setPosition({1920 - (int)window.getSize().x - 25, 25});

  window.setFramerateLimit(60);

  sf::Font font;
  if (!font.openFromFile("0xProtoNerdFont.ttf")) {
    std::cout << "Failed to load font\n";
    return -1;
  }

  sf::Text fpsText(font, "FPS:");
  fpsText.setCharacterSize(10);
  fpsText.setFillColor(sf::Color::White);
  fpsText.setPosition({3, 3});

  Maze3D maze(maze_size);
  Maze3DRenderer renderer(window);

  sf::Color grid_color = sf::Color(255, 255, 255);
  sf::Color path_color = sf::Color(64, 255, 64);

  renderer.set_color(grid_color, path_color);

  Right *= movement_speed;
  Up *= movement_speed;
  Front *= movement_speed;

  maze.start_cell = sf::Vector3i(0, 0, 0);
  maze.end_cell = sf::Vector3i(maze.grid_size.x - 1, maze.grid_size.y - 1, maze.grid_size.z - 1);
  maze.cell_size = cell_size;

  renderer.camera.position = {0.0f, 0.0f, 0.0f};
  renderer.camera.rotation = {-std::numbers::pi_v<float> / 8.0f, 0.0f, 0.0f};
  renderer.ortho_zoom = 40.0f;
  renderer.project_perspective = true;

  bool render_path = false;
  bool render_graph = false;

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
          window.close();
        } else if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
          renderer.camera.position = {0.0f, 0.0f, 0.0f};
          renderer.camera.rotation = {0.0f, 0.0f, 0.0f};
          renderer.cube_rotation = {0.0f, 0.0f, 0.0f};
        } else if (keyPressed->scancode == sf::Keyboard::Scancode::P) {
          renderer.project_perspective = !renderer.project_perspective;
        } else if (keyPressed->scancode == sf::Keyboard::Scancode::G) {
          render_path = false;
          generate_depth_first_maze(maze);
          solve_depth_first_maze(maze);
          render_path = true;
        } else if (keyPressed->scancode == sf::Keyboard::Scancode::T) {
          render_graph = !render_graph;
        }
      }
    }

    float delta_time = deltaClock.restart().asSeconds();
    total_time += delta_time;

    // renderer.cube_rotation.y += (std::numbers::pi_v<float> / 16.0f) * delta_time;
    // renderer.cube_rotation.x += (std::numbers::pi_v<float> / 12.0f) * delta_time;

    // Camera Movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
      renderer.move_camera_rel(-Front * delta_time);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
      renderer.move_camera_rel(Front * delta_time);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
      renderer.move_camera_rel(-Right * delta_time);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
      renderer.move_camera_rel(Right * delta_time);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
      renderer.move_camera_rel(Up * delta_time);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
      renderer.move_camera_rel(-Up * delta_time);
    }

    // Camera rotation
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
      renderer.rotate_camera(sf::Vector3f{rotation_speed, 0.0, 0.0} * delta_time);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
      renderer.rotate_camera(sf::Vector3f{-rotation_speed, 0.0, 0.0} * delta_time);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
      renderer.rotate_camera(sf::Vector3f{0.0, rotation_speed, 0.0} * delta_time);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
      renderer.rotate_camera(sf::Vector3f{0.0, -rotation_speed, 0.0} * delta_time);
    }

    window.clear();

    float fps = 1.0f / delta_time;

    std::stringstream ss;
    ss << "FPS:" << static_cast<int>(fps);
    fpsText.setString(ss.str());
    window.draw(fpsText);

    // renderer.draw_axis();

    if (render_graph) {
      renderer.draw_graph(maze);
    } else {
      renderer.draw_grid(maze);

      if (render_path) {
        renderer.draw_path(maze);
      }
    }

    for (sf::Vector3f p : corner_points) {
      // renderer.scale(p, renderer.cube_scale);
      // renderer.rotate_x(p, renderer.cube_rotation.x);
      // renderer.rotate_y(p, renderer.cube_rotation.y);
      // renderer.rotate_z(p, renderer.cube_rotation.z);
      // renderer.translate(p, renderer.cube_position);

      // renderer.draw_point(p, sf::Color(255, 32, 32));
    }

    window.display();
  }
}
