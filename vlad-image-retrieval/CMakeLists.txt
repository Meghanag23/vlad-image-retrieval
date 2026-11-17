cmake_minimum_required(VERSION 3.10)
project(vlad_image_retrieval CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(vlad_search ${SOURCES})
target_include_directories(vlad_search PRIVATE ${CMAKE_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(vlad_search ${OpenCV_LIBS})
