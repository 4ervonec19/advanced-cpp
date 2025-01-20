#ifndef PLANE_HPP
#define PLANE_HPP

#include "passengers.hpp"
#include "segments.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

class Plane {
private:
  std::vector<Segment> segments;

public:
  Plane(int economy_weight, int business_weight, int first_class_weight);

  int getWeightInfo() const;
  int getWeightEconomyInfo() const;
  int getWeightBusinessInfo() const;
  int getWeightFirstClassInfo() const;
  int getMaxWeightEconomyInfo() const;
  int getMaxWeightBusinessInfo() const;
  int getMaxWeightFirstClassInfo() const;

  bool canAddToSegment(int segmentIndex, int weight) const;
  void addWeightToSegment(int segmentIndex, int weight);

  void relocateLuggageToEconomy(int requiredWeight, int passengerID);
  void registerUnit(std::unique_ptr<Unit> unit);
};

#endif // PLANE_HPP
