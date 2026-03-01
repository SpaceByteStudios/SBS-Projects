#include "plantsData.h"

#include <cassert>

void PlantsData::addType(const PlantType& type) {
  types[type.id] = type;
}

PlantType PlantsData::get(const int id) {
  assert(types.contains(id));

  return types.at(id);
}
