#include "Misc.h"
#include  "fpc.h"

void cam1p::kbCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Handler* hand = (Handler*)glfwGetWindowUserPointer(window);
	hand->player->kbCallBack(window, key, scancode, action, mods);

	forward = hand->player->front;
	cameraPos = hand->player->playerPos + hand->player->front;
	cameraCenter = forward + cameraPos;
	setView();
}
void cam1p::moveCam(glm::vec3* pos) {
	cameraPos = *pos;
}

void cam1p::rotateCam(float deg) {
	//viewMatrix = glm::rotate(viewMatrix,
	//	glm::radians(deg - 90),
	//	glm::vec3(0, 1, 0));
	float cosine = glm::cos(deg);
	float sine = glm::sin(deg);
	glm::mat2 rotMat = {
		cosine,-sine,
		sine,cosine
	};

	glm::vec2 xz(forward.x, forward.z);
	glm::vec2 final = rotMat * xz;
	forward = -glm::vec3(final.x, 0, final.y);
}
