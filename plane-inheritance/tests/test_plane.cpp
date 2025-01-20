#include "../units/plane.hpp"
#include <cassert>
#include <iostream>
#include <memory>

// int Unit::currentID = 0;

void testPlane() {
  // Инициализация самолёта с лимитами
  Plane plane(100, 60,
              10); // ECONOMY: 100 кг, BUSINESS: 60 кг, FIRST_CLASS: 10 кг

  // Tест 0: Проверка максимального веса в сегментах
  assert(plane.getMaxWeightEconomyInfo() == 100);
  assert(plane.getMaxWeightBusinessInfo() == 60);
  assert(plane.getMaxWeightFirstClassInfo() == 10);

  // Тест 1: Проверка добавления пассажира эконом-класса
  std::unique_ptr<EconomyPassenger> econPassenger =
      std::make_unique<EconomyPassenger>(std::vector<int>{5},
                                         std::vector<int>{20});
  plane.registerUnit(std::move(econPassenger));
  assert(plane.getWeightEconomyInfo() == 25);
  assert(plane.getWeightBusinessInfo() == 0);
  assert(plane.getWeightFirstClassInfo() == 0);
  std::cout << "Test 1 passed.\n";

  // Тест 2: Ошибка при добавлении ещё одного пассажира с превышением лимита
  auto econPassengerFail = std::make_unique<EconomyPassenger>(
      std::vector<int>{5}, std::vector<int>{25});
  plane.registerUnit(std::move(econPassengerFail));
  assert(plane.getWeightEconomyInfo() == 25);
  std::cout << "Test 2 passed.\n";

  // Тест 3: Добавление ещё одного пассажира эконом-класса
  auto econPassengerNew = std::make_unique<EconomyPassenger>(
      std::vector<int>{6}, std::vector<int>{17});
  plane.registerUnit(std::move(econPassengerNew));
  assert(plane.getWeightEconomyInfo() == 48);
  std::cout << "Test 3 passed.\n";

  // Тест 4: Добавление пассажира бизнес-класса
  auto busPassenger = std::make_unique<BusinessPassenger>(
      std::vector<int>{10, 5}, std::vector<int>{20, 19});
  plane.registerUnit(std::move(busPassenger));
  assert(plane.getWeightEconomyInfo() == 48);
  assert(plane.getWeightBusinessInfo() == 54);
  assert(plane.getWeightInfo() == 102);
  std::cout << "Test 4 passed.\n";

  // Тест 5: Ещё один бизнес-пассажир
  auto busPassengerNew = std::make_unique<BusinessPassenger>(
      std::vector<int>{5}, std::vector<int>{1, 15});
  plane.registerUnit(std::move(busPassengerNew));
  assert(plane.getWeightEconomyInfo() == 63);
  assert(plane.getWeightBusinessInfo() == 60);
  assert(plane.getWeightInfo() == 123);
  std::cout << "Test 5 passed.\n";

  // Тест 6: Ещё один пассажир эконом-класса
  auto econPassenger_ = std::make_unique<EconomyPassenger>(
      std::vector<int>{10}, std::vector<int>{24});
  plane.registerUnit(std::move(econPassenger_));
  assert(plane.getWeightEconomyInfo() == 97);
  assert(plane.getWeightInfo() == 157);
  std::cout << "Test 6 passed.\n";

  // Тест 7: Бизнес-пассажир с превышением лимита
  auto busPassenger_ = std::make_unique<BusinessPassenger>(
      std::vector<int>{0}, std::vector<int>{20, 19});
  plane.registerUnit(std::move(busPassenger_));
  assert(plane.getWeightEconomyInfo() == 92); // 97 - 20 - 24 + 39
  assert(plane.getWeightInfo() == 152);
  std::cout << "Test 7 passed.\n";

  // Тест 8: Ошибка при добавлении бизнес-пассажира с превышением лимита
  auto busPassengerFail = std::make_unique<BusinessPassenger>(
      std::vector<int>{10, 10, 3}, std::vector<int>{20, 5, 10});
  plane.registerUnit(std::move(busPassengerFail)); // Не должен добавиться
  assert(plane.getWeightEconomyInfo() == 92);
  assert(plane.getWeightBusinessInfo() == 60);
  std::cout << "Test 8 passed.\n";

  // Тест 9: Ошибка при добавлении пассажира первого класса
  auto firstPassengerFail = std::make_unique<FirstClassPassenger>(
      std::vector<int>{70}, std::vector<int>{20, 100});
  plane.registerUnit(std::move(firstPassengerFail));
  assert(plane.getWeightFirstClassInfo() == 0);
  std::cout << "Test 9 passed.\n";

  // Тест 10: Успешное добавление пассажира первого класса
  auto firstPassengerNew_ = std::make_unique<FirstClassPassenger>(
      std::vector<int>{2, 2}, std::vector<int>{2, 2});
  plane.registerUnit(std::move(firstPassengerNew_));
  assert(plane.getWeightFirstClassInfo() == 8);
  std::cout << "Test 10 passed.\n";

  // Тест 11: Добавление пассажира первого класса с нулевыми весами
  auto firstPassengerNewFin = std::make_unique<FirstClassPassenger>(
      std::vector<int>{0, 60}, std::vector<int>{0, 0});
  plane.registerUnit(std::move(firstPassengerNewFin));
  assert(plane.getWeightFirstClassInfo() == 8);
  assert(plane.getWeightEconomyInfo() == 92);
  std::cout << "Test 11 passed.\n";

  auto firstPassengerNewDel = std::make_unique<FirstClassPassenger>(
      std::vector<int>{0, 0}, std::vector<int>{60, 0});

  // Тест 12: Даже удаляя, не разместим первого класса
  plane.registerUnit(std::move(firstPassengerNewDel));
  std::cout << "Test 12 passed.\n";

  // Тест 13: Не можем добавить в эконом
  auto econPassengerFailNew = std::make_unique<EconomyPassenger>(
      std::vector<int>{5}, std::vector<int>{5});
  plane.registerUnit(std::move(econPassengerFailNew));
  assert(plane.getWeightEconomyInfo() == 92);
  std::cout << "Test 13 passed.\n";

  // Тест 14: Первый багаж добавится в первый класс, второй нет (для первого
  // класса)
  auto firstPassengerAdd = std::make_unique<FirstClassPassenger>(
      std::vector<int>{0, 0}, std::vector<int>{1, 3});
  plane.registerUnit(std::move(firstPassengerAdd));
  assert(plane.getWeightEconomyInfo() == 95);
  assert(plane.getWeightFirstClassInfo() == 9);
  std::cout << "Test 14 passed.\n";

  // Тест 15: Второй багаж добавится в первый класс, первый нет (для первого
  // класса)
  auto firstPassengerAdd_ = std::make_unique<FirstClassPassenger>(
      std::vector<int>{0, 0}, std::vector<int>{3, 1});
  plane.registerUnit(std::move(firstPassengerAdd_));
  assert(plane.getWeightEconomyInfo() == 98);
  assert(plane.getWeightFirstClassInfo() == 10);
  assert(plane.getWeightBusinessInfo() == 60);
  std::cout << "Test 15 passed.\n";

  {
    Plane plane(10, 5,
                10); // ECONOMY: 10 кг, BUSINESS: 5 кг, FIRST_CLASS: 10 кг
    // Тест 16: Оба по одиночке можем разместить (для первого класса)
    auto firstPassengerAdd__ = std::make_unique<FirstClassPassenger>(
        std::vector<int>{0, 0}, std::vector<int>{5, 6});
    plane.registerUnit(std::move(firstPassengerAdd__));
    assert(plane.getWeightEconomyInfo() == 5);
    assert(plane.getWeightFirstClassInfo() == 6);
    assert(plane.getWeightBusinessInfo() == 0);
    std::cout << "Test 16 passed.\n";

    // Дополнительно для покрытия passenger.hpp
    auto econPassenger = std::make_unique<EconomyPassenger>(
        std::vector<int>{5, 6}, std::vector<int>{20});
    plane.registerUnit(std::move(econPassenger));

    auto econPassenger_ = std::make_unique<EconomyPassenger>(
        std::vector<int>{5}, std::vector<int>{10, 10});
    plane.registerUnit(std::move(econPassenger_));

    auto econPassengerNew = std::make_unique<EconomyPassenger>(
        std::vector<int>{12}, std::vector<int>{10});
    plane.registerUnit(std::move(econPassengerNew));

    auto busPassenger_ = std::make_unique<BusinessPassenger>(
        std::vector<int>{15, 15}, std::vector<int>{20, 19});
    plane.registerUnit(std::move(busPassenger_));

    auto busPassenger__ = std::make_unique<BusinessPassenger>(
        std::vector<int>{10, 10}, std::vector<int>{20, 21});
    plane.registerUnit(std::move(busPassenger__));

    auto busPassenger1 = std::make_unique<BusinessPassenger>(
        std::vector<int>{10, 10}, std::vector<int>{19, 20, 1});
    plane.registerUnit(std::move(busPassenger1));
  }
}

int main() {
  testPlane();
  std::cout << "All tests passed successfully.\n";
  return 0;
}
