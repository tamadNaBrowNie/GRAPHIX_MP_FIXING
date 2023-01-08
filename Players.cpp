#include "Players.h"

void PlayerClass::cycleLight()
{
	if (glfwGetTime() - timeOfLastLightStrengthSwap
		<= LIGHT_SWAP_COOLDOWN)
		return;
	switch (this->str)
	{
	case Intensity::LOW:
		this->str = Intensity::MED;
		break;
	case Intensity::MED:
		this->str = Intensity::HI;
		break;
	case Intensity::HI:
		this->str = Intensity::LOW;
		break;
	default:break;
	}
	timeOfLastLightStrengthSwap = glfwGetTime();
}

PlayerClass::PlayerClass(std::string path, glm::vec3 pos, glm::vec3 rot, float scale) :
	ModelClass(path),
	playerPos(pos),
	playerRot(rot),
	playerScale(scale),
	bulb(new lightBuilder()),
	front(glm::normalize(glm::vec3(0, 0, 1))),
	transformationMatrix(glm::mat4(1.0)),
	str(Intensity::LOW)
{
	bulb->setLumens(0.5)
		->setAmbStr(1)
		->setSpecPhong(10)
		->setSpecStr(1)
		->setLightVec(new glm::vec3(front + pos))
		->setLightColor(new glm::vec3(1))
		->setAmbColor(new glm::vec3(1));
}

void PlayerClass::placeUnifs(GLint* unifs)
{
	switch (this->str)
	{
	case Intensity::LOW:
		bulb->setLumens(0.4);
		break;
	case Intensity::MED:
		bulb->setLumens(0.8);
		break;
	case Intensity::HI:
		bulb->setLumens(1.2);
		break;
	default:
		break;
	}

	bulb->placeUnifs(unifs);
}

void PlayerClass::draw(GLuint shaderProgram)
{
	glUseProgram(shaderProgram);
	glBindVertexArray(this->VAO);

	// Initialize transformation matrix, and assign position, scaling, and rotation
	transformationMatrix = glm::translate(glm::mat4(1),
		this->playerPos);

	// Scale
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(this->playerScale));

	// X-axis rotation
	transformationMatrix = glm::rotate(transformationMatrix,
		glm::radians(this->playerRot.x),
		glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
	// Y-axis rotation
	transformationMatrix = glm::rotate(transformationMatrix,
		glm::radians(this->playerRot.y),
		glm::normalize(glm::vec3(0, 0.5f, 0)));
	// Z-axis rotation
	transformationMatrix = glm::rotate(transformationMatrix,
		glm::radians(this->playerRot.z),
		glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));

	// Initialize transformation location, and assign transformation
	unsigned int transformationLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

	glActiveTexture(GL_TEXTURE0);
	GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
	glBindTexture(GL_TEXTURE_2D, this->textures[0]);
	glUniform1i(tex0Address, 0);

	if (withNormals)
	{
		glActiveTexture(GL_TEXTURE1);
		GLuint tex1Address = glGetUniformLocation(shaderProgram, "norm_tex");
		glBindTexture(GL_TEXTURE_2D, this->textures[1]);
		glUniform1i(tex1Address, 1);
	}

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, this->vertexData.size() / 5);
}

void PlayerClass::kbCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_F:cycleLight(); break;
	case GLFW_KEY_W:this->playerPos += ZX_V * front;break;
	case GLFW_KEY_S:this->playerPos -= ZX_V * front; break;
	case GLFW_KEY_A:this->playerRot.y += DELTA; break;
	case GLFW_KEY_D:this->playerRot.y -= DELTA; break;
	case GLFW_KEY_E:this->playerPos.y -= Y_SPEED; break;
	case GLFW_KEY_Q:
		if (this->playerPos.y < 0)
			this->playerPos.y += Y_SPEED;
		break;
	default:break;
	}

	front.x = playerRot.y == 90 ? 0 : glm::cos(glm::radians(playerRot.y));
	front.z = playerRot.y == 90 ? -1 : -glm::sin(glm::radians(playerRot.y));

	front = glm::normalize(front);

	bulb->setLightVec(new glm::vec3(playerPos + front));
}

