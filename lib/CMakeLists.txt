add_library(lib
	mc-draw.h
	mc-draw.c
	mc-text.h
	mc-text.c
	mc-obj.h
	mc-obj.c
	mc-shapes.h
	mc-shapes.c
	mc-input.h
	mc-input.c
	mc-menu.h
	mc-menu.c
	linked_list.h
	linked_list.c
	)

target_link_libraries(lib fonts)
target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
