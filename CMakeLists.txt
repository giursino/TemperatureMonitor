cmake_minimum_required (VERSION 3.0)

project (temeperature-monitor)

include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# libknxusb
find_package(LibKnxUsb REQUIRED)

# pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(src)

# cmocka
option(ENABLE_TESTS "Perform unit tests after build" OFF)
if (ENABLE_TESTS)
  find_package(CMocka CONFIG REQUIRED)
  include(AddCMockaTest)
  include(AddMockedTest)
  add_subdirectory(test)
  enable_testing()
endif(ENABLE_TESTS)
