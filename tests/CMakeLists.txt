find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# test draw
add_executable(test-draw
	test-draw.c) 
target_link_libraries(test-draw ${SDL2_LIBRARIES})
target_link_libraries(test-draw lib)
target_link_libraries(test-draw m)

# test linked list
add_executable(test-ll
	test-ll.c)
target_link_libraries(test-ll lib)

# test object
add_executable(test-obj
	test-obj.c)
target_link_libraries(test-obj lib)

# test text
add_executable(test-text
	test-text.c) 
target_link_libraries(test-text ${SDL2_LIBRARIES})
target_link_libraries(test-text lib)
target_link_libraries(test-text fonts)

# test menu
add_executable(test-menu
	test-menu.c) 
target_link_libraries(test-menu ${SDL2_LIBRARIES})
target_link_libraries(test-menu lib)
target_link_libraries(test-menu fonts)
