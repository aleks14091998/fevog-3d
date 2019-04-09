#ifndef _SCENE_H
#define _SCENE_H

#include"HeadersGame.h"
#include<vector>
#include"objects/Camera.h"

class CObject;
class CCamera;

class CScene {
private:
	valk::IRenderAPI *renderer;
	std::vector<IObject*> objectList;
public:
	void init();
	void process();
	void render();
	void destroy();

	CScene(Editor *editor);
	~CScene();

	void addObj(IObject *obj);
	void removeObj(IObject *obj);

	std::vector<IObject*> GetObjectList() {
		return objectList;
	}
};


#endif