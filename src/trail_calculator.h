//
// Created by rschwalk on 27/01/2026.
//

#ifndef TRAILASSIST_TRAIL_CALCULATOR_H
#define TRAILASSIST_TRAIL_CALCULATOR_H
#include <vector>

namespace TrailAssist {
  struct Trail
  {
    int days;
  };
  int calculate_daily_distance(const std::vector<int> &hut_distances, const Trail &trail);
  bool check_hike_possibility(const std::vector<int> &hut_distances, const Trail &trail, const int max_day_distance);
}
#endif// TRAILASSIST_TRAIL_CALCULATOR_H
