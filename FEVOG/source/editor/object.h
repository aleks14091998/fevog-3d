#ifndef _OBJECT_H
#define _OBJECT_H

#include"HeadersGame.h"
#include"component.h"
#include<vector>
#include"iobject.h"

class CObject : IObject {
private:
	float i = 0;//test
	valk::IInput *input;
	CRenderComponents* render_components;
	std::vector<IComponents*> components;
public:
	CObject(Editor *editor);
	~CObject();

	void init();
	void process();
	void render();
	void destroy();
};

#endif