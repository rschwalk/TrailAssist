include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


include(CheckCXXSourceCompiles)


macro(TrailAssist_supports_sanitizers)
  # Emscripten doesn't support sanitizers
  if(EMSCRIPTEN)
    set(SUPPORTS_UBSAN OFF)
    set(SUPPORTS_ASAN OFF)
  elseif((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)

    message(STATUS "Sanity checking UndefinedBehaviorSanitizer, it should be supported on this platform")
    set(TEST_PROGRAM "int main() { return 0; }")

    # Check if UndefinedBehaviorSanitizer works at link time
    set(CMAKE_REQUIRED_FLAGS "-fsanitize=undefined")
    set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=undefined")
    check_cxx_source_compiles("${TEST_PROGRAM}" HAS_UBSAN_LINK_SUPPORT)

    if(HAS_UBSAN_LINK_SUPPORT)
      message(STATUS "UndefinedBehaviorSanitizer is supported at both compile and link time.")
      set(SUPPORTS_UBSAN ON)
    else()
      message(WARNING "UndefinedBehaviorSanitizer is NOT supported at link time.")
      set(SUPPORTS_UBSAN OFF)
    endif()
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    if (NOT WIN32)
      message(STATUS "Sanity checking AddressSanitizer, it should be supported on this platform")
      set(TEST_PROGRAM "int main() { return 0; }")

      # Check if AddressSanitizer works at link time
      set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
      set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=address")
      check_cxx_source_compiles("${TEST_PROGRAM}" HAS_ASAN_LINK_SUPPORT)

      if(HAS_ASAN_LINK_SUPPORT)
        message(STATUS "AddressSanitizer is supported at both compile and link time.")
        set(SUPPORTS_ASAN ON)
      else()
        message(WARNING "AddressSanitizer is NOT supported at link time.")
        set(SUPPORTS_ASAN OFF)
      endif()
    else()
      set(SUPPORTS_ASAN ON)
    endif()
  endif()
endmacro()

macro(TrailAssist_setup_options)
  option(TrailAssist_ENABLE_HARDENING "Enable hardening" ON)
  option(TrailAssist_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    TrailAssist_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    TrailAssist_ENABLE_HARDENING
    OFF)

  TrailAssist_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR TrailAssist_PACKAGING_MAINTAINER_MODE)
    option(TrailAssist_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(TrailAssist_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(TrailAssist_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(TrailAssist_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(TrailAssist_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(TrailAssist_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(TrailAssist_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(TrailAssist_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(TrailAssist_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(TrailAssist_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(TrailAssist_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(TrailAssist_ENABLE_PCH "Enable precompiled headers" OFF)
    option(TrailAssist_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(TrailAssist_ENABLE_IPO "Enable IPO/LTO" ON)
    option(TrailAssist_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(TrailAssist_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(TrailAssist_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(TrailAssist_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(TrailAssist_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(TrailAssist_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(TrailAssist_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(TrailAssist_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(TrailAssist_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(TrailAssist_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(TrailAssist_ENABLE_PCH "Enable precompiled headers" OFF)
    option(TrailAssist_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      TrailAssist_ENABLE_IPO
      TrailAssist_WARNINGS_AS_ERRORS
      TrailAssist_ENABLE_USER_LINKER
      TrailAssist_ENABLE_SANITIZER_ADDRESS
      TrailAssist_ENABLE_SANITIZER_LEAK
      TrailAssist_ENABLE_SANITIZER_UNDEFINED
      TrailAssist_ENABLE_SANITIZER_THREAD
      TrailAssist_ENABLE_SANITIZER_MEMORY
      TrailAssist_ENABLE_UNITY_BUILD
      TrailAssist_ENABLE_CLANG_TIDY
      TrailAssist_ENABLE_CPPCHECK
      TrailAssist_ENABLE_COVERAGE
      TrailAssist_ENABLE_PCH
      TrailAssist_ENABLE_CACHE)
  endif()

  TrailAssist_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (TrailAssist_ENABLE_SANITIZER_ADDRESS OR TrailAssist_ENABLE_SANITIZER_THREAD OR TrailAssist_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(TrailAssist_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(TrailAssist_global_options)
  if(TrailAssist_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    TrailAssist_enable_ipo()
  endif()

  TrailAssist_supports_sanitizers()

  if(TrailAssist_ENABLE_HARDENING AND TrailAssist_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR TrailAssist_ENABLE_SANITIZER_UNDEFINED
       OR TrailAssist_ENABLE_SANITIZER_ADDRESS
       OR TrailAssist_ENABLE_SANITIZER_THREAD
       OR TrailAssist_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${TrailAssist_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${TrailAssist_ENABLE_SANITIZER_UNDEFINED}")
    TrailAssist_enable_hardening(TrailAssist_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(TrailAssist_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(TrailAssist_warnings INTERFACE)
  add_library(TrailAssist_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  TrailAssist_set_project_warnings(
    TrailAssist_warnings
    ${TrailAssist_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  # Linker and sanitizers not supported in Emscripten
  if(NOT EMSCRIPTEN)
    if(TrailAssist_ENABLE_USER_LINKER)
      include(cmake/Linker.cmake)
      TrailAssist_configure_linker(TrailAssist_options)
    endif()

    include(cmake/Sanitizers.cmake)
    TrailAssist_enable_sanitizers(
      TrailAssist_options
      ${TrailAssist_ENABLE_SANITIZER_ADDRESS}
      ${TrailAssist_ENABLE_SANITIZER_LEAK}
      ${TrailAssist_ENABLE_SANITIZER_UNDEFINED}
      ${TrailAssist_ENABLE_SANITIZER_THREAD}
      ${TrailAssist_ENABLE_SANITIZER_MEMORY})
  endif()

  set_target_properties(TrailAssist_options PROPERTIES UNITY_BUILD ${TrailAssist_ENABLE_UNITY_BUILD})

  if(TrailAssist_ENABLE_PCH)
    target_precompile_headers(
      TrailAssist_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(TrailAssist_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    TrailAssist_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(TrailAssist_ENABLE_CLANG_TIDY)
    TrailAssist_enable_clang_tidy(TrailAssist_options ${TrailAssist_WARNINGS_AS_ERRORS})
  endif()

  if(TrailAssist_ENABLE_CPPCHECK)
    TrailAssist_enable_cppcheck(${TrailAssist_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(TrailAssist_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    TrailAssist_enable_coverage(TrailAssist_options)
  endif()

  if(TrailAssist_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(TrailAssist_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(TrailAssist_ENABLE_HARDENING AND NOT TrailAssist_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR TrailAssist_ENABLE_SANITIZER_UNDEFINED
       OR TrailAssist_ENABLE_SANITIZER_ADDRESS
       OR TrailAssist_ENABLE_SANITIZER_THREAD
       OR TrailAssist_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    TrailAssist_enable_hardening(TrailAssist_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
