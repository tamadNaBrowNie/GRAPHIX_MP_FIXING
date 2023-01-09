#include "TDCam.h"

void OrthoCamera::kbCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	const float speed = 0.1f;
	glm::vec3 right = glm::cross(this->worldUp, this->forward);
	glm::vec3 move = glm::vec3(0);
	Handler* hand = (Handler*)glfwGetWindowUserPointer(window);
	PlayerClass player = *(hand->player);

	switch (key)
	{
	case GLFW_KEY_W:
		move = -speed * this->worldUp;
		//viewMatrix =glm::translate(viewMatrix, -speed * this->worldUp);
		break;
	case GLFW_KEY_S: move = speed * this->worldUp;
		break;
	case GLFW_KEY_A: move = -speed * glm::normalize(right);
		break;
	case GLFW_KEY_D:move = speed * glm::normalize(right);

		break;
	case GLFW_KEY_2:
		cameraCenter = player.playerPos;
		cameraPos = cameraCenter - forward;
		setView();
		break;
	}
	if (move != glm::vec3(0) && move.y == 0)
		viewMatrix = glm::translate(viewMatrix, move);

}


