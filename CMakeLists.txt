cmake_minimum_required(VERSION 3.0)
project(LessCPP)


## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
if(WIN32)
	set(SFML_DIR "C:/Users/Lev/SFML-2.5.1-64/lib/cmake/SFML")
endif()

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)
add_executable(less less.cc block_shape.cc)
target_link_libraries(less sfml-graphics)
