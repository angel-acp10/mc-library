find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# test draw
add_executable(test-draw
	test-draw.c) 
target_link_libraries(test-draw ${SDL2_LIBRARIES})
target_link_libraries(test-draw lib)

# test linked list
add_executable(test-ll
	test-ll.c)
target_link_libraries(test-ll lib)

# test object
add_executable(test-obj
	test-obj.c)
target_link_libraries(test-obj lib)