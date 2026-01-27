add_executable(trailassist main.cpp
        trail_calculator.cpp)

target_link_libraries(
        trailassist
        PRIVATE TrailAssist::TrailAssist_options
        TrailAssist::TrailAssist_warnings)

target_link_system_libraries(
        trailassist
        PRIVATE
        CLI11::CLI11
        fmt::fmt
        spdlog::spdlog
        lefticus::tools)

target_include_directories(
        trailassist
        PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
