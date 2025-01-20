#ifndef SEGMENTS_HPP
#define SEGMENTS_HPP

#include <algorithm>
#include <functional>
#include <numeric>
#include <string>
#include <vector>

class Segment {
protected:
  std::string segment_type;
  int max_weight;
  int current_weight;
  std::vector<std::pair<int, int>> luggage_weights;

public:
  Segment(const std::string &segment_type_, int weight_limit)
      : segment_type(segment_type_), max_weight(weight_limit),
        current_weight(0) {}

  bool canAddLuggage(int weight) const {
    return (current_weight + weight <= max_weight);
  }

  void addLuggage(int weight, int id) {
    luggage_weights.push_back({id, weight});
  }

  int getCurrentWeight() const { return current_weight; }

  void updCurrentWeight(int weight) { current_weight += weight; }

  std::string getType() const { return segment_type; }

  int getMaxWeight() const { return max_weight; }

  std::vector<std::pair<int, int>> getLuggageWeights() const {
    return luggage_weights;
  }

  void drop_luggage(int id, int weight) {
    auto it =
        std::find_if(luggage_weights.begin(), luggage_weights.end(),
                     [id, weight](const std::pair<int, int> &luggage) {
                       return luggage.first == id && luggage.second == weight;
                     });

    if (it != luggage_weights.end()) {
      luggage_weights.erase(it);
    }
  }
  void SetLuggage(std::vector<std::pair<int, int>> upd) {
    luggage_weights = upd;
  }
};

#endif
