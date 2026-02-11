#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector3.hpp>

#include "maze3d.hh"

struct Camera {
  sf::Vector3f position;
  sf::Vector3f rotation;
};

struct Maze3DRenderer {
  Maze3DRenderer(sf::RenderWindow& window);

  void rotate_x(sf::Vector3f& v, float angle);
  void rotate_y(sf::Vector3f& v, float angle);
  void rotate_z(sf::Vector3f& v, float angle);
  void translate(sf::Vector3f& v, const sf::Vector3f& t);
  void apply_camera(sf::Vector3f& v, Camera& camera);

  void ortho_projection(sf::Vector3f& v, const sf::Vector2u& window_size, float zoom = 100.0f);

  void pers_projection(sf::Vector3f& v, const sf::Vector2u& window_size, float fov_deg = 90.0f);
  bool clipLineToNearPlane(sf::Vector3f& v1, sf::Vector3f& v2, float near);

  void draw_grid(const Maze3D& maze);
  void draw_path(const Maze3D& maze);
  void draw_graph(const Maze3D& maze);

  void draw_axis();

  void set_color(const sf::Color& new_grid_color, const sf::Color& new_path_color);

  void move_camera(const sf::Vector3f& movement);
  void move_camera_rel(const sf::Vector3f& movement);
  void rotate_camera(const sf::Vector3f& rotation);

  sf::RenderWindow& window;

  sf::Color grid_color;
  sf::Color path_color;

  sf::Vector3f cube_rotation;
  Camera camera;
  bool project_perspective;
  float ortho_zoom = 100.0f;
};
