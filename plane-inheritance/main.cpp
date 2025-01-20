#include "units/plane.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

void handlePassengerInput() {

  std::string segment_info;
  int firstClassMax = 0, businessClassMax = 0, economyClassMax = 0;

  // Считываем первые три строки
  for (int count = 0; count < 3; ++count) {
    std::getline(std::cin, segment_info);
    std::istringstream iss(segment_info);
    std::string segment;
    int weight;

    // Ожидаем, что каждая строка будет в формате "SEGMENT_NAME WEIGHT"
    if (iss >> segment >> weight) {
      if (segment == "FIRST_CLASS_SEGMENT") {
        firstClassMax = weight;
      } else if (segment == "BUSINESS_CLASS_SEGMENT") {
        businessClassMax = weight;
      } else if (segment == "ECONOMY_CLASS_SEGMENT") {
        economyClassMax = weight;
      }
    }
  }

  // Инициализируем самолет
  Plane plane(economyClassMax, businessClassMax, firstClassMax);

  std::string inputLine;
  // std::cout << "Enter passenger type and weights or 'exit' to quit: ";

  while (std::getline(std::cin, inputLine) && inputLine != "exit") {
    std::istringstream ss(inputLine);
    std::string passengerType;
    std::vector<int> allWeights;

    // Считываем тип пассажира
    ss >> passengerType;

    // Считываем веса
    int weight;
    while (ss >> weight) {
      allWeights.push_back(weight);
    }

    std::vector<int> handLuggage;
    std::vector<int> luggage;

    if (passengerType == "ECONOMY") {
      // Для эконом-класса: первая позиция — ручная кладь, остальные (вторая) —
      // багаж
      if (!allWeights.empty()) {
        handLuggage.push_back(allWeights[0]);
      }
      if (allWeights.size() > 1) {
        luggage.insert(luggage.end(), allWeights.begin() + 1, allWeights.end());
      }
    } else if (passengerType == "BUSINESS" || passengerType == "FIRST_CLASS") {
      // Для бизнес и первого класса: первые две позиции — ручная кладь,
      // остальные — багаж
      if (allWeights.size() > 0) {
        handLuggage.push_back(allWeights[0]);
      }
      if (allWeights.size() > 1) {
        handLuggage.push_back(allWeights[1]);
      }
      if (allWeights.size() > 2) {
        luggage.insert(luggage.end(), allWeights.begin() + 2, allWeights.end());
      }
    } else {
      continue;
    }

    // Создаем объект пассажира в зависимости от типа
    std::unique_ptr<Unit> passenger;
    if (passengerType == "ECONOMY") {
      passenger = std::make_unique<EconomyPassenger>(handLuggage, luggage);
    } else if (passengerType == "BUSINESS") {
      passenger = std::make_unique<BusinessPassenger>(handLuggage, luggage);
    } else if (passengerType == "FIRST_CLASS") {
      passenger = std::make_unique<FirstClassPassenger>(handLuggage, luggage);
    }

    plane.registerUnit(std::move(passenger));
    // std::cout << "Current total weight in plane: " << plane.getWeightInfo()
    // << std::endl; std::cout << "Enter passenger type and weights or 'exit' to
    // quit: ";
  }
}

int main() {

  handlePassengerInput();
  // std::cout << "All inputs processed!" << std::endl;
  return 0;
}
