#pragma once
#include "Classes/Classes.h"

class Player
{
private:
	OrthoCamera tpc;
	PerspectiveCamera fpc;
	ModelClass obj;
	
	/// TODO:must contain camera, model, the point light and the controls. will add it tomorrow.
public:
	void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void Mouse_Callback(GLFWwindow* window, double xpos, double ypos);

};

