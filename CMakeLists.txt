cmake_minimum_required(VERSION 3.20.0)
project(Dungeon)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

add_compile_options(
       -g3
)

add_executable(client 
	main.cpp 
	client/graphics/render.cpp
	client/graphics/sprite.cpp
	client/graphics/texture.cpp
	client/graphics/texture_atlas.cpp
	client/control.cpp
	common/ecs/ecs.cpp
	common/glad.c
	common/physics.cpp
	common/transform.cpp
)

target_include_directories(client PRIVATE include)
target_include_directories(client PRIVATE common)
target_include_directories(client PRIVATE common/ecs)
target_include_directories(client PRIVATE client)
target_include_directories(client PRIVATE extern)

target_link_libraries(client "${CMAKE_CURRENT_SOURCE_DIR}/lib/libglfw3.a")

install(TARGETS client DESTINATION "./")