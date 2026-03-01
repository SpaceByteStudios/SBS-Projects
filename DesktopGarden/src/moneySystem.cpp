#include "moneySystem.h"

MoneySystem::MoneySystem() {
}

void MoneySystem::addMoney(int moneyAmount) {
  money += moneyAmount;
}

void MoneySystem::removeMoney(int moneyAmount) {
  money -= moneyAmount;
}

bool MoneySystem::hasEnoughMoney(int neededMoney) {
  return neededMoney < money;
}
