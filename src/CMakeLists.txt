add_executable(trailassist main.cpp
        ../include/TrailAssist/trail_calculator.h)

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

target_include_directories(trailassist PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

# Apply WASM-specific configuration
if(EMSCRIPTEN)
  TrailAssist_configure_wasm_target(trailassist
    TITLE "TrailAssist - Helper for hiking"
    DESCRIPTION "TrailAssist will help you to figure out the best route."
  )
endif()
