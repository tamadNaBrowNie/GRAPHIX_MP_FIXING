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
