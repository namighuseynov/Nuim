#include "nmpch.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Input.h"

bool Nuim::Input::pressed = 0;


float Nuim::Input::GetAxis(const KeyCode& key) {
    return 0.0f;
}

int Nuim::Input::GetKey(GLFWwindow* window, const Nuim::KeyCode& key) {
    if (glfwGetKey(window, (int)key) == GLFW_PRESS) {
        pressed = true;
        return 1;
    }
    pressed = false;
    return 0;
}

int Nuim::Input::GetKeyDown(GLFWwindow* window, const KeyCode& key) {
    if (!pressed) {
        return 1;
    }
    return 0;
}

int Nuim::Input::GetKeyUp(GLFWwindow* window, const KeyCode& key) {
    return 0;
}

int Nuim::Input::GetMouseButton(GLFWwindow* window, const KeyCode& key) {
    return 0;
}

void Nuim::Input::Update() {

}
