#include "transform.h"
#include "glm/gtc/type_ptr.hpp"
#include <cstdio>

Transform::Transform() {

	this->transform = glm::mat4(1.0);
}

Transform::Transform(float x, float y) {

	this->transform = glm::mat4(1.0);
	this->translate(glm::vec2(x,y));
}

Transform::Transform(float x, float y, float scale){
	this->transform = glm::mat4(1.0);
	this->translate(glm::vec2(x,y));
	this->set_scale(scale);
	
}

glm::vec3 Transform::get_position(){

	glm::vec4 dummy = glm::vec4(this->transform[3]);
	return glm::vec3(dummy);
}

void Transform::translate(glm::vec2 translate) {

	this->transform = glm::translate(this->transform, glm::vec3(translate.x,translate.y,0.f));
}

void Transform::set_scale(float scale) {
	this->transform = glm::scale(this->transform, glm::vec3(scale, scale, scale));
}

void Transform::rotate(glm::vec2 hub, float angle){
	glm::vec3 offset = glm::vec3(hub.x, hub.y, 0.f);
	this->transform = glm::translate(this->transform, -1.f * offset);
	this->transform = glm::rotate(this->transform, angle, glm::vec3(0.f,0.f,1.f));
	this->transform = glm::translate(this->transform, offset);
}

