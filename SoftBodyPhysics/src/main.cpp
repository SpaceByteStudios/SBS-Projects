#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "ball.hh"

int main() {
  sf::RenderWindow window(sf::VideoMode({500, 500}), "Simulation");

  window.setPosition({1920 - (int)window.getSize().x - 50, 50});
  window.setFramerateLimit(60);

  Ball ball1(10.0);

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto *keyPressed =
                     event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
          window.close();
        }
      }
    }

    ball1.update(1.0 / 60.0);

    window.clear();
    ball1.draw(window);
  }
}
