add_subdirectory(Graphics)

# build age
include_directories(include)
file (GLOB sources
"src/*.cpp"
)
add_library(age STATIC ${sources})
target_include_directories(age PRIVATE Graphics/Core/include)
target_link_libraries(age graphics)

add_executable(tst test/src/main.cpp)
target_include_directories(tst PRIVATE Graphics)

add_dependencies(age graphics)
add_dependencies(tst age)

target_link_libraries(tst age)

# save exe path
file(RELATIVE_PATH exe_path ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
file(WRITE ${PROJECT_SOURCE_DIR}/.build/exe_path.txt ".build/${exe_path}/tst")
