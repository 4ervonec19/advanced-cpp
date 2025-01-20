#ifndef PASSENGERS_HPP
#define PASSENGERS_HPP

#include "unit.hpp"
#include <string>
#include <vector>

class Passenger : public Unit {
protected:
  std::vector<int> hand_luggage_weights;
  std::vector<int> luggage_weights;

public:
  Passenger(const std::string &input_type, const std::vector<int> &handLuggage,
            const std::vector<int> &luggage);

  int getHandLuggageWeight() const override;
  int getLuggageWeight() const override;

  std::vector<int> getHandLuggage() const;
  std::vector<int> getLuggage() const;

  virtual bool canRegister() const override;
};

// Класс EconomyPassenger
class EconomyPassenger : public Passenger {
public:
  EconomyPassenger(const std::vector<int> &handLuggage,
                   const std::vector<int> &luggage);
  bool canRegister() const override;
};

// Класс BusinessPassenger
class BusinessPassenger : public Passenger {
public:
  BusinessPassenger(const std::vector<int> &handLuggage,
                    const std::vector<int> &luggage);
  bool canRegister() const override;
};

// Класс FirstClassPassenger
class FirstClassPassenger : public Passenger {
public:
  FirstClassPassenger(const std::vector<int> &handLuggage,
                      const std::vector<int> &luggage);
  bool canRegister() const override;
};

#endif // PASSENGERS_HPP
