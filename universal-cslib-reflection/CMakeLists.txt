set(BUILD_TESTING OFF)

Include(FetchContent)
FetchContent_Declare(MoreConcepts GIT_REPOSITORY https://github.com/MiSo1289/more_concepts.git GIT_TAG ea5b6815f5929cdb086f76d43f3e9b122bbcaaeb)
FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.8.1)
FetchContent_MakeAvailable(Catch2 MoreConcepts)

add_library(universal-cslib-reflection INTERFACE)
target_compile_features(universal-cslib-reflection INTERFACE cxx_std_20)
target_compile_options(universal-cslib-reflection INTERFACE /clang:-ftemplate-backtrace-limit=0 /clang:-fno-rtti)
target_link_libraries(universal-cslib-reflection INTERFACE universal-cslib simple-reflection more_concepts)
add_subdirectory(src)

add_executable(universal-cslib-reflection-tests)
target_link_libraries(universal-cslib-reflection-tests PRIVATE Catch2::Catch2WithMain universal-cslib-reflection)
add_subdirectory(tests)

include(CTest)
include(Catch)
catch_discover_tests(universal-cslib-reflection-tests)
