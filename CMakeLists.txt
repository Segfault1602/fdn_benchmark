cmake_minimum_required(VERSION 4.1.1)

project(
  PROJECT_NAME
  VERSION 0.0.1
  DESCRIPTION "PROJECT_DESCRIPTION")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(FDNBENCH_ENABLE_HARDENING "Enable hardening" ON)
option(FDNBENCH_USE_SANITIZER "Use AddressSanitizer" ON)

include(cmake/project-warnings.cmake)
include(cmake/project-options.cmake)

include(FetchContent)
FetchContent_Declare(
  CPM
  GIT_REPOSITORY https://github.com/cpm-cmake/CPM.cmake
  GIT_TAG v0.42.1)
FetchContent_MakeAvailable(CPM)
include(${cpm_SOURCE_DIR}/cmake/CPM.cmake)

cpmaddpackage("gh:catchorg/Catch2@3.12.0")
cpmaddpackage("gh:martinus/nanobench#master")
cpmaddpackage("gh:adamstark/AudioFile#1.1.4")

cpmaddpackage(
  URI
  "gh:google/benchmark@1.9.5"
  OPTIONS
  "BENCHMARK_DOWNLOAD_DEPENDENCIES ON")

cpmaddpackage(
  NAME
  Eigen
  GIT_TAG
  5.0.1
  GIT_REPOSITORY
  https://gitlab.com/libeigen/eigen)

if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
endif()

add_subdirectory(external)
add_subdirectory(src)
