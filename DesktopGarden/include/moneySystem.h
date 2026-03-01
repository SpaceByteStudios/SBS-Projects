#pragma once

struct MoneySystem {
  MoneySystem();

  void addMoney(int moneyAmount);
  void removeMoney(int moneyAmount);

  bool hasEnoughMoney(int neededMoney);

  int money = 25;
};
