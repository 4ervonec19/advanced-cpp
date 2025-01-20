#include "passengers.hpp"
#include <numeric>

// Реализация конструктора и методов Passenger
Passenger::Passenger(const std::string &input_type,
                     const std::vector<int> &handLuggage,
                     const std::vector<int> &luggage)
    : Unit(input_type), hand_luggage_weights(handLuggage),
      luggage_weights(luggage) {}

int Passenger::getHandLuggageWeight() const {
  return std::accumulate(hand_luggage_weights.begin(),
                         hand_luggage_weights.end(), 0);
}

int Passenger::getLuggageWeight() const {
  return std::accumulate(luggage_weights.begin(), luggage_weights.end(), 0);
}

std::vector<int> Passenger::getHandLuggage() const {
  return hand_luggage_weights;
}

std::vector<int> Passenger::getLuggage() const { return luggage_weights; }

bool Passenger::canRegister() const { return true; }

// Реализация конструктора и методов EconomyPassenger
EconomyPassenger::EconomyPassenger(const std::vector<int> &handLuggage,
                                   const std::vector<int> &luggage)
    : Passenger("ECONOMY", handLuggage, luggage) {}

bool EconomyPassenger::canRegister() const {
  if (hand_luggage_weights.size() > 1)
    return false;
  if (getHandLuggageWeight() > 10)
    return false;
  if (luggage_weights.size() > 1)
    return false;
  if (getLuggageWeight() > 24)
    return false;
  return true;
}

// Реализация конструктора и методов BusinessPassenger
BusinessPassenger::BusinessPassenger(const std::vector<int> &handLuggage,
                                     const std::vector<int> &luggage)
    : Passenger("BUSINESS", handLuggage, luggage) {}

bool BusinessPassenger::canRegister() const {
  if (hand_luggage_weights.size() > 2)
    return false;
  if (getHandLuggageWeight() > 24)
    return false;
  if (luggage_weights.size() > 2)
    return false;
  if (getLuggageWeight() > 40)
    return false;
  return true;
}

// Реализация конструктора и методов FirstClassPassenger
FirstClassPassenger::FirstClassPassenger(const std::vector<int> &handLuggage,
                                         const std::vector<int> &luggage)
    : Passenger("FIRST_CLASS", handLuggage, luggage) {}

bool FirstClassPassenger::canRegister() const {
  if (hand_luggage_weights.size() > 2)
    return false;
  if (getHandLuggageWeight() > 60)
    return false;
  if (luggage_weights.size() > 2)
    return false;
  return true;
}
