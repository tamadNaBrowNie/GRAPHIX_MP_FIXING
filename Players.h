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
	const float FRONT_VEC = 0.3f;
	const float Y_SPEED = 0.3f;
	const float DELTA = 2.0f;
	const float LIGHT_SWAP_COOLDOWN = 0.2f;
	float timeOfLastLightStrengthSwap = 0.0f;
	enum class Intensity
	{
		LOW,
		MED,
		HI
	};
	Intensity str = Intensity::LOW;
	void cycleLight();

public:
	glm::vec3 playerPos;
	glm::vec3 playerRot;
	float playerScale;
	lightBuilder* bulb;
	glm::mat4 transformationMatrix;
	glm::vec3 front;

	inline PlayerClass(std::string path,
		glm::vec3 pos,
		glm::vec3 rot,
		float scale) : ModelClass(path),
		playerPos(pos),
		playerRot(rot),
		playerScale(scale),
		bulb(new lightBuilder()),
		front(glm::vec3(0, 0, 0)),
		transformationMatrix(glm::mat4(1.0)),
		str(Intensity::LOW)
	{
		bulb->setLumens(2)
			->setAmbStr(1)
			->setSpecPhong(10)
			->setSpecStr(1)
			->setLightVec(new glm::vec3(front + pos))
			->setLightColor(new glm::vec3(1))
			->setAmbColor(new glm::vec3(1));
	}

	inline void placeUnifs(GLint* unifs)
	{
		switch (this->str)
		{
		case Intensity::LOW:
			bulb->setLumens(2);
			break;
		case Intensity::MED:
			bulb->setLumens(3);
			break;
		case Intensity::HI:
			bulb->setLumens(4);
			break;
		default:
			break;
		}

		bulb->placeUnifs(unifs);
	}

	inline void placeLight(GLint unif)
	{
		bulb->placeLight(unif);
	}

	void draw(GLuint shaderProgram);

	float getDepth()
	{
		return this->playerPos.y;
	}

	void kbCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		switch (key)
		{
		case GLFW_KEY_F:cycleLight(); break;
		case GLFW_KEY_W:this->playerPos += FRONT_VEC * front; break;
		case GLFW_KEY_S:this->playerPos -= FRONT_VEC * front; break;
		case GLFW_KEY_A:this->playerRot.y += DELTA; break;
		case GLFW_KEY_D:this->playerRot.y -= DELTA; break;
		case GLFW_KEY_E:this->playerPos.y -= Y_SPEED; break;
		case GLFW_KEY_Q:
			if (this->playerPos.y < 0)
				this->playerPos.y += Y_SPEED; break;
		default:break;
		}

		front.x = playerRot.y == 90 ? 0 : glm::cos(glm::radians(playerRot.y));
		front.z = playerRot.y == 90 ? -1 : -glm::sin(glm::radians(playerRot.y));

		front = glm::normalize(front);

		glm::vec3 sunSpot = playerPos;

		sunSpot += glm::normalize(front);

		bulb->setLightVec(&sunSpot);

		glm::vec3 lightPos = playerPos;

		//const float OFFSET = 0.8f;

		//lightPos.z -= OFFSET;
		lightPos += front;

		bulb->setLightVec(&lightPos);
	}
};

