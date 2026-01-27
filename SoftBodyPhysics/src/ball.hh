#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class Ball {
public:
  Ball();
  Ball(float start_radius);
  Ball(float start_radius, sf::Vector2f &start_pos);
  Ball(float start_radius, sf::Vector2f &start_pos, sf::Vector2f &start_vel);

  void update(float delta_time);
  void draw(sf::RenderWindow &window);

  void applyForce(sf::Vector2f &force);

  void setRadius(float radius);
  void setPosition(sf::Vector2f &new_pos);
  void setVelocity(sf::Vector2f &new_vel);
  void setColor(sf::Color &new_color);

  float getArea();

private:
  float radius;

  sf::Vector2f pos;
  sf::Vector2f vel;
  sf::Vector2f acc;

  sf::Color color;

  sf::CircleShape circle;
};
