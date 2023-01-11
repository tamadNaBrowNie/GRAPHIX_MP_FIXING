#include "Models.h"
class EnemyClass : public ModelClass
{
public:
	//redundant code.
	glm::vec3 enemyPos;
	glm::vec3 enemyRot;
	float enemyScale;
	EnemyClass(std::string path,
		glm::vec3 pos,
		glm::vec3 rot,
		float scale);
	//EnemyClass(ModelClass*);
	void draw(GLuint shaderProgram);
};
