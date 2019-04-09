#ifndef _COMPONENT_H
#define _COMPONENT_H

#include"editor.h"
#include"HeadersGame.h"
#include"managers/ResourceManager.h"

class IComponents {
public:
	virtual void render() = 0;
};

class CRenderComponents : public IComponents {
private:
	valk::IRenderAPI *renderer;
	valk::render::ResourceList *Components;
public:
	CRenderComponents(Editor *editor) : renderer(editor->GetRender()), Components(new valk::render::ResourceList){
		Components->Material = new Material();
	}
	~CRenderComponents() {
		delete Components;
	}

	void SetMesh(char *filename) {
		CResourceManager::AddMesh(Components->Mesh, filename);
	}

	void SetShader(char *filename) {
		CResourceManager::AddShader(Components->Shader, filename);
	}

	void SetTexture(char *filename, E_TEXTURE_TYPE type) {
		Components->Material->SetTexture(filename, type);
	}

	inline void SetPosition(float x, float y, float z) {
		Components->transform.translate[0] = x;
		Components->transform.translate[1] = y;
		Components->transform.translate[2] = z;
	}

	inline void SetRotation(int x, int y, int z, float angle) {
		Components->transform.rotate[0] = x;
		Components->transform.rotate[1] = y;
		Components->transform.rotate[2] = z;
		Components->transform.rotateAngle = angle;
	}

	inline void SetScale(float x, float y, float z) {
		Components->transform.scale[0] = x;
		Components->transform.scale[1] = y;
		Components->transform.scale[2] = z;
	}

	void render() {
		renderer->Draw(Components);
	}

};

class CCameraComponents : public IComponents {
private:
	valk::IRenderAPI *renderer;
	valk::render::CameraParameters *Components;
public:
	CCameraComponents(Editor *editor) : renderer(editor->GetRender()), Components(new valk::render::CameraParameters) {

	}
	~CCameraComponents() {
		delete Components;
	}

	inline void SetPosition(float x, float y, float z) {
		Components->Position[0] = x;
		Components->Position[1] = y;
		Components->Position[2] = z;
	}

	inline void SetCenterPoint(float x, float y, float z) {
		Components->CenterPoint[0] = x;
		Components->CenterPoint[1] = y;
		Components->CenterPoint[2] = z;
	}

	inline void GetPosition(float &x, float &y, float &z) {
		x = Components->Position[0];
		y = Components->Position[1];
		z = Components->Position[2];
	}

	inline void GetCenterPoint(float &x, float &y, float &z) {
		x = Components->CenterPoint[0];
		y = Components->CenterPoint[1];
		z = Components->CenterPoint[2];
	}

	void render() {
		renderer->DrawCamera(Components);
	}
};


#endif