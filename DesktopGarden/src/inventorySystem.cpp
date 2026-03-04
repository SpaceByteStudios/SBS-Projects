#include "inventorySystem.h"

InventorySystem::InventorySystem() {
  plantsAmount.resize(4, 0);
}

void InventorySystem::addStock(int id, int amount) {
  plantsAmount[id] += amount;
}

void InventorySystem::removeStock(int id, int amount) {
  plantsAmount[id] -= amount;
}

int InventorySystem::getStock(int id) {
  return plantsAmount.at(id);
}
