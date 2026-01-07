include_directories(cimgui/imgui)

add_library(
	cimgui
	
	"cimgui/cimgui.cpp"
	"cimgui/imgui/imgui.cpp"
	"cimgui/imgui/imgui_draw.cpp"
	"cimgui/imgui/imgui_demo.cpp"
	"cimgui/imgui/imgui_widgets.cpp"
	"cimgui/imgui/imgui_tables.cpp"
	"cimgui/imgui/backends/imgui_impl_sdl3.cpp"
	"cimgui/imgui/backends/imgui_impl_opengl3.cpp"
)
target_link_libraries(cimgui SDL3)
target_compile_definitions(cimgui PUBLIC "-DIMGUI_IMPL_API=extern \"C\" ")