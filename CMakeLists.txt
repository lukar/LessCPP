cmake_minimum_required(VERSION 3.1)
project(LessCPP CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB source_files "src/*")
add_executable(less ${source_files})

if(UNIX)
	set(CMAKE_INSTALL_RPATH lib)
	set(CMAKE_BUILD_RPATH lib)
	set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
	set(CMAKE_SKIP_INSTALL_RPATH FALSE)
	target_compile_options(less PRIVATE -Wall -Wextra -pedantic) 
elseif(WIN32)
	set(SFML_STATIC_LIBRARIES TRUE)
	set(BUILD_SHARED_LIBS FALSE)
	target_compile_options(less PRIVATE -Wall -pedantic) 
endif()


set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(libs/nlohmann_json)

add_subdirectory(libs/SFML)

add_subdirectory(libs/osdialog)

target_link_libraries(less sfml-graphics sfml-audio nlohmann_json::nlohmann_json osdialog)
target_include_directories(less PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/osdialog>
)

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
