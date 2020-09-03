file(GLOB sources
    "*.cpp"
)
add_library(audio STATIC ${sources})
if (APPLE)
target_link_libraries(audio "-framework OpenAL")
else (APPLE)
target_link_libraries(audio openal)
endif (APPLE)
