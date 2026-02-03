add_library(fdnbench_options INTERFACE)
add_library(fdnbench::fdnbench_options ALIAS fdnbench_options)
target_compile_features(fdnbench_options INTERFACE cxx_std_23)
target_compile_options(fdnbench_options INTERFACE -march=native)

if(FDNBENCH_USE_SANITIZER)
  if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    message(STATUS "Enabling AddressSanitizer")
    target_compile_options(fdnbench_options INTERFACE $<$<CONFIG:Debug>:-fsanitize=address>)
    target_link_options(fdnbench_options INTERFACE $<$<CONFIG:Debug>:-fsanitize=address>)
  endif()
endif()

include(CheckCXXSymbolExists)

if(cxx_std_20 IN_LIST CMAKE_CXX_COMPILE_FEATURES)
  set(header version)
else()
  set(header ciso646)
endif()

check_cxx_symbol_exists(_LIBCPP_VERSION ${header} LIBCPP)
if(LIBCPP)
  if(FDNBENCH_ENABLE_HARDENING)
    message(STATUS "Enabling libc++ hardening")
    target_compile_definitions(
      fdnbench_options INTERFACE $<$<CONFIG:Debug>:_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG>
                                 $<$<CONFIG:RelWithDebInfo>:_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG>)
  endif()
endif()

check_cxx_symbol_exists(_STD_VERSION_HEADER_ ${header} MSVC_STL)
if(MSVC_STL)
  if(FDNBENCH_ENABLE_HARDENING)
    message(STATUS "Enabling MSVC STL hardening")
    target_compile_definitions(fdnbench_options INTERFACE $<$<CONFIG:Debug>:_MSVC_STL_HARDENING=1>
                                                          $<$<CONFIG:RelWithDebInfo>:_MSVC_STL_HARDENING=1>)
  endif()
endif()
