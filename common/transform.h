#pragma once

#include "glm/glm.hpp"

class Transform {

	public:

		Transform();
		Transform(float x, float y);

		//Should be changed out to be a mat3 at some point
		glm::mat4 transform;

		glm::vec2 get_position();
		void set_position(float x, float y);
		void translate(glm::vec2 delta);

		float get_scale();
		void set_scale(float scale);

		//Radians
		float get_rotation();
		void set_rotation(float angle);
		void rotate(float angle);
};

struct PhysicsComp {
	glm::vec2 velocity;
	glm::vec2 acceleration;
};