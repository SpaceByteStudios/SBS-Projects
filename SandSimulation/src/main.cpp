#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>

#include "simulation.hh"

int main() {
  Simulation simulation(100, 100);

  while (simulation.isRunning) {
    simulation.processInput();
    simulation.run();
    simulation.draw();
  }
}
