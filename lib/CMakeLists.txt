add_library(lib
	mc-draw.h
	mc-draw.c
	mc-display.h
	mc-display.c
	)

target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
