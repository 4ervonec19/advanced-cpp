#include "../units/segments.hpp"
#include <cassert>
#include <iostream>
#include <vector>

void testMaxWeightAndType() {
  Segment segment("ECONOMY", 100);
  assert(segment.getMaxWeight() == 100);
  assert(segment.getType() == "ECONOMY");
  std::cout << "testMaxWeightAndType passed.\n";
}

void testCurrentWeight() {
  Segment segment("ECONOMY", 100);
  assert(segment.getCurrentWeight() == 0);
  std::cout << "testCurrentWeight passed.\n";
}

void testCanAddLuggage() {
  Segment segment("ECONOMY", 100);
  assert(segment.canAddLuggage(80));   // Можно добавить 80
  assert(!segment.canAddLuggage(120)); // Нельзя добавить 120, превышает лимит
  std::cout << "testCanAddLuggage passed.\n";
}

void testAddLuggage() {
  Segment segment("ECONOMY", 100);
  segment.updCurrentWeight(80);
  assert(segment.getCurrentWeight() == 80);
  std::cout << "testAddLuggage passed.\n";
}

void testLuggageWeights() {
  Segment segment("ECONOMY", 100);
  segment.addLuggage(15, 0);
  auto luggage_id = segment.getLuggageWeights();

  assert(luggage_id.size() == 1);     // Должен быть один элемент
  assert(luggage_id[0].first == 0);   // ID пассажира
  assert(luggage_id[0].second == 15); // Вес багажа

  std::cout << "testLuggageWeights passed.\n";
}

int main() {
  // Запуск всех тестов
  testMaxWeightAndType();
  testCurrentWeight();
  testCanAddLuggage();
  testAddLuggage();
  testLuggageWeights();

  std::cout << "All Segment tests passed successfully.\n";
  return 0;
}
