add_library(tests
	test-draw.h
	test-draw.c
	test-text.h
	test-text.c
	)

target_link_libraries(tests lib)
target_include_directories(tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# test linked list
add_executable(test-ll
	test-ll.c)
target_link_libraries(test-ll lib)
