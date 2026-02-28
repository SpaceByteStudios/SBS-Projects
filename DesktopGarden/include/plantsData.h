#pragma once

#include <unordered_map>

#include "plantType.h"

class PlantsData {
 public:
  void addType(const PlantType& type);

  const PlantType* get(const int id) const;

 private:
  std::unordered_map<int, PlantType> types;
};
