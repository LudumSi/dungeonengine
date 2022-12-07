#include "transform.h"
#include "glm/gtc/type_ptr.hpp"
#include <cstdio>

Transform::Transform() {

	this->transform = glm::mat4(1.0);
}

Transform::Transform(float x, float y) {

	this->transform = glm::mat4(1.0);
	transform = glm::translate(transform, glm::vec3(x,y,0.f));

}

glm::vec3 Transform::get_position(){

	glm::vec4 dummy = glm::vec4(this->transform[3]);
	return glm::vec3(dummy);
}

void Transform::translate(glm::vec2 translate) {

	this->transform = glm::translate(this->transform, glm::vec3(translate.x,translate.y,0.f));
}

