find_package(OpenAL REQUIRED)
file(GLOB sources
    "*.cpp"
)
add_library(audio STATIC ${sources})
target_include_directories(audio PRIVATE ${OPENAL_INCLUDE_DIR})

if (APPLE)
target_link_libraries(audio "-framework OpenAL")
else ()
target_link_libraries(audio ${OPENAL_LIBRARY})
endif ()
