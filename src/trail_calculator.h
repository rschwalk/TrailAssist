//
// Created by rschwalk on 27/01/2026.
//

#ifndef TRAILASSIST_TRAIL_CALCULATOR_H
#define TRAILASSIST_TRAIL_CALCULATOR_H
#include <string>
#include <vector>

namespace TrailAssist {

struct Trail
{
  int days;
  std::vector<int> hut_distances;

  Trail(int days, std::vector<int> &hut_distances);
  Trail(const Trail &trail) = delete;
  Trail &operator=(const Trail &trail) = delete;
  Trail(Trail &&trail) = delete;
  Trail &operator=(Trail &&trail) = delete;
  ~Trail() = default;

  [[nodiscard]] int calculate_daily_distance() const;
  [[nodiscard]] bool check_hike_possibility(const int max_day_distance) const;
  void print_result(const int max_day_distance) const;
};
void calculate_daily_distance_from_file(const std::string &filename);

}// namespace TrailAssist
#endif// TRAILASSIST_TRAIL_CALCULATOR_H
