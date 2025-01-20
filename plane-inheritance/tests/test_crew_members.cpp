#include "../units/crew_members.hpp"
#include <cassert>
#include <iostream>

// int Unit::currentID = 0;

// Тест для класса Pilot
void testPilot() {
  Pilot pilot;

  // Тест конструктора
  assert(pilot.getType() == "PILOT");

  // Тест методов получения веса ручной клади и багажа
  assert(pilot.getHandLuggageWeight() == 0);
  assert(pilot.getLuggageWeight() == 0);

  // Тест на возможность регистрации
  assert(pilot.canRegister() == true);
}

// Тест для класса FlightAttendant
void testFlightAttendant() {
  FlightAttendant flightAttendant;

  // Тест конструктора
  assert(flightAttendant.getType() == "FLIGHT_ATTENDANT");

  // Тест методов получения веса ручной клади и багажа
  assert(flightAttendant.getHandLuggageWeight() == 0);
  assert(flightAttendant.getLuggageWeight() == 0);

  // Тест на возможность регистрации
  assert(flightAttendant.canRegister() == true);
}

int main() {
  // Запуск всех тестов
  testPilot();
  testFlightAttendant();

  std::cout << "All tests passed successfully." << std::endl;
  return 0;
}
