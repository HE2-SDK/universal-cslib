cmake_minimum_required(VERSION 3.28)
project(universal-cslib VERSION 1.0.51
                 DESCRIPTION "Universal CSLib"
                 LANGUAGES CXX)

option(UCSL_EIGEN_MATH "Use Eigen for math objects.")

include(FetchContent)
FetchContent_Declare(Eigen3 GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git GIT_TAG 0fb2ed140d4fc0108553ecfb25f2d7fc1a9319a1 EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(Eigen3)
FetchContent_Declare(simple-reflection GIT_REPOSITORY https://github.com/angryzor/simple-reflection.git GIT_TAG v1.0.3 EXCLUDE_FROM_ALL FIND_PACKAGE_ARGS 1.0.3)
FetchContent_MakeAvailable(simple-reflection)

add_subdirectory(universal-cslib)
add_subdirectory(universal-cslib-reflection)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(universal-cslib-config-version.cmake VERSION ${PROJECT_VERSION} COMPATIBILITY SameMinorVersion)
configure_file(cmake/universal-cslib-config.cmake "${CMAKE_CURRENT_BINARY_DIR}/universal-cslib-config.cmake" COPYONLY)
export(TARGETS universal-cslib universal-cslib-reflection eigen FILE universal-cslib-targets.cmake)
export(PACKAGE universal-cslib)
