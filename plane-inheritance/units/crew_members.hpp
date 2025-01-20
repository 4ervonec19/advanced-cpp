#ifndef CREW_MEMBERS_HPP
#define CREW_MEMBERS_HPP

#include "unit.hpp"

// Класс Pilot
class Pilot : public Unit {
public:
  Pilot();                                   // Конструктор
  int getHandLuggageWeight() const override; // Вес ручной клади
  int getLuggageWeight() const override;     // Вес багажа
  bool canRegister() const override;         // Проверка возможности регистрации
};

// Класс FlightAttendant
class FlightAttendant : public Unit {
public:
  FlightAttendant();                         // Конструктор
  int getHandLuggageWeight() const override; // Вес ручной клади
  int getLuggageWeight() const override;     // Вес багажа
  bool canRegister() const override;         // Проверка возможности регистрации
};

#endif // CREW_MEMBERS_HPP
