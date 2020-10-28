set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I ${CMAKE_CURRENT_LIST_DIR}")

file(GLOB sources
    "*.cpp"
    "Core/*.cpp"
    "Core/Utils/*.cpp"
    "Core/Window/*.cpp"
    "Memory/*.cpp"
    "MemoryHolders/*.cpp"
    "ObjectCreation/*.cpp"
    "ObjectCreation/Components/*.cpp"
    "Objects/*.cpp"
    "Rendering/*.cpp"
    "View/*.cpp"
)

find_package(Freetype REQUIRED FATAL_ERROR)
find_package(glfw3 REQUIRED FATAL_ERROR)
find_package(glm REQUIRED FATAL_ERROR)
find_package(Vulkan REQUIRED FATAL_ERROR)

add_library(graphics STATIC ${sources})

target_include_directories(graphics PRIVATE ${Vulkan_INCLUDE_DIRS} ${GLM_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIR})
target_link_libraries(graphics ${Vulkan_LIBRARIES} ${GLFW3_LIBRARY} ${FREETYPE_LIBRARIES} utils containers)
add_dependencies(graphics utis containers)
