add_library(sfml-widgets src/button.cc src/text_input.cc)

target_link_libraries(sfml-widgets sfml-graphics)

target_include_directories(sfml-widgets PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)

if(UNIX)
    install(TARGETS sfml-widgets DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_RPATH})
endif()
