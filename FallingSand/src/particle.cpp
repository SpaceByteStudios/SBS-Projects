#include "particle.hh"

Particle::Particle(){
  color = sf::Color(255, 255, 255);
}

Particle::Particle(sf::Vector2i& position){
  pos = position;
}

