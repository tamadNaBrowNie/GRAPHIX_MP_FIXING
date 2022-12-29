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

