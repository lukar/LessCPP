cmake_minimum_required(VERSION 3.1)
project(LessCPP)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)
add_definitions(-DGUI)

file(GLOB source_files "src/*")
file(GLOB header_files "include/*")

if(UNIX)
	set(CMAKE_INSTALL_RPATH ../lib)
	set(CMAKE_SKIP_INSTALL_RPATH FALSE)
elseif(WIN32)
	set(SFML_STATIC_LIBRARIES TRUE)
	set(SFML_DIR "C:/Users/AvianY/Documents/lib/SFML-2.5.1/lib/cmake/SFML")
endif()


find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
add_executable(less ${source_files} ${header_files})
target_include_directories(less PRIVATE include)
target_link_libraries(less sfml-graphics sfml-audio)

install(TARGETS less DESTINATION bin)
install(FILES resources/Roboto_Medium.ttf DESTINATION bin)

if(UNIX)
	install(FILES
        /usr/lib/x86_64-linux-gnu/libsfml-graphics.so
        /usr/lib/x86_64-linux-gnu/libsfml-graphics.so.2.5
        /usr/lib/x86_64-linux-gnu/libsfml-graphics.so.2.5.1
        /usr/lib/x86_64-linux-gnu/libsfml-window.so
        /usr/lib/x86_64-linux-gnu/libsfml-window.so.2.5
        /usr/lib/x86_64-linux-gnu/libsfml-window.so.2.5.1
        /usr/lib/x86_64-linux-gnu/libsfml-system.so
        /usr/lib/x86_64-linux-gnu/libsfml-system.so.2.5
        /usr/lib/x86_64-linux-gnu/libsfml-system.so.2.5.1
        DESTINATION lib)
endif()
