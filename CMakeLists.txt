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
add_executable(server server/server.cc)

if(UNIX)
    target_compile_options(less PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(server PRIVATE -Wall -Wextra -pedantic)
elseif(WIN32)
    target_compile_options(less PRIVATE -Wall -pedantic)
    target_compile_options(server PRIVATE -Wall -pedantic)
endif()

add_subdirectory(libs)

target_link_libraries(less sfml-graphics sfml-audio sfml-network nlohmann_json::nlohmann_json osdialog sfml-widgets)
target_link_libraries(server sfml-network)
target_include_directories(less PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/contexts/include>
)

#### COPY ALL RESOURCES TO BUILD DIRECTORY ####
file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sounds DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# define dummy target that can be used to create a folder with necessary files
add_custom_target(distribute)
set(DIST_PREFIX ${CMAKE_SOURCE_DIR}/ignore_dist)

#### FOR DISTRIBUTION PUT TARGETS AND RESOURCES IN DIST DIRECTORY ####
add_custom_command(TARGET distribute POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:less> ${DIST_PREFIX}/$<TARGET_FILE_NAME:less>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sfml-graphics> ${DIST_PREFIX}/libs/$<TARGET_FILE_NAME:sfml-graphics>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sfml-audio> ${DIST_PREFIX}/libs/$<TARGET_FILE_NAME:sfml-audio>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sfml-network> ${DIST_PREFIX}/libs/$<TARGET_FILE_NAME:sfml-network>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sfml-widgets> ${DIST_PREFIX}/libs/$<TARGET_FILE_NAME:sfml-widgets>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:osdialog> ${DIST_PREFIX}/libs/$<TARGET_FILE_NAME:osdialog>
    COMMAND ${CMAKE_COMMAND} -E copy_directory resources ${DIST_PREFIX}/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory sounds ${DIST_PREFIX}/sounds
)
