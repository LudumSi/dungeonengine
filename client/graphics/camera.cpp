#include "camera.h"

#include "glm\gtc\matrix_transform.hpp"

Camera::Camera(GLFWwindow* window){

    //Set up height and width
    int win_height, win_width;
	this->window = window;
	glfwGetFramebufferSize(window, &win_width, &win_height);

    //TODO: Fix things so that the Y-axis works as traditionally expected
	this->projection = glm::ortho(-1.f * (float)win_width/2, (float)win_width/2, (float)win_height/2, -1.f * (float)win_height/2, -1.0f, 1.0f);

    this->camera_position = glm::vec2(0.0,0.0);
    this->camera_view = glm::mat4(1.0);
}

void Camera::set_camera(glm::vec2 pos){
    camera_position = pos;
    camera_view = glm::translate(glm::mat4(1.f), -1.f * glm::vec3(camera_position, 0.f));
}