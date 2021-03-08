add_subdirectory(nlohmann_json)
set(JSON_BuildTests OFF CACHE INTERNAL "")

if(WIN32)
        set(SFML_STATIC_LIBRARIES TRUE)
        set(BUILD_SHARED_LIBS FALSE)
endif()
add_subdirectory(SFML)


add_subdirectory(osdialog)

add_subdirectory(SFMLWidgets)
