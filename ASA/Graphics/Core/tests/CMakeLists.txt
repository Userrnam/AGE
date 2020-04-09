add_executable(tst src/main.cpp)

target_link_libraries(tst core)

# save exe path
file(RELATIVE_PATH exe_path ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
file(WRITE ${PROJECT_SOURCE_DIR}/.build/exe_path.txt ".build/${exe_path}/tst")
