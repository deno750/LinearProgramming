cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 11)

project(lp)

include_directories(include)

file(GLOB lp_SRC CONFIGURE_DEPENDS "src/*.cpp" )

add_executable(${PROJECT_NAME} ${lp_SRC})
