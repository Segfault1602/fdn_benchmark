add_executable(main main.cpp)
target_link_libraries(
  main
  PRIVATE benchmark::benchmark
          AudioFile
          silvin_fdn
          fdnbench_options
          fdnbench_warnings
          libphonon.dylib)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/steamaudio/include)
target_link_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/steamaudio/lib/osx)
