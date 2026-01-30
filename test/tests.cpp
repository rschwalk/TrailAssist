#include <catch2/catch_test_macros.hpp>

#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

#include "trail_calculator.h"

class AutoRestoreRdbuf
{
  std::ostream &out;
  std::streambuf *old;

public:
  ~AutoRestoreRdbuf() { out.rdbuf(old); }
  AutoRestoreRdbuf(const AutoRestoreRdbuf &) = delete;
  AutoRestoreRdbuf(AutoRestoreRdbuf &&) = delete;
  explicit AutoRestoreRdbuf(std::ostream &out) : out{ out }, old{ out.rdbuf() } {}
};

std::string captureOutput(const std::function<void()> &func)
{
  AutoRestoreRdbuf const restore{ std::cout };
  std::ostringstream const oss;
  std::cout.rdbuf(oss.rdbuf());
  func();
  return oss.str();
}

std::string captureError(const std::function<void()> &func)
{
  AutoRestoreRdbuf const restore{ std::cerr };
  std::ostringstream const oss;
  std::cerr.rdbuf(oss.rdbuf());
  func();
  return oss.str();
}

TEST_CASE("Calculated distance has the expected result")
{
  std::vector<int> hut_distances{ 11, 16, 5, 5, 12, 10 }; // NOLINT(*-avoid-magic-numbers)
  TrailAssist::Trail const trail(3, hut_distances);

  const int distance = trail.calculate_daily_distance();
  REQUIRE(distance == 26);
}

TEST_CASE("Standard file input")
{

  auto output = captureOutput([]() {
    std::string const input{ "../../test/input_standard.txt" };
    TrailAssist::calculate_daily_distance_from_file(input);
  });

  const auto *expected_result {
    R"(1. Tag: 11 km
2. Tag: 26 km
3. Tag: 22 km

Maximum: 26 km
)"};

  REQUIRE(output == expected_result);

  auto output_2 = captureOutput([]() {
    std::string const input{ "../../test/input_standard_v2.txt" };
    TrailAssist::calculate_daily_distance_from_file(input);
  });

  const auto *const expected_result_2 {
    R"(1. Tag: 11 km
2. Tag: 26 km
3. Tag: 22 km
4. Tag: 11 km
5. Tag: 26 km
6. Tag: 22 km

Maximum: 26 km
)"};

  REQUIRE(output_2 == expected_result_2);
}

TEST_CASE("Wrong input handling")
{
  auto output = captureError([]() {
    std::string const input{ "../../test/input_wrong_missmatch.txt" };
    TrailAssist::calculate_daily_distance_from_file(input);
  });

  const auto *const expected_result {"File is invalid, number of huts not matching with the data given!\n"};

  REQUIRE(output == expected_result);

  auto output_2 = captureError([]() {
    std::string const input{ "../../test/input_wrong_string.txt" };
    TrailAssist::calculate_daily_distance_from_file(input);
  });

  const auto *const expected_result_2 {"Conversation of data not possible: stoi: no conversion\n"};

  REQUIRE(output_2 == expected_result_2);
};
