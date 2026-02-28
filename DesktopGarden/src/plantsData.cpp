#include "plantsData.h"

void PlantsData::addType(const PlantType& type) {
  types[type.id] = type;
}

const PlantType* PlantsData::get(const int id) const {
  return &types.at(id);
}
