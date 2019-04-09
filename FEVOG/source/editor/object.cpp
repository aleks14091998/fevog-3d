#include"object.h"

CObject::CObject(Editor *editor) : 
	input(editor->GetInput()),
	render_components(new CRenderComponents(editor))
{

}

CObject::~CObject() {
	delete render_components;
}

void CObject::init() {
	//test
	render_components->SetMesh((char*)"test.obj");
	render_components->SetShader((char*)"standart_texture");
	render_components->SetTexture((char*)"resource/img/container.png", E_TEXTURE_TYPE::DEFAULT);

	render_components->SetPosition(-20 + rand() % 20, 0, -20 + rand() % 20);
	render_components->SetScale(1 + rand() % 5, 1 + rand() % 5, 1 + rand() % 5);
	render_components->SetRotation(1, 1, 1, 0);
	//test
}

void CObject::process() {
//	render_components->SetRotation(1, 0, 0, i);
//	i += 0.03;
}

void CObject::render() {
	render_components->render();
	for (unsigned int i = 0; i < components.size(); i++) {
		components[i]->render();
	}
}

void CObject::destroy() {

}