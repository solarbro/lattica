set(TEST_SRC
"viewer.cpp"
"gl_core_3_3.c"
"imgui_impl_opengl3.cpp"
"imgui_impl_glfw.cpp"
)

set(TEST_EXE viewer)

add_executable(${TEST_EXE} ${TEST_SRC})

target_include_directories(${TEST_EXE} PUBLIC ${PROJECT_SOURCE_DIR}/dependencies/)
target_include_directories(${TEST_EXE} PUBLIC ${PROJECT_SOURCE_DIR}/include/)

target_link_libraries(${TEST_EXE} ${PROJECT_SOURCE_DIR}/dependencies/build/cmake_imgui/libimgui.a)
target_link_libraries(${TEST_EXE} glfw)
target_link_libraries(${TEST_EXE} GL)
target_link_libraries(${TEST_EXE} pthread)
target_link_libraries(${TEST_EXE} lattica)

add_dependencies(${TEST_EXE} lattica)

