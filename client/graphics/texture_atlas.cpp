
#include "texture_atlas.h"
#include <stb_image.h>
#include <iostream>

TextureAtlas::TextureAtlas(int chunk, int size, const char* missing_tex) {
	resolution = chunk;
	side_len = size;

	next_x = 0;
	next_y = 0;

	this->flushed = false;

	/*
	//Create assignment grid and fill it with falses to indicate nothing has been moved
	assignment_grid = new bool*[side_len];

	for (int i = 0; i < side_len; i++) {
		assignment_grid[i] = new bool[side_len];
		for (int j = 0; j < side_len; j++) {
			assignment_grid[i][j] = false;
		}
	}
	*/

	//Create buffer and fill it with empty data
	scanline_size = resolution * side_len;

	//side_len x side_len chunks, each of which are resolution x resolution
	texture = new Texture(scanline_size,scanline_size);

	//Load the missing texture based on the missing texture path
	add_image(missing_tex);
}

TextureAtlas::~TextureAtlas() {

	/*
	if (assignment_grid) {
		for (int i = 0; i < side_len; i++) {
			delete[] assignment_grid[i];
		}

		delete[] assignment_grid;
	}
	*/

	if (texture && !flushed) {
		delete texture;
	}
}

//Removes the preceding "assets/" and the following .*
std::string strip_texture_name(std::string tex_name){

	//Remove the following .*
	int idx = tex_name.find('.');
	if(idx){
		tex_name.erase(tex_name.begin()+idx, tex_name.end());
	}

	//Remove the preceding assets/
	if(tex_name.find("assets/") == 0){
		tex_name.erase(0,7);
	}

	return tex_name;
}

void TextureAtlas::add_image(Texture image, std::string name) {

	//Check to make sure that the atlas is still writeable
	if(flushed){
		std::cout << "Cannot add " << name << " to texture atlas: Atlas not writeable" << std::endl;
	}

	//Check to make sure there are available coordinates
	if (next_y >= side_len) {
		std::cout << "Cannot add " << name << " to texture atlas: Atlas full" << std::endl;
		return;
	}

	//Catch failure to load image
	//Texture loader will already have printed an error
	if (!image.buffer) return;

	//Check that the image height and width are equal to atlas resolution
	//At some point should handle integer multiples but we'll cross that bridge when we get to it
	if (image.height > resolution) {
		std::cerr << "Image " << name << " with a height of " << image.height << " is longer than " << resolution << std::endl;
		return;
	}

	if (image.width > resolution) {
		std::cerr << "Image " << name << " with a width of " << image.width << " is longer than " << resolution << std::endl;
		return;
	}

	//Actually copy the new texture to the next available coordinates
	texture->copy_image(&image, next_x * resolution, next_y * resolution);

	//Save the texture coords in the dictionary along with the stripped texture name
	float tex_x = (float) next_x / (float) side_len;
	float tex_y = (float) next_y / (float) side_len;
	atlas[name] = glm::vec2(tex_x, tex_y);

	//Update the next available coords
	next_x++;
	if (next_x >= side_len) {
		next_x = 0;
		next_y++;
	}

	//Texture is deleted when it goes out of scope
}

void TextureAtlas::add_image(const char* path, std::string name) {

	//Create a new texture based on the path
	add_image(Texture(path), name);
}

void TextureAtlas::add_image(const char* path) {
	add_image(path, strip_texture_name(path));
}

//Adds a raw bitmap
//Bitmap must be 4 bytes per pixel RGBA for this to end up looking right
void TextureAtlas::add_image(unsigned char* bitmap, int width, int height, std::string name){
	add_image(Texture(bitmap, width, height), name);
}

glm::vec2 TextureAtlas::get_coords(const char* name) {

	std::map<std::string, glm::vec2>::iterator it;

	//Return texture coordinates of the texture, otherwise
	//Return debug texture
	it = atlas.find(name);
	if (it != atlas.end()) {
		return it->second;
	}
	else {
		return glm::vec2(0, 0);
	}
}

void TextureAtlas::flush(){
	if(!flushed && texture){
		delete texture;
	}
	flushed = true;
}
