#ifndef _RENDER_GL43_H
#define _RENDER_GL43_H

#include"common.h"
#include"GL43.h"

class CCore;

class CRenderGL43 : public IRenderAPI {
private:
	CCore *core;
	
	const float	_c_fFov, _c_fZNear, _c_fZFar;

	glm::mat4 Projection;
	glm::mat4 View;

	void _MessageProc(const TWinMessage &stMsg);
	void SetDefaultStates();
	void _OnResize();

	static void CALLBACK s_MessageProc(void *pParametr, const TWinMessage &stMsg);
	void InitGrid();
	void DrawGrid();

public:
	CRenderGL43(CCore *core);
	~CRenderGL43();

	void Init();
	void StartFrame();
	void EndFrame();
	void Draw(valk::render::ResourceList *list);
	void DrawCamera(valk::render::CameraParameters *list);

	IJTS_BASE_IMPLEMENTATION(IRenderAPI);
};

#endif
