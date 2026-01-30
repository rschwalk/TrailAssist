#include <exception>
#include <fmt/base.h>
#include <fmt/format.h>

#include <CLI/CLI.hpp>

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `TrailAssist`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>
#include <iostream>
#include <string>

#include "trail_calculator.h"

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, const char **argv)
{
  try {
    CLI::App app{ fmt::format("{} version {}", TrailAssist::cmake::project_name, TrailAssist::cmake::project_version) };

    std::string input_file;
    app.add_option("input_file", input_file, "Input file")->required();

    CLI11_PARSE(app, argc, argv);

    TrailAssist::calculate_daily_distance_from_file(input_file);

  } catch (const std::exception &e) {
    std::cout << "Unhandled exception in main: " << e.what() << "\n";
  }
}
