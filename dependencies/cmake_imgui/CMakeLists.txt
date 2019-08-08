set(IMGUI_RELATIVE_PATH ../imgui)
set(IMGUI_SRC
${IMGUI_RELATIVE_PATH}/imgui.cpp
${IMGUI_RELATIVE_PATH}/imgui_widgets.cpp
${IMGUI_RELATIVE_PATH}/imgui_draw.cpp
${IMGUI_RELATIVE_PATH}/imgui_demo.cpp
)

set(LIBIMGUI imgui)

add_library(${LIBIMGUI} STATIC ${IMGUI_SRC})

target_include_directories(${LIBIMGUI} PUBLIC ${IMGUI_RELATIVE_PATH})