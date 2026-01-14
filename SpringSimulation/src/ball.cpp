#include "ball.hh"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <numbers>

Ball::Ball() {
  radius = 10;
  color = sf::Color(255, 255, 255);

  circle = sf::CircleShape(radius);
  circle.setOrigin({radius / 2, radius / 2});
  circle.setPosition(pos);
  circle.setFillColor(color);
}

Ball::Ball(float start_radius) {
  radius = start_radius;

  circle = sf::CircleShape(radius);
  circle.setOrigin({radius / 2, radius / 2});
  circle.setPosition(pos);
  circle.setFillColor(color);
}

Ball::Ball(float start_radius, sf::Vector2f &start_pos) {
  radius = start_radius;
  pos = start_pos;

  color = sf::Color(255, 255, 255);

  circle = sf::CircleShape(radius);
  circle.setOrigin({radius / 2, radius / 2});
  circle.setPosition(pos);
  circle.setFillColor(color);
}

Ball::Ball(float start_radius, sf::Vector2f &start_pos,
           sf::Vector2f &start_vel) {
  radius = start_radius;

  pos = start_pos;
  vel = start_vel;

  color = sf::Color(255, 255, 255);

  circle = sf::CircleShape(radius);
  circle.setOrigin({radius / 2, radius / 2});
  circle.setPosition(pos);
  circle.setFillColor(color);
}

void Ball::update(float delta_time) {
  vel += acc * delta_time;
  pos += vel * delta_time;

  acc = {0, 0};

  circle.setPosition(pos);
}

void Ball::draw(sf::RenderWindow &window) { window.draw(circle); }

void Ball::applyForce(sf::Vector2f &force) { acc += force / getArea(); }

void Ball::setRadius(float new_radius) { radius = new_radius; }
void Ball::setPosition(sf::Vector2f &new_pos) { pos = new_pos; }
void Ball::setVelocity(sf::Vector2f &new_vel) { vel = new_vel; }
void Ball::setColor(sf::Color &new_color) { color = new_color; }

float Ball::getArea() { return std::numbers::pi_v<float> * radius * radius; }
