include_directories(include)

file (GLOB sources "src/*.cpp")

add_library(core STATIC ${sources})
target_link_libraries(core vulkan.1 glfw vulkan.1 vulkan.1.2.131)

add_subdirectory(tests)
