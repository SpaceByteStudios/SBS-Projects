#pragma once

#include <vector>

struct InventorySystem {
  InventorySystem();

  void addStock(int id, int amount);
  void removeStock(int id, int amount);

  int getStock(int id);

  std::vector<int> plantsAmount;
};
