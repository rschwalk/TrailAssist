//
// Created by rschwalk on 27/01/2026.
//

#ifndef TRAILASSIST_TRAIL_CALCULATOR_H
#define TRAILASSIST_TRAIL_CALCULATOR_H
#include <string>
#include <vector>
#include <filesystem>

namespace TrailAssist {

void calculate_daily_distance_from_file(const std::string &filename);
int calculate_daily_distance(const std::vector<int> &hut_distances, const int days);
bool check_hike_possibility(const std::vector<int> &hut_distances, const int max_day_distance, const int days);
void print_result(const std::vector<int> &hut_distances, const int max_day_distance);

}// namespace TrailAssist
#endif// TRAILASSIST_TRAIL_CALCULATOR_H
