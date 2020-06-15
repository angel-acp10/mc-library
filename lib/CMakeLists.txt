add_library(lib
	mc-draw.h
	mc-draw.c
	mc-display.h
	mc-display.c
	mc-text.h
	mc-text.c
	)

target_link_libraries(lib fonts)
target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
