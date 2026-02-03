if(MSVC)
  set(FDNBENCH_WARNINGS_CXX /W3 /permissive-)
elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  set(FDNBENCH_WARNINGS_CXX
      -Wall
      -Wextra
      -Wpedantic
      -Wno-sign-compare
      -Wno-language-extension-token
      #   -Wunsafe-buffer-usage
  )

endif()

add_library(fdnbench_warnings INTERFACE)
add_library(fdnbench::fdnbench_warnings ALIAS fdnbench_warnings)
target_compile_options(fdnbench_warnings INTERFACE ${FDNBENCH_WARNINGS_CXX})
