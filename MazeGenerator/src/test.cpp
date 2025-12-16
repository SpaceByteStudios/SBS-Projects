#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/WindowEnums.hpp>

int main() {
  sf::RenderWindow window(sf::VideoMode({500, 500}), "Test");
  window.setFramerateLimit(60);

  while (window.isOpen()) {
    while (auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    window.clear(sf::Color::Black);

    sf::CircleShape circle(20.0);
    circle.setPosition({50.0, 50.0});
    circle.setFillColor(sf::Color(255, 255, 255));

    window.draw(circle);

    window.display();
  }
}
