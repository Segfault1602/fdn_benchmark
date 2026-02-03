add_library(silvin_fdn)

target_sources(
  silvin_fdn
  PRIVATE EQComb.cpp EQFilter.cpp FDN.cpp
  PUBLIC FILE_SET
         HEADERS
         FILES
         EQComb.h
         EQFilter.h
         FDN.h
         Global.h)

target_link_libraries(silvin_fdn PRIVATE fdnbench_options fdnbench_warnings)
target_link_libraries(silvin_fdn PUBLIC Eigen3::Eigen)
target_compile_features(silvin_fdn PRIVATE cxx_std_23)
