include_directories("External" "Graphics")

add_subdirectory("Animation")
add_subdirectory("Audio")
add_subdirectory("Containers")
add_subdirectory("Events")
add_subdirectory("External")
add_subdirectory("Graphics")
add_subdirectory("Scene")
add_subdirectory("Utils")

file (GLOB sources
    "*.cpp"
)
add_library(age STATIC ${sources})
target_link_libraries(age audio graphics utils scene external events animation)

add_dependencies(age graphics utils scene external events animation)
