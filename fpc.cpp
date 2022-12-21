#include "fpc.h"

void cam1p::kbCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key)
	{/*
	 case GLFW_KEY_W:viewMatrix = glm::translate(viewMatrix, -speed * this->worldUp);
	 break;
	 case GLFW_KEY_S: viewMatrix = glm::translate(viewMatrix, speed * this->worldUp);
	 break;
	 case GLFW_KEY_A: viewMatrix = glm::translate(viewMatrix, -speed * glm::normalize(right));
	 break;
	 case GLFW_KEY_D: viewMatrix = glm::translate(viewMatrix, speed * glm::normalize(right));
	 break;
	 default:
	 break;*/
	}
}
void cam1p::moveCam(glm::vec3* pos) {
	cameraPos += *pos;
	cameraCenter = cameraPos + forward;
	setView();
}

void cam1p::rotateCam(float deg) {
	viewMatrix = glm::rotate(viewMatrix,
		glm::radians(deg - 90),
		glm::vec3(0, 1, 0));
}
