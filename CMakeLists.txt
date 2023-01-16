cmake_minimum_required(VERSION 3.20.0)
project(Dungeon)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

add_executable(client 
	client/main.cpp 
	client/graphics/render.cpp
	client/graphics/sprite.cpp
	client/graphics/texture.cpp
	client/graphics/texture_atlas.cpp
	client/control.cpp
	common/ecs.cpp
	common/glad.c
	common/physics.cpp
	common/transform.cpp
)

add_executable(server 
	server/main.cpp
	common/ecs.cpp
	common/glad.c
	common/physics.cpp
	common/transform.cpp
)

target_include_directories(client PRIVATE include)
target_include_directories(client PRIVATE common)
target_include_directories(client PRIVATE client)
target_include_directories(client PRIVATE extern)

target_include_directories(server PRIVATE include)
target_include_directories(server PRIVATE common)
target_include_directories(server PRIVATE client)
target_include_directories(server PRIVATE extern)

target_link_libraries(client "${CMAKE_CURRENT_SOURCE_DIR}/lib/libglfw3.a")

install(TARGETS client DESTINATION "./")