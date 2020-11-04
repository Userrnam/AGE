# add_subdirectory("Animation")
# add_subdirectory("Audio")
# add_subdirectory("Containers")
# add_subdirectory("Events")
# add_subdirectory("External")
# add_subdirectory("Graphics")
# add_subdirectory("Scene")
# add_subdirectory("UI")
# add_subdirectory("Utils")

file (GLOB sources
    "*.cpp"
    "Animation/*.cpp"
    "Audio/*.cpp"
    "Containers/*.cpp"
    "Events/*.cpp"
    "External/*.cpp"
    "Graphics/*.cpp"
    "Graphics/Core/*.cpp"
    "Graphics/Core/Utils/*.cpp"
    "Graphics/Core/Window/*.cpp"
    "Graphics/Memory/*.cpp"
    "Graphics/MemoryHolders/*.cpp"
    "Graphics/ObjectCreation/*.cpp"
    "Graphics/ObjectCreation/Components/*.cpp"
    "Graphics/Objects/*.cpp"
    "Graphics/Rendering/*.cpp"
    "Graphics/View/*.cpp"
    "Scene/*.cpp"
    "UI/*.cpp"
    "Utils/*.cpp"
)

find_package(OpenAL REQUIRED)

find_package(Freetype REQUIRED FATAL_ERROR)
find_package(glfw3 REQUIRED FATAL_ERROR)
find_package(glm REQUIRED FATAL_ERROR)
find_package(Vulkan REQUIRED FATAL_ERROR)

add_library(age STATIC ${sources})

target_include_directories(age PRIVATE ${OPENAL_INCLUDE_DIR})
target_include_directories(age PRIVATE ${Vulkan_INCLUDE_DIRS} ${GLM_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIR})

target_link_libraries(age ${Vulkan_LIBRARIES} ${GLFW3_LIBRARY} ${FREETYPE_LIBRARIES})

if (APPLE)
target_link_libraries(age "-framework OpenAL")
else ()
target_link_libraries(age ${OPENAL_LIBRARY})
endif ()

set_source_files_properties("External/stb_impl.cpp" PROPERTIES COMPILE_FLAGS "-Ofast" )

# target_link_libraries(age audio graphics utils scene external events animation ui)

# add_dependencies(age graphics utils scene external events animation ui)
