cmake_minimum_required(VERSION 3.1)
project(LessCPP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -std=c++17")

set(CMAKE_BUILD_RPATH "./SFML-2.5.1_linux/lib")
set(CMAKE_INSTALL_PREFIX "solution")
set(CMAKE_INSTALL_RPATH "lib")

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)
add_executable(less less.cc block_shape.cc helpers.cc player.cc wall.cc)
target_link_libraries(less sfml-graphics)

install(TARGETS less DESTINATION bin)
install(FILES resources/Roboto_Medium.ttf DESTINATION resources)
install(FILES /usr/lib/x86_64-linux-gnu/libsfml-graphics.so.2.5.1 DESTINATION lib)