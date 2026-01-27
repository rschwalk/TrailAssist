//
// Created by rschwalk on 27/01/2026.
//
#include <vector>
#include <algorithm>
#include <numeric>

#include "trail_calculator.h"

int TrailAssist::calculate_daily_distance(const std::vector<int> &hut_distances, const Trail &trail)
{
  // Find the min needed max distance to complete the trail

  auto min_possible_distance_at = std::ranges::max_element(hut_distances.begin(), hut_distances.end());
  int min_possible_distance = *min_possible_distance_at;

  // NOLINTNEXTLINE(boost-use-ranges)
  int max_possible_distance = std::accumulate(hut_distances.begin(), hut_distances.end(), 0);

  while (max_possible_distance > min_possible_distance) {
    const int middle = (min_possible_distance + max_possible_distance) / 2;

    if (check_hike_possibility(hut_distances, trail, middle)) {
      max_possible_distance = middle;
    } else {
      min_possible_distance = middle + 1;
    }
  }

  return min_possible_distance;
}

bool TrailAssist::check_hike_possibility(const std::vector<int> &hut_distances,
  const TrailAssist::Trail &trail,
  const int max_day_distance)
{
  auto day_count{1};
  auto current_distance{0};

  for (const auto &hut_distance : hut_distances) {
    if (hut_distance > max_day_distance) {
      return false;
    }

    if (current_distance + hut_distance <= max_day_distance) {
      current_distance += hut_distance;
    } else {
      day_count++;
      current_distance = hut_distance;
    }

    if (day_count > trail.days) {
      return false;
    }
  }

  return true;
}
