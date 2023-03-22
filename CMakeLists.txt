cmake_minimum_required(VERSION 3.20.0)
project(Dungeon)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

add_compile_options(
       -g3
)

add_executable(client 
	client/main.cpp 
	client/graphics/render.cpp
	client/graphics/camera.cpp
	client/graphics/render_helpers.cpp
	client/graphics/sprite_render.cpp
	client/graphics/sprite.cpp
	client/graphics/text_render.cpp
	client/graphics/texture.cpp
	client/graphics/texture_atlas.cpp
	client/control.cpp
	common/ecs/entity.cpp
	common/ecs/system.cpp
	common/ecs/world.cpp
	common/ecs/entityhandle.cpp
	common/glad.c
	common/physics.cpp
	common/transform.cpp
	common/network/network_manager.cpp
)

add_executable(server 
	server/main.cpp
	common/ecs.cpp
	common/glad.c
	common/physics.cpp
	common/transform.cpp
	common/network/network_manager.cpp
)

if(WIN32)
  target_link_libraries(client wsock32 ws2_32)
  target_link_libraries(server wsock32 ws2_32)
endif()

target_include_directories(client PRIVATE include)
target_include_directories(client PRIVATE common)
target_include_directories(client PRIVATE common/ecs)
target_include_directories(client PRIVATE client)
target_include_directories(client PRIVATE extern/freetype-2.12.1/include)

target_include_directories(server PRIVATE include)
target_include_directories(server PRIVATE common)
target_include_directories(server PRIVATE client)
target_include_directories(server PRIVATE extern)

target_link_libraries(client "${CMAKE_CURRENT_SOURCE_DIR}/lib/libglfw3.a")

install(TARGETS client DESTINATION "./")

add_subdirectory(extern/freetype-2.12.1)

target_link_libraries(client freetype)