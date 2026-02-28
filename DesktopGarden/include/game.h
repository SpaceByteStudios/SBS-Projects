#pragma once

#include "garden.h"
#include "plantsData.h"

class Game {
 public:
  void init();

  void run();

 private:
  Garden garden;
  PlantsData plantsData;
};
