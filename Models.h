#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>



#include <string>
#include <vector>
//TODO: restructure. compose model to enemy and player.
//if to make this to engine, probably find a way to make this composable to new classes devs will create
class ModelClass
{
protected:
	std::string objPath;
	std::vector<GLfloat> vertexData;
	std::vector<GLuint> textures;
	bool withNormals = false;
	GLuint VAO, VBO;
	glm::vec3 place;
	glm::vec3 spin;
	GLfloat	scale;
public:
	inline ModelClass(std::string path) : objPath(path),
		VAO(NULL),
		VBO(NULL),place(0),spin(0),scale(0) {}

	void loadObj();

	void attachTexture(std::string texPath, GLint format);
	void attachNormalTexture(std::string texPath, GLint format);
	void createVAO_VBO();
	inline glm::vec3 getPlace() {
		return place;
	}
	inline glm::vec3 getSpin() {
		return spin;
	}
	inline GLfloat getScale() {
		return scale;
	}
	inline GLuint getVAO()
	{
		return this->VAO;
	}

	inline GLuint getBaseTexture()
	{
		return this->textures[0];
	}

	inline GLuint getNormals()
	{
		return this->textures[1];
	}

	inline std::vector<GLfloat> getVertexData()
	{
		return this->vertexData;
	}
};

