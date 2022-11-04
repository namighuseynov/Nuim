#include "nmpch.h"

#include "Input.h"

float Nuim::Input::GetAxis(const KeyCode& key) {
    return 0.0f;
}

int Nuim::Input::GetKey(GLFWwindow* window, const Nuim::KeyCode& key) {
    if (GetKey(window, key) == GLFW_PRESS) {
        return 1;
    }
    return 0;
}

int Nuim::Input::GetKeyDown(GLFWwindow* window, const KeyCode& key) {
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
