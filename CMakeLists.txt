cmake_minimum_required(VERSION 3.1)
project(LessCPP CXX)

set(CMAKE_CXX_STANDARD 17)

# USE RPATH FOR EXECUTABLE (must be defined before add_executable)
if(UNIX)
    set(CMAKE_INSTALL_RPATH "lib")
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
    set(CMAKE_SKIP_RPATH FALSE)
endif()

file(GLOB sources "src/*")
file(GLOB context_sources "contexts/src/*")
add_executable(less ${sources} ${context_sources})

if(UNIX)
    target_compile_options(less PRIVATE -Wall -Wextra -pedantic)
elseif(WIN32)
    target_compile_options(less PRIVATE -Wall -pedantic)
endif()

add_subdirectory(libs)

target_link_libraries(less sfml-graphics sfml-audio sfml-network nlohmann_json::nlohmann_json osdialog sfml-widgets)
target_include_directories(less PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/contexts/include>
)

#### COPY ALL RESOURCES TO BUILD DIRECTORY ####
file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sounds DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#### FOR INSTALLING PUT TARGETS AND RESOURCES IN INSTALL DIRECTORY ####
install(TARGETS less DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY resources sounds DESTINATION ${CMAKE_INSTALL_PREFIX})

