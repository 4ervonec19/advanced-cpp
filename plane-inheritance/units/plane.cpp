#include "plane.hpp"

// Конструктор
Plane::Plane(int economy_weight, int business_weight, int first_class_weight) {
  segments.emplace_back("ECONOMY", economy_weight);
  segments.emplace_back("BUSINESS", business_weight);
  segments.emplace_back("FIRST", first_class_weight);
}

int Plane::getWeightInfo() const {
  int total = 0;
  for (const auto &segment : segments) {
    total += segment.getCurrentWeight();
  }
  return total;
}

int Plane::getWeightEconomyInfo() const {
  return segments[0].getCurrentWeight();
}
int Plane::getWeightBusinessInfo() const {
  return segments[1].getCurrentWeight();
}
int Plane::getWeightFirstClassInfo() const {
  return segments[2].getCurrentWeight();
}

int Plane::getMaxWeightEconomyInfo() const {
  return segments[0].getMaxWeight();
}
int Plane::getMaxWeightBusinessInfo() const {
  return segments[1].getMaxWeight();
}
int Plane::getMaxWeightFirstClassInfo() const {
  return segments[2].getMaxWeight();
}

bool Plane::canAddToSegment(int segmentIndex, int weight) const {
  return segments[segmentIndex].canAddLuggage(weight);
}

void Plane::addWeightToSegment(int segmentIndex, int weight) {
  segments[segmentIndex].updCurrentWeight(weight);
}

void Plane::relocateLuggageToEconomy(int requiredWeight, int passengerID) {
  int total_weight = 0;
  auto economyLuggage = segments[0].getLuggageWeights();

  for (auto p : economyLuggage) {
    total_weight += p.second;
  }

  if (total_weight < requiredWeight) {
    std::cout << "!!CANNOT ADD LUGGAGE OF PASSENGER, ID = " << passengerID
              << "!!\n";
    return;
  }

  std::sort(economyLuggage.begin(), economyLuggage.end(),
            [](const std::pair<int, int> &a, const std::pair<int, int> &b) {
              return a.second > b.second;
            });

  int currentSum = 0;
  for (auto it = economyLuggage.begin(); it != economyLuggage.end();) {
    int id = it->first;
    int weight = it->second;

    currentSum += weight;
    std::cout << "!!PASSENGER'S LUGGAGE REMOVED FROM ECONOMY, ID = " << id
              << "!!\n";
    segments[0].updCurrentWeight(-weight);
    economyLuggage.erase(it);

    if (currentSum >= requiredWeight) {
      segments[0].updCurrentWeight(requiredWeight);
      segments[0].SetLuggage(economyLuggage);
      return;
    }
  }
}

void Plane::registerUnit(std::unique_ptr<Unit> unit) {
  Passenger *passenger = dynamic_cast<Passenger *>(unit.get());
  if (!passenger)
    return;

  std::vector<int> handLuggage = passenger->getHandLuggage();
  std::vector<int> luggage = passenger->getLuggage();
  int handWeight = std::accumulate(handLuggage.begin(), handLuggage.end(), 0);
  int luggageWeight = std::accumulate(luggage.begin(), luggage.end(), 0);
  int totalWeight = handWeight + luggageWeight;
  bool canRegister = passenger->canRegister();

  if (!canRegister) {
    std::cout << "!!CANT REGISTER " << passenger->getType()
              << " PASSENGER, ID = " << passenger->getID() << "!!\n";
    return;
  }

  int segmentIndex = (passenger->getType() == "ECONOMY")       ? 0
                     : (passenger->getType() == "BUSINESS")    ? 1
                     : (passenger->getType() == "FIRST_CLASS") ? 2
                                                               : -1;

  if (canAddToSegment(segmentIndex, totalWeight)) {
    addWeightToSegment(segmentIndex, totalWeight);
    if (segmentIndex == 0) {
      for (const auto &luggageItem : luggage) {
        segments[0].addLuggage(luggageItem, passenger->getID());
      }
    }
  } else if (segmentIndex == 1 || segmentIndex == 2) {
    if (canAddToSegment(segmentIndex, handWeight)) {
      addWeightToSegment(segmentIndex, handWeight);

      int remainingWeight = luggageWeight;
      if (luggage.size() == 2) {
        int luggage1 = luggage[0], luggage2 = luggage[1];
        if (canAddToSegment(segmentIndex, luggage1) &&
            canAddToSegment(segmentIndex, luggage2)) {
          int max = std::max(luggage1, luggage2);
          addWeightToSegment(segmentIndex, max);
          remainingWeight = std::min(luggage1, luggage2);
        } else if (canAddToSegment(segmentIndex, luggage1)) {
          addWeightToSegment(segmentIndex, luggage1);
          remainingWeight = luggage2;
        } else if (canAddToSegment(segmentIndex, luggage2)) {
          addWeightToSegment(segmentIndex, luggage2);
          remainingWeight = luggage1;
        }
      }

      if (canAddToSegment(0, remainingWeight)) {
        segments[0].updCurrentWeight(remainingWeight);
      } else {
        relocateLuggageToEconomy(remainingWeight, passenger->getID());
      }
    } else {
      std::cout << "!!CANT REGISTER " << passenger->getType()
                << " PASSENGER, ID = " << passenger->getID() << "!!\n";
    }
  }
}
