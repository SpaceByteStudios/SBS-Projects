#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <vector>

#include "maze3d.hh"
#include "maze3d_renderer.hh"

const sf::Vector3i maze_size{2, 2, 2};
const sf::Vector3f cell_size{1.0f, 1.0f, 1.0f};

std::vector<sf::Vector3f> plane_pos = {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
                                       {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}};

std::vector<sf::Vector3f> front_plane_pos = {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f},
                                             {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}};

sf::Clock deltaClock;

sf::Vector3f Right{1.0f, 0.0f, 0.0f};
sf::Vector3f Up{0.0f, 1.0f, 0.0f};
sf::Vector3f Front{0.0f, 0.0f, 1.0f};

const float movement_speed = 5.0f;
const float rotation_speed = M_PI / 4.0f;

int main() {
  srand(time(nullptr));

  sf::RenderWindow window(sf::VideoMode({1000, 1000}), "Maze Generator");
  window.setPosition({1920 - (int)window.getSize().x - 25, 25});

  window.setFramerateLimit(60);

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

  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int z = 0; z < maze.grid_size.z; z++) {
      for (int x = 0; x < maze.grid_size.x; x++) {
        int index = maze.index_at_pos({x, y, z});
        maze.grid[index].walls_bitmap = 63;
      }
    }
  }

  renderer.camera.position = {1.0f, 1.0f, 5.0f};
  renderer.camera.rotation = {0.0f, 0.0f, 0.0f};

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
        }
      }
    }

    float delta_time = deltaClock.restart().asSeconds();

    renderer.cube_rotation.y += (M_PI / 16.0f) * delta_time;
    renderer.cube_rotation.x += (M_PI / 12.0f) * delta_time;

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

    renderer.draw_axis();

    // renderer.draw_plane(plane_pos, sf::Color{255, 0, 255});
    // renderer.draw_plane(front_plane_pos, sf::Color{0, 255, 0});
    renderer.draw_grid(maze);

    window.display();
  }
}
