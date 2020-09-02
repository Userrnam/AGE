file(GLOB sources
    "src/*.cpp"
)

add_executable(tst ${sources})
target_link_libraries(tst age)
target_compile_definitions(tst PRIVATE RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/Resources/" CMAKE_DEFINITION)

# save exe path
file(RELATIVE_PATH exe_path ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
file(WRITE ${PROJECT_SOURCE_DIR}/.build/exe_path.txt ".build/${exe_path}/tst")

add_dependencies(tst age)
