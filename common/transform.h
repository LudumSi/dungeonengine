#pragma once

#include "glm/glm.hpp"

class Transform {

	public:

		Transform();
		Transform(float x, float y);
		Transform(float x, float y, float scale);

		//Should be changed out to be a mat3 at some point
		glm::mat4 transform;

		glm::vec3 get_position();
		void set_position(float x, float y);
		void translate(glm::vec2 delta);

		float get_scale();
		void set_scale(float scale);

		//Radians
		float get_rotation();
		void set_rotation(float angle);
		void rotate(float angle);
};

