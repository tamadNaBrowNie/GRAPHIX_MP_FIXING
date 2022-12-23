#include "Players.h"

PlayerClass::PlayerClass(std::string path, 
	glm::vec3 pos, 
	glm::vec3 rot, 
	float scale) : ModelClass(path),
playerPos(pos),
playerRot(rot),
playerScale(scale),
bulb(new lightBuilder()),
front(glm::vec3(0, 0, 0)),
transformationMatrix(glm::mat4(1.0))
{
	glm::vec3 src = pos;
	src.z -= 0.7;
	bulb
		->setAmbStr(1)
		->setSpecPhong(10)
		->setSpecStr(1)
		->setLightVec(&src)
		->setLightColor(new glm::vec3(1))
		->setAmbColor(new glm::vec3(1));
		cycleStrength();
}


void PlayerClass::placeUnifs(GLint* unifs)
{
	cycleStrength();
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
void PlayerClass::cycleStrength() {
		switch (this->str)
		{
		case Intensity::LOW:
			bulb->setLumens(1);
			break;
		case Intensity::MED:
			bulb->setLumens(2);
			break;
		case Intensity::HI:
			bulb->setLumens(3);
			break;
		default:
			break;
		}
}
void PlayerClass::cycleMode() {
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
	default:
		this->str = Intensity::LOW;
		break;
	}
}
void PlayerClass::kbCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	// Submarine Forward/Backward movement
	if (key == GLFW_KEY_W)
	{

			this->playerPos -= ZX_VELOCITY * front;
			
			
	}
	else if (key == GLFW_KEY_S)
	{
		this->playerPos += ZX_VELOCITY * front;
		
	}

	// Submarine Ascend/Descend movement
	if (key == GLFW_KEY_Q && playerPos.y + 0.1f <= 0)
	{
		this->playerPos.y += Y_VELOCITY;
	}
	else if (key == GLFW_KEY_E)
	{
		this->playerPos.y -= Y_VELOCITY;
	}

	// Submarine Left/Right rotation movement
	if (key == GLFW_KEY_A)
	{
		this->playerRot.y += ROTATION_SPEED;
	}
	else if (key == GLFW_KEY_D)
	{
		this->playerRot.y -= ROTATION_SPEED;
	}

	if (key == GLFW_KEY_F &&
		(timeOfLastLightStrengthSwap == 0 ||
			glfwGetTime() - timeOfLastLightStrengthSwap > LIGHT_SWAP_COOLDOWN))
	{
		cycleMode();

		timeOfLastLightStrengthSwap = glfwGetTime();
	}

	front.x = playerRot.y == 90 ? 0 : -glm::cos(glm::radians(playerRot.y));
	front.z = playerRot.y == 90 ? 1 : glm::sin(glm::radians(playerRot.y));

	front = glm::normalize(front);

	glm::vec3 pos = playerPos;

	pos += glm::normalize(front);

	glm::vec3 lightPos = playerPos;

	const float OFFSET = 0.8f;

	lightPos.z -= OFFSET;
	lightPos += front;

	bulb->setLightVec(&lightPos);
}