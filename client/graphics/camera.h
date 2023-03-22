#pragma once

#include "glm/glm.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW\glfw3.h"

class Camera{

    public:
        GLFWwindow* window;

        glm::vec2 camera_position;
        glm::mat4 camera_view;
        glm::mat4 projection;

        Camera(GLFWwindow* window);
        void set_camera(glm::vec2 pos);
};