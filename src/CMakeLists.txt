find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(mc-gui
	main.c
	)

target_link_libraries(mc-gui lib)
target_link_libraries(mc-gui fonts)
target_link_libraries(mc-gui ${SDL2_LIBRARIES})

