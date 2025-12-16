#include <x86gprintrin.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>

#include "simulation.hh"

int main() {
  Simulation simulation(160, 90);

  while (simulation.isRunning) {
    simulation.processInput();
    simulation.run();
    simulation.draw();
  }
}
