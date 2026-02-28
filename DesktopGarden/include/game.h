#pragma once

#include "garden.h"
#include "plantsData.h"
#include "ui.h"

class Game {
 public:
  void init();

  void run();

 private:
  Garden garden;
  PlantsData plantsData;
  UI ui;
};
