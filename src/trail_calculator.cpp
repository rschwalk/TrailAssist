//
// Created by rschwalk on 27/01/2026.
//

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#include "trail_calculator.h"

int TrailAssist::calculate_daily_distance(const std::vector<int> &hut_distances, const int days)
{
  // Find the min needed max distance to complete the trail
  //
  const auto min_possible_distance_at = std::ranges::max_element(hut_distances.begin(), hut_distances.end());
  int min_possible_distance = *min_possible_distance_at;

  // NOLINTNEXTLINE(boost-use-ranges)
  int max_possible_distance = std::accumulate(hut_distances.begin(), hut_distances.end(), 0);

  while (max_possible_distance > min_possible_distance) {

    if (const int middle = (min_possible_distance + max_possible_distance) / 2;
      check_hike_possibility(hut_distances, middle, days)) {
      max_possible_distance = middle;
    } else {
      min_possible_distance = middle + 1;
    }
  }

  return min_possible_distance;
}

bool TrailAssist::check_hike_possibility(const std::vector<int> &hut_distances,
  const int max_day_distance,
  const int days)
{
  auto day_count{ 1 };
  auto current_distance{ 0 };

  for (const auto &hut_distance : hut_distances) {
    if (hut_distance > max_day_distance) { return false; }

    if (current_distance + hut_distance <= max_day_distance) {
      current_distance += hut_distance;
    } else {
      day_count++;
      current_distance = hut_distance;
    }

    if (day_count > days) { return false; }
  }

  return true;
}
void TrailAssist::print_result(const std::vector<int> &hut_distances, const int max_day_distance)
{
  auto day_distance{ 0 };
  auto day_count{ 1 };
  for (const auto &hut_distance : hut_distances) {
    if ((day_distance + hut_distance) > max_day_distance) {
      std::cout << day_count++ << ". Tag: " << day_distance << " km\n";
      day_distance = hut_distance;
    } else {
      day_distance += hut_distance;
    }
  }
  std::cout << day_count++ << ". Tag: " << day_distance << " km\n";
  std::cout << "\nMaximum: " << max_day_distance << " km\n";
}

void TrailAssist::calculate_daily_distance_from_file(const std::string &filename)
{
  auto data_path = std::filesystem::path(filename);

  if (!std::filesystem::exists(data_path)) {
    std::cerr << "Wrong path given: " << data_path << "\n";
    return;
  }

  std::cout << "Input file: " << data_path << "\n";
  auto file_content = std::ifstream(data_path);

  if (!file_content.is_open()) { std::cerr << "Could not open file: " << data_path << "\n"; }

  int huts = 0;
  int days = 0;
  auto hut_distances = std::vector<int>{};
  auto count = 0;

  try {
    std::string line;
    while (std::getline(file_content, line)) {
      switch (count) {
      case 0:
        huts = std::stoi(line);
        break;
      case 1:
        days = std::stoi(line);
        break;
      default:
        hut_distances.push_back(std::stoi(line));
        break;
      }
      count++;
    }
  } catch (std::invalid_argument const &ex) {
    std::cerr << "Conversation of data not possible: " << ex.what() << '\n';
    return;
  } catch (std::out_of_range const &ex) {
    std::cerr << "Value of distance is to big: " << ex.what() << '\n';
    return;
  }

  if (huts != (count - 2)) {
    std::cerr << "File is invalid, number of huts not matching with the data given!\n";
    return;
  }

  auto min_max_distance = TrailAssist::calculate_daily_distance(hut_distances, days);

  print_result(hut_distances, min_max_distance);

}
