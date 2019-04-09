#ifndef _CAMERA_OBJ_H
#define _CAMERA_OBJ_H

#include"../HeadersGame.h"
#include"../component.h"
#include"renderer/Texture.h"
#include <glm/glm.hpp>

class CCamera : IObject{ /// Iobject
private:
	valk::IInput *input;
	CCameraComponents* components;

	glm::vec3 viewMatrix, pos, upMatrix;
	valk::MouseStates mouse;
	float speed;

public:
	CCamera(Editor *editor);
	~CCamera();

	void init();
	void process();
	void render();
	void destroy();

};

#endif