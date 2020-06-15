add_library(fonts
    mc-font.h
	helvetica_12.c
	)

target_include_directories(fonts PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
