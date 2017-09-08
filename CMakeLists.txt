cmake_minimum_required(VERSION 3.5)

# Build TBB
list(APPEND CMAKE_PREFIX_PATH tbb/cmake)
set(TBB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/tbb")
include(${TBB_ROOT}/cmake/TBBBuild.cmake)
tbb_build(TBB_ROOT ${TBB_ROOT})
find_package(TBB REQUIRED tbb)
set(TBB_INCLUDE_DIRS "${TBB_ROOT}/include")
set(TBB_LIBRARIES ${TBB_IMPORTED_TARGETS})

add_executable(tbb-test src/tbb_test.cpp)
set_property(TARGET tbb-test PROPERTY CXX_STANDARD 11)
target_link_libraries(tbb-test ${TBB_LIBRARIES} pthread)
target_include_directories(tbb-test PRIVATE ${TBB_INCLUDE_DIRS})
