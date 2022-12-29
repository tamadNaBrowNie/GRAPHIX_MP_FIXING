#pragma once
#include "Players.h"
#include "Cameras.h"
#define P_CD 0.1f
const float THETA0 = 90.f;

const glm::vec3 fps_off = glm::vec3(0,1,0);
class Handler
{
public:
	MyCamera* cam;
	PlayerClass* player;

};

enum class Mode
{
	TPS,
	FPS,
	TD
};