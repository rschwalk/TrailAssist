#include <catch2/catch_test_macros.hpp>

#include "trail_calculator.h"
#include <vector>


TEST_CASE("Calculated distance has the expected result", "[distance]")
{

  std::vector<int> const hut_distances{ 11, 16, 5, 5, 12, 10 };

  constexpr int days{ 3 };

  const int distance = TrailAssist::calculate_daily_distance(hut_distances, days);
  REQUIRE(distance == 26);
}
