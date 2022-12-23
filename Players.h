#pragma once

#include "Models.h"
#include "light.h"
#include <GLFW/glfw3.h>
/// <summary>
/// builder classes allow you to chain methods. similar to java builder classes
/// </summary>

class PlayerClass : public ModelClass
{
private:
	const float ZX_VELOCITY = 0.3f;
	const float Y_VELOCITY = 0.3f;
	const float ROTATION_SPEED = 2.0f;
	const float LIGHT_SWAP_COOLDOWN = 0.2f;
	float timeOfLastLightStrengthSwap = 0.0f;
	enum class Intensity
	{
		LOW,
		MED,
		HI
	};
	Intensity str = Intensity::LOW;

public:
	
	glm::vec3 playerPos;
	
	glm::vec3 playerRot;
	
	float playerScale;
	
	lightBuilder* bulb;
	
	glm::mat4 transformationMatrix;
	
	glm::vec3 front;

	PlayerClass(std::string path,
		glm::vec3 pos,
		glm::vec3 rot,
		float scale);

	void cycleStrength();

	void placeUnifs(GLint* unifs);

	inline void placeLight(GLint unif)
	{
		bulb->placeLight(unif);
	}

	void draw(GLuint shaderProgram);

	float getDepth()
	{
		return this->playerPos.y;
	}
	void cycleMode();
	void kbCallBack(GLFWwindow* window, 
					int key, 
					int scancode, 
					int action, 
					int mods);
};

