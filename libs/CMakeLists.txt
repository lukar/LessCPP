add_subdirectory(nlohmann_json)
set(JSON_BuildTests OFF CACHE INTERNAL "")


add_subdirectory(SFML)
if(WIN32)
        set(SFML_STATIC_LIBRARIES TRUE)
        set(BUILD_SHARED_LIBS FALSE)
endif()
if(UNIX)
        install(TARGETS sfml-graphics sfml-audio sfml-network DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_RPATH})
endif()
set(SFML_INSTALL_CONFIG_FILES FALSE)


add_subdirectory(osdialog)

add_subdirectory(SFMLWidgets)
