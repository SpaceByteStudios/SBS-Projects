#pragma once

#include <unordered_map>

#include "plantType.h"

class PlantsData {
 public:
  void addType(const PlantType& type);

  PlantType get(const int id);

 private:
  std::unordered_map<int, PlantType> types;
};
