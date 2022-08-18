cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

project(MetaEngine)

include_directories(${PROJECT_SOURCE_DIR}/Framework/Interface)
include_directories(${PROJECT_SOURCE_DIR}/Framework/Common)

add_subdirectory(Framework)
add_subdirectory(Empty)