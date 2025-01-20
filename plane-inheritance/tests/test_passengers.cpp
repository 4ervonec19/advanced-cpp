#include "../units/passengers.hpp"
#include <cassert>
#include <iostream>

int Unit::currentID = 0;

void testEconomyPassenger() {
  EconomyPassenger econPassenger({5}, {20});
  assert(econPassenger.canRegister()); // Должен пройти регистрацию

  EconomyPassenger econPassengerFail1({15}, {20});
  assert(!econPassengerFail1.canRegister()); // Слишком тяжелая ручная кладь

  EconomyPassenger econPassengerFail2({5}, {30});
  assert(!econPassengerFail2.canRegister()); // Слишком тяжелый багаж

  EconomyPassenger econPassengerFail3({5, 5}, {20});
  assert(!econPassengerFail3.canRegister()); // Слишком много ручной клади

  EconomyPassenger econPassengerFail4({5}, {20, 20});
  assert(!econPassengerFail4.canRegister()); // Слишком много багажа

  assert(econPassenger.getID() == 0);
  assert(econPassengerFail4.getID() == 4);

  std::cout << "EconomyPassenger tests passed.\n";
}

void testBusinessPassenger() {
  BusinessPassenger busPassenger({10, 5}, {20, 15});
  assert(busPassenger.canRegister()); // Должен пройти регистрацию

  BusinessPassenger busPassengerFail1({25}, {20});
  assert(!busPassengerFail1.canRegister()); // Слишком тяжелая ручная кладь

  BusinessPassenger busPassengerFail2({10}, {45});
  assert(!busPassengerFail2.canRegister()); // Слишком тяжелый багаж

  BusinessPassenger busPassengerFail3({10, 10, 3}, {20});
  assert(!busPassengerFail3.canRegister()); // Слишком много ручной клади

  BusinessPassenger busPassengerFail4({10}, {20, 25});
  assert(!busPassengerFail4.canRegister()); // Слишком тяжелый багаж

  BusinessPassenger busPassengerFail5({10}, {50});
  assert(!busPassengerFail5.canRegister()); // Слишком тяжелый багаж

  BusinessPassenger busPassengerFail6({10, 10, 3}, {20, 5, 10});
  assert(!busPassengerFail6.canRegister()); // Слишком много всего

  BusinessPassenger busPassengerFail7({10}, {20, 5, 10});
  assert(!busPassengerFail7.canRegister()); // Слишком много багажа

  assert(busPassenger.getID() == 5);
  assert(busPassengerFail7.getID() == 12);

  std::cout << "BusinessPassenger tests passed.\n";
}

void testFirstClassPassenger() {
  FirstClassPassenger firstPassenger({30}, {20, 100});
  assert(firstPassenger.canRegister()); // Должен пройти регистрацию

  FirstClassPassenger firstPassengerFail1({70}, {20, 100});
  assert(!firstPassengerFail1.canRegister()); // Слишком тяжелая ручная кладь

  FirstClassPassenger firstPassengerFail2({30}, {100});
  assert(firstPassengerFail2.canRegister()); // Нет ограничений по весу багажа

  FirstClassPassenger firstPassengerFail3({30, 30, 30}, {20});
  assert(!firstPassengerFail3.canRegister()); // Слишком много ручной клади

  assert(firstPassenger.getID() == 13);
  assert(firstPassengerFail3.getID() == 16);

  std::cout << "FirstClassPassenger tests passed.\n";
}

int main() {
  // Запуск всех тестов
  testEconomyPassenger();
  testBusinessPassenger();
  testFirstClassPassenger();

  std::cout << "All tests passed successfully.\n";
  return 0;
}
