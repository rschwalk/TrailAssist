#include <catch2/catch_test_macros.hpp>

#include <vector>
#include "trail_calculator.h"


TEST_CASE("Calculated distance has the expected result", "[distance]")
{

  std::vector<int> const hut_distances{11, 16, 5, 5, 12, 10};

  const int distance = TrailAssist::calculate_daily_distance(hut_distances);
  REQUIRE(distance == 26);
}
