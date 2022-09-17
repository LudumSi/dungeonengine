#include "transform.h"
#include "glm/gtc/type_ptr.hpp"

Transform::Transform() {

	this->transform = glm::mat4(1.0);
}

Transform::Transform(float x, float y) {

	this->transform = glm::mat4(1.0);
	transform = glm::translate(transform, glm::vec3(x,y,0.f));

}

void Transform::translate(glm::vec2 transform) {

	this->transform = glm::translate(this->transform, glm::vec3(transform.x,transform.y,0.f));
}

