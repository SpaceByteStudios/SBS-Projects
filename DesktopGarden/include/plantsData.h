#pragma once

#include <unordered_map>

#include "plantType.h"

class PlantsData {
 public:
  void addType(const PlantType& type) {
    types[type.id] = type;
  }

  const PlantType* get(const int id) const {
    return &types.at(id);
  }

 private:
  std::unordered_map<int, PlantType> types;
};
