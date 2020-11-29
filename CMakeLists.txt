cmake_minimum_required (VERSION 3.0)

# verbose makefile
set(CMAKE_VERBOSE_MAKEFILE OFF)

project (temperature-monitor VERSION 0.1.0)

# project version
message(STATUS "Project version: ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# get git revision
find_package(Git)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-list --max-count=1 HEAD
        OUTPUT_VARIABLE GIT_REVISION
        ERROR_QUIET
        )
    if(NOT ${GIT_REVISION} STREQUAL "")
        string(STRIP ${GIT_REVISION} GIT_REVISION)
    endif()
    message(STATUS "Current git revision is ${GIT_REVISION}")
else()
    set(GIT_REVISION "unknown")
endif()


# include GNU standard dirs
include(GNUInstallDirs)


# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
  )

# add the binary tree to the search path for include files
# so that we will find Config.h
include_directories("${PROJECT_BINARY_DIR}")


# add local cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# libknxusb
find_package(LibKnxUsb REQUIRED)

# pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set (CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set (CPACK_SET_DESTDIR "ON") # absolute path install
set (CPACK_GENERATOR "STGZ;TGZ;DEB")
set (CPACK_DEBIAN_PACKAGE_MAINTAINER "giursino") #required
include (CPack)


add_subdirectory(src)
add_subdirectory(utils)


# cmocka
option(ENABLE_TESTS "Perform unit tests after build" OFF)
if (ENABLE_TESTS)
  find_package(CMocka CONFIG REQUIRED)
  include(AddCMockaTest)
  include(AddMockedTest)
  add_subdirectory(test)
  enable_testing()
endif(ENABLE_TESTS)
