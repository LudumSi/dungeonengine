#pragma once

#include <iostream>
#include <fstream>

#include "texture_atlas.h"

//Function to get shader file data
std::string get_file(const char* filename);

//Function to make shader program
unsigned int get_shaders(const char* shader_name);

unsigned int texture_from_atlas(TextureAtlas* atlas);

