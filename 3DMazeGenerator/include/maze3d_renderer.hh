#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector3.hpp>
#include <vector>

#include "maze3d.hh"

struct Camera {
  sf::Vector3f position;
  sf::Vector3f rotation;
};

struct Maze3DRenderer {
  Maze3DRenderer(sf::RenderWindow& window);

  void draw_grid(const Maze3D& maze);
  void draw_path(const Maze3D& maze);

  void draw_axis();
  void draw_plane(std::vector<sf::Vector3f> vertex_pos, const sf::Color& color);

  void set_color(const sf::Color& new_grid_color, const sf::Color& new_path_color);

  void move_camera(const sf::Vector3f& movement);
  void move_camera_rel(const sf::Vector3f& movement);
  void rotate_camera(const sf::Vector3f& rotation);

  sf::RenderWindow& window;

  sf::Color grid_color;
  sf::Color path_color;

  sf::Vector3f cube_rotation;
  Camera camera;
};
