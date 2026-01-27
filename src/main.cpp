#include <cstdlib>
#include <exception>
#include <fmt/base.h>
#include <fmt/format.h>
#include <optional>

#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `TrailAssist`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>
#include <string>
#include <vector>

#include "trail_calculator.h"

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, const char **argv)
{
  try {
    CLI::App app{ fmt::format("{} version {}", TrailAssist::cmake::project_name, TrailAssist::cmake::project_version) };

    std::optional<std::string> message;
    app.add_option("-m,--message", message, "A message to print back out");
    bool show_version = false;
    app.add_flag("--version", show_version, "Show version information");

    CLI11_PARSE(app, argc, argv);

    spdlog::info("Hello to TrailAssist!");

    const std::vector<int> hut_distances{11, 16, 5, 5, 12, 10};

    constexpr TrailAssist::Trail days{3};

    int distance = TrailAssist::calculate_daily_distance(hut_distances, days);

    spdlog::info(fmt::format("Optimized max daily distance: {}", distance));

  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
  }
}
