#ifndef UNIT_HPP
#define UNIT_HPP
#include <iostream>
#include <string>

class Unit {
protected:
  static int currentID;
  int ID;           // ID человека в самолете
  std::string type; // Тип человека на самолете

public:
  // Конструктор класса Unit
  Unit(const std::string &input_type) : ID(currentID++), type(input_type) {}

  // Default деструктор
  virtual ~Unit() = default;

  // Чистые вирутальные функции для абстрактности класса
  virtual int getHandLuggageWeight() const = 0;
  virtual int getLuggageWeight() const = 0;
  virtual bool canRegister() const = 0;

  // Вернуть ID человека на борту
  int getID() const { return ID; }

  // Вернуть тип человека на борту
  std::string getType() const { return type; }
};

#endif // UNIT_HPP