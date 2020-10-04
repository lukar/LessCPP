cmake_minimum_required(VERSION 3.1)
project(LessCPP)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

file(GLOB source_files "src/*")

if(UNIX)
	set(CMAKE_INSTALL_RPATH lib)
	set(CMAKE_SKIP_INSTALL_RPATH FALSE)
elseif(WIN32)
	set(SFML_STATIC_LIBRARIES TRUE)
endif()

add_executable(less ${source_files})

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(libs/nlohmann_json)

add_subdirectory(libs/SFML)

target_link_libraries(less sfml-graphics sfml-audio nlohmann_json::nlohmann_json)
include_directories(include)

file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sounds DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(WIN32)
    file(GLOB DLLS "${SFML_FOLDER}/bin/*.dll")
    file(COPY ${DLLS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

# INSTALL
install(TARGETS less DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY resources sounds DESTINATION ${CMAKE_INSTALL_PREFIX})

if(UNIX)
	file(GLOB sfml_libs "${CMAKE_CURRENT_BINARY_DIR}/libs/SFML/lib/*.so*")
	install(FILES ${sfml_libs} DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()
